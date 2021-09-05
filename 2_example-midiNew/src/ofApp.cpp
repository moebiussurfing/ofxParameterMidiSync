
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(255, 255, 255);
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	//-
	
	params.setName("ofApp-Params");

	params.add(vecParam.set("vecParam", {0, 0, 0}, {-1, -1, -1}, {1, 1, 1}));
	params.add(param2.set("param2", 0, 0, 10));
	params.add(color.set("color", ofColor(0), ofColor(0,0), ofColor(255, 255)));
  
	paramsGroups[0].setName("0");
    paramsGroups[0].add(boton.set("boton", true));
	paramsGroups[0].add(floatColor.set("FloatColor", ofFloatColor::white, ofFloatColor(0,0), ofFloatColor::white));
	
    paramsGroups[1].setName("1");
    paramsGroups[1].add(param1.set("param1", 0, 0, 1000));
    paramsGroups[0].add(paramsGroups[1]);
    
    paramsGroups[2].setName("2");
    paramsGroups[1].add(paramsGroups[2]);
	
    params.add(paramsGroups[0]);
    
	//-

	sync.setup(params);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0);
	
	sync.drawImGui();
}

//--------------------------------------------------------------
void ofApp::exit() {
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	
	//you can use keystrokes too.
	switch(key) {
		case ' ':
            sync.learn();
            break;
        case 's':
            sync.save();
            break;
        case 'l':
            sync.load();
            break;
        case OF_KEY_BACKSPACE:
            sync.unlearn();
            break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased() {
}
