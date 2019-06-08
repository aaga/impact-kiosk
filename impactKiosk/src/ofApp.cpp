#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    std::string file = "questions.json";
    
    if (inputJSON.open(file)) {
        ofLogNotice("ofApp::setup") << inputJSON.getRawString();
        ofLogNotice("ofApp::setup") << "numAxes: " << inputJSON["numAxes"] << endl;
        ofLogNotice("ofApp::setup") << "third axis label: " << inputJSON["axesLabels"][2] << endl;
        ofLogNotice("ofApp::setup") << "second question text: " << inputJSON["questions"][1]["text"] << endl;
    } else {
        ofLogError("ofApp::setup") << "Failed to parse JSON" << endl;
    }

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofDrawBitmapString(inputJSON["questions"][1]["text"], 10, 14);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
