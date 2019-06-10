#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxBlur.h"

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
        ofImage graphData;
        ofImage faceData;
        ofTexture graph;
        ofTexture face;
        ofVideoGrabber cam;
        ofxBlur blur;
		
        int qNum;
        bool ready;
        string currentQuestionText;
        vector<string> axesLabels;
        vector<double> axesValues;
    
        void makeGraph();
        void makeFace(const vector<int>& delta);
};
