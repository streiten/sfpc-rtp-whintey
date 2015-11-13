#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxSvg.h"
#include "ofxAnimatableFloat.h"


class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
        void initControls();
        ofPoint calcPoint(float amp, float angle, float phase);
        void drawWave();
        float drawLissaous();
    
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ofParameterGroup paramsGroup1;
        ofParameterGroup paramsGroup2;
    
        // Parameters for the two Waves
        ofParameter<bool> pShow[2];
        ofParameter<float> pAmp[2];
        ofParameter<float> pFreq[2];
        ofParameter<float> pPhase[2];
        ofParameter<int> pDensity[2];
        ofParameter<int> pImgScale[2];
    
        //ofParameter<bool> pJitterScale[2];

        ofParameter<int> pAnimSpeed[2];

        // General Purpose Strings
        ofParameter<int> plissajouRatio[2];
        ofParameter<string> pparam1;
        ofParameter<string> pparam2;
        ofParameter<string> pparam3;
    
        //8 bits red, 8 bits green, 8 bits blue, from 0 to 255 in 256 steps
        ofFbo rgbaFbo; // with alpha
    
        ofParameter<bool> pRec;
        bool recLoopDone;
        float lastPhase;
        void saveFrame();
    
        ofImage bckgImg, triImg, eyesImg;
        float imgScales[2];
    
        ofxPanel gui;
    
        ofxAnimatableFloat  anim[2];
        ofPoint center;

};
