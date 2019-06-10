#include "ofApp.h"
#include "chartdir/chartdir.h"
#include "ofxAsync.h"

#define WIDGET_WIDTH 500
#define WIDGET_HEIGHT 360

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(10);
    qNum = 0;
    ready = false;
    newImageAvailable = false;
    std::string file = "questions.json";

    userName = "Ashwin Agarwal";
    if (ofDirectory::doesDirectoryExist(userName)) {
        ofDirectory::removeDirectory(userName, true);
    }
    ofDirectory::createDirectory(userName);

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
        // Thread is done
        ofxAsync::waitForAll();

        graph.loadData(graphData.getPixels());
        face.loadData(faceData.getPixels());

        blur.begin();
        face.draw(0,0);
        blur.end();

        qNum++;
        if (qNum == 3) qNum = 0;
        currentQuestionText = inputJSON["questions"][qNum]["text"].asString();

        ready = false;
        newImageAvailable = true;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofDrawBitmapString(currentQuestionText, 10, 14);
    if (graph.isAllocated()) {
        graph.draw(ofGetWidth() - WIDGET_WIDTH, 0);
    }

    if (face.isAllocated()) {
        blur.draw(ofRectangle(ofGetWidth() - WIDGET_WIDTH, WIDGET_HEIGHT, WIDGET_WIDTH, WIDGET_HEIGHT));

        if (newImageAvailable) {
            newImageAvailable = false;
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
    }
}

//--------------------------------------------------------------
void ofApp::handleYesNo(bool yes){
    vector<int> delta;
    // Update values according to answer
    for (int i = 0; i < axesValues.size(); i++) {
        delta.push_back(inputJSON["questions"][qNum][yes ? "yes" : "no"][i].asInt());
        axesValues[i] += delta[i];
        if (axesValues[i] < 0) axesValues[i] = 0;
    }

    // Launch thread
    ofxAsync::run([delta, this]() {
        makeGraph();
        makeFace(delta);
        ready = true; // signal that we're done
    });
}

//--------------------------------------------------------------
void ofApp::makeGraph(){
    // Create graph image
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
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
