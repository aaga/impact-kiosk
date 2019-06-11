#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxBlur.h"
#include "ofxTextInputField.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyReleased(int key);
    
        void handleYesNo(bool yes);
    
    private:
        ofxJSONElement inputJSON;
    
        // OpenGL can only run on the main thread,
        // so we need to separate pixel data from render
        ofImage graphData, faceData;
        ofTexture graph, face;
        ofVideoGrabber cam;
        ofxBlur blur;
        ofTrueTypeFont large_font, medium_font, small_font;
        ofxTextInputField nameInput;
		
        string userName, currentQuestionText, answer;
        int qNum;
        bool ready, newImageAvailable;
        vector<string> axesLabels;
        vector<double> axesValues;
    
        void logInfo();
        void drawLogin();
        void saveImage();
        void makeGraph();
        void makeFace(const vector<int>& delta);
};
