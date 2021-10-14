
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	params.setName("ofApp-Params");

	params.add(vecParam.set("vecParam", {0, 0, 0}, {-1, -1, -1}, {1, 1, 1}));
	params.add(intParam.set("intParam", 0, 0, 10));
	params.add(color.set("color", ofColor(0), ofColor(0,0), ofColor(255, 255)));
  
	paramsGroups[0].setName("Group 0");
    paramsGroups[0].add(bBool1.set("bBool1", true));
    paramsGroups[0].add(bBool2.set("bBool2", true));
	paramsGroups[0].add(floatColor1.set("floatColor1", ofFloatColor::orange, ofFloatColor(0,0), ofFloatColor::white));
	
    paramsGroups[1].setName("Group 1");
    paramsGroups[1].add(floatParam1.set("floatParam1", 0, 0, 1000));
	paramsGroups[1].add(floatParam2.set("floatParam2", 0, 0, 10));
	paramsGroups[1].add(floatColor2.set("floatColor2", ofFloatColor::green, ofFloatColor(0,0), ofFloatColor::white));
    paramsGroups[0].add(paramsGroups[1]);
    
    paramsGroups[2].setName("Group 2");
    paramsGroups[2].add(bBool2);
    paramsGroups[2].add(floatColor1);
    paramsGroups[2].add(floatColor2);
    paramsGroups[1].add(paramsGroups[2]);
	
    params.add(paramsGroups[0]);
    
	//-

	mMidiParams.setup(params);
}

//--------------------------------------------------------------
void ofApp::draw() {
	
	mMidiParams.drawImGui();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	switch(key) {
		case ' ':
            mMidiParams.learn();
            break;
        case 's':
            mMidiParams.save();
            break;
        case 'l':
            mMidiParams.load();
            break;
        case OF_KEY_BACKSPACE:
            mMidiParams.unlearn();
            break;
	}
}
