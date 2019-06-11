#include "ofApp.h"
#include "chartdir/chartdir.h"
#include "ofxAsync.h"

#define WIDGET_WIDTH 500
#define WIDGET_HEIGHT 360
#define QUESTION_DELAY 4000 // in milliseconds

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(10);
    ofBackground(255);
    qNum = 0;
    ready = false;
    newImageAvailable = false;
    std::string file = "questions.json";
    large_font.load("Roboto-Regular.ttf", 48);
    medium_font.load("Roboto-Regular.ttf", 28);
    small_font.load("Roboto-Regular.ttf", 20);

    nameInput.setup();
    nameInput.setFont(medium_font);
    nameInput.setBounds(200, 375, 500, 65);

    graphData.setUseTexture(false);
    faceData.setUseTexture(false);
    cam.setUseTexture(false);
    cam.initGrabber(640, 480);
    blur.setup(cam.getWidth(), cam.getHeight(), 10, .2, 2);

    if (inputJSON.open(file)) {
        currentQuestionText = inputJSON["questions"][qNum]["text"].asString();
        for (int i = 0; i < inputJSON["numAxes"].asInt(); i++) {
            axesLabels.push_back(inputJSON["axesLabels"][i].asString());
            axesValues.push_back(0.0);
        }
    } else {
        ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    ofxAsync::update();
    if (ready) {
        // Graph/Face generation thread is done
        ofxAsync::waitForAll();

        graph.loadData(graphData.getPixels());
        face.loadData(faceData.getPixels());

        blur.begin();
        face.draw(0,0);
        blur.end();

        ready = false;
        newImageAvailable = true;
        ofResetElapsedTimeCounter();
        logInfo();
    }
    if (face.isAllocated() && ofGetElapsedTimeMillis() > QUESTION_DELAY) {
        face.clear();
        
        // Load next question
        qNum++;
        if (qNum == inputJSON["questions"].size()) qNum = 0; // cycle for debugging purposes
        currentQuestionText = inputJSON["questions"][qNum]["text"].asString();
    }
}

//--------------------------------------------------------------
void ofApp::logInfo() {
    ofLog() << "User: " << userName << endl;
    ofLog() << "Question #" << qNum + 1 << endl;
    ofLog() << "\"" << currentQuestionText << "\"" << endl;
    ofLog() << "User answered: \"" << answer << "\"" << endl;
    ofLog() << ofGetTimestampString("%c") << endl;
    ofLog() << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (userName.empty()) {
        drawLogin();
        return;
    }
    
    // Draw info and question text
    ofSetColor(0);
    large_font.drawString("Hello, " + userName, 75, 150);
    ofSetColor(150);
    small_font.drawString("Answer the questions below and watch \nhow your Impact Graph changes!", 75, 225);
    ofSetColor(0);
    medium_font.drawString(currentQuestionText, 75, 540);
    ofSetColor(255);
    
    if (graph.isAllocated()) {
        // Draw radar chart
        graph.draw(ofGetWidth() - WIDGET_WIDTH, 0);
    }

    if (face.isAllocated()) {
        // Draw filtered webcam image
        blur.draw(ofRectangle(ofGetWidth() - WIDGET_WIDTH, WIDGET_HEIGHT, WIDGET_WIDTH, WIDGET_HEIGHT));
        
        answer == "Yes" ? ofSetColor(ofColor::lightGreen) : ofSetColor(ofColor::coral);
        medium_font.drawString(answer, 75, 580);
        ofSetColor(255);
        
        if (newImageAvailable) {
            newImageAvailable = false;
            saveImage();
        }
    }
}

//--------------------------------------------------------------
void ofApp::drawLogin() {
    ofSetColor(0);
    large_font.drawString("Welcome!", 200, 275);
    ofSetColor(150);
    small_font.drawString("Type your name and press enter to get started", 200, 325);
    ofSetColor(0);
    ofPushStyle();
    nameInput.draw();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::saveImage() {
    ofImage img;
    
    // Get image from screen
    img.grabScreen(ofGetWidth() - WIDGET_WIDTH, WIDGET_HEIGHT, WIDGET_WIDTH, WIDGET_HEIGHT);
    img.setUseTexture(false);
    
    // Save image on new thread
    ofxAsync::run([img, this]() mutable {
        ofPixels pixels = img.getPixels();
        pixels.swapRgb();
        img.setFromPixels(pixels);
        img.save(userName + "/" + to_string(qNum + 1) + ".jpg");
    });
}

//--------------------------------------------------------------
void ofApp::handleYesNo(bool yes){
    if (face.isAllocated() || userName.empty()) {
        // Ignore input when still showing previous last question
        // or if user isn't logged in yet
        return;
    }

    // Update values according to answer
    vector<int> delta;
    for (int i = 0; i < axesValues.size(); i++) {
        delta.push_back(inputJSON["questions"][qNum][yes ? "yes" : "no"][i].asInt());
        axesValues[i] += delta[i];
        if (axesValues[i] < 0) axesValues[i] = 0;
    }
    answer = yes ? "Yes" : "No";

    // Launch thread
    ofxAsync::run([delta, this]() {
        makeGraph();
        makeFace(delta);
        ready = true; // signal that we're done
    });
}

//--------------------------------------------------------------
void ofApp::makeGraph(){
    // Create graph image using ChartDirector
    PolarChart *c = new PolarChart(WIDGET_WIDTH, WIDGET_HEIGHT);
    c->setPlotArea(245, 185, 150);
    c->addAreaLayer(DoubleArray(axesValues.data(), (int)axesValues.size()), 0x9999ff);
    const char *labels[axesLabels.size()];
    for (int i = 0; i < axesLabels.size(); i++) {
        labels[i] = axesLabels[i].c_str();
    }
    c->angularAxis()->setLabels(StringArray(labels, (int)axesLabels.size()));
    c->makeChart("simpleradar.png");
    delete c;

    // Load graph image
    ofSetDataPathRoot("../Resources/"); // look for image where ChartDirector saved it
    graphData.load("simpleradar.png");
    ofFile::removeFile("simpleradar.png");
    ofSetDataPathRoot("../../../data"); // reset data path
}

//--------------------------------------------------------------
void ofApp::makeFace(const vector<int>& delta){
    // Compute statistics
    float sum = 0.0;
    int max = -5;
    int min = 5;
    for (int i = 0; i < delta.size(); i++) {
        max = std::max(max, delta[i]);
        min = std::min(min, delta[i]);
        sum += delta[i];
    }
    float normalized_range = (max - min) / 5.0;
    float average = sum / delta.size();

    // Apply saturation filter
    cam.update();
    faceData.setFromPixels(cam.getPixels());
    for (int i = 0; i < faceData.getWidth(); i++) {
        for (int j = 0; j < faceData.getHeight(); j++) {
            ofColor pixel = faceData.getColor(i, j);
            pixel.setSaturation(pixel.getSaturation() * abs(average));
            faceData.setColor(i, j, pixel);
        }
    }
    faceData.update();

    // Apply blur filter
    blur.setScale(normalized_range);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'y') {
        // 'Y' pressed
        handleYesNo(true);
    }
    if (key == 'n') {
        // 'N' pressed
        handleYesNo(false);
    }
    if (key == OF_KEY_RETURN && userName.empty() && !nameInput.getText().empty()) {
        userName = nameInput.getText();
        // Set up folder for saving images
        if (ofDirectory::doesDirectoryExist(userName)) {
            ofDirectory::removeDirectory(userName, true);
        }
        ofDirectory::createDirectory(userName);
    }
}
