
#pragma once

#include "ofMain.h"

#include "ofxGui.h"
#include "ofxSurfingMidi.h"

class ofApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
    
	// Parameters
    ofParameterGroup params;
    ofParameterGroup paramsGroups[3];
    ofParameter<float> param1, param2;
    ofParameter<ofColor>color;
    ofParameter<ofFloatColor>floatColor;
	ofParameter<glm::vec3>vecParam;
    ofParameter<bool> boton;
    
	ofxSurfingMidi sync;
};
