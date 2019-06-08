#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    qNum = 0;
    std::string file = "questions.json";
    
    if (inputJSON.open(file)) {
        currentQuestionText = inputJSON["questions"][qNum]["text"].asString();
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

}

void ofApp::handleYesNo(bool yes){
    qNum++;
    if (qNum == 2) qNum = 0;
    currentQuestionText = inputJSON["questions"][qNum]["text"].asString();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
