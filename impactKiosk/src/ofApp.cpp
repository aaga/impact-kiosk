#include "ofApp.h"
#include "chartdir/chartdir.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    qNum = 0;
    std::string file = "questions.json";
    
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

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofDrawBitmapString(currentQuestionText, 10, 14);
    if (graph.isAllocated()) {
        graph.draw(ofGetWidth() - graph.getWidth(), 0);
    }

}

void ofApp::handleYesNo(bool yes){
    qNum++;
    if (qNum == 2) qNum = 0;
    currentQuestionText = inputJSON["questions"][qNum]["text"].asString();
    for (int i = 0; i < axesValues.size(); i++) {
        axesValues[i] += inputJSON["questions"][qNum][yes ? "yes" : "no"][i].asInt();
        if (axesValues[i] < 0) axesValues[i] = 0;
    }
    makeGraph();
}

void ofApp::makeGraph(){
    PolarChart *c = new PolarChart(500, 360);
    c->setPlotArea(245, 185, 150);
    c->addAreaLayer(DoubleArray(axesValues.data(), (int)axesValues.size()), 0x9999ff);
    const char *labels[axesLabels.size()];
    for (int i = 0; i < axesLabels.size(); i++) {
        labels[i] = axesLabels[i].c_str();
    }
    c->angularAxis()->setLabels(StringArray(labels, (int)axesLabels.size()));
    c->makeChart("simpleradar.png");
    delete c;
    ofSetDataPathRoot("../Resources/");
    graph.load("simpleradar.png");
    ofSetDataPathRoot("../../../data");
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
