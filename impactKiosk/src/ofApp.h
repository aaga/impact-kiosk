#pragma once

#include "ofMain.h"
#include "ofxJSON.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyReleased(int key);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void handleYesNo(bool yes);
    
    private:
        ofxJSONElement inputJSON;
    
        string userName;
        ofImage graph;
		
        int qNum;
        string currentQuestionText;
        std::vector<string> axesLabels;
        std::vector<double> axesValues;
        void makeGraph();
};
