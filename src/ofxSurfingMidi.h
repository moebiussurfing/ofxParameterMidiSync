#pragma once


/*

TODO:

+ add midi out for feedback mirror
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

	//void setBool(bool b);
	//bool getBool();

	ofParameterGroup params_MidiPorts{ "PORTS" };
	ofParameterGroup params_AppState{ "AppState" };


};

