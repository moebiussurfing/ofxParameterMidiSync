
#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxSurfingMidi.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void draw();
	void keyPressed(int key);
    
	// Parameters
    ofParameterGroup params;
    ofParameterGroup paramsGroups[3];
    ofParameter<float> floatParam1, floatParam2;
	ofParameter<int> intParam;
    ofParameter<ofColor>color;
    ofParameter<ofFloatColor>floatColor1;
    ofParameter<ofFloatColor>floatColor2;
	ofParameter<glm::vec3>vecParam;
    ofParameter<bool> bBool1;
    ofParameter<bool> bBool2;
    //ofParameter<void> bButton;
    
	ofxSurfingMidi mMidiParams;
};
