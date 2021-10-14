#pragma once


/*

TODO:

+ bool/buttons to midi notes not working..
+ 

+ add dropdown combo for port selector
+ add midi out for feedback mirror or check bSendFeedback
+ add groups on runtime

*/


#include "ofMain.h"

#include "ofxParameterMidiSync.h"

#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"

class ofxSurfingMidi : public ofxParameterMidiSync
{

public:
	//ofxSurfingMidi();
	~ofxSurfingMidi();

private:
	void init();
	void startup();

public:
	void setup(ofAbstractParameter & parameters) override;
	void reset(ofAbstractParameter & parameters);
	void drawImGui();

private:
	void exit() override;

	void setupImGui();

	ofxSurfing_ImGui_Manager guiManager;
	
	//void update(ofEventArgs& e) override;
	//void update();
	//void draw();
	//void update(ofEventArgs & args);
	//void draw(ofEventArgs & args);
	//void keyPressed(ofKeyEventArgs &eventArgs);

	ofParameterGroup params_MidiPorts{ "PORTS" };
	ofParameterGroup params_AppState{ "AppState" };

public:
	ofParameter<bool> bGui;
	ofParameter<bool> bGui_User;
	ofParameter<bool> bGui_Params;

};

