#pragma once

#include "ofMain.h"

#include "ofxParameterMidiSync.h"

#include "ofxSurfingImGui.h"
#include "ofxSurfingHelpers.h"

class ofxSurfingMidi : public ofxParameterMidiSync
{

public:

	//ofxSurfingMidi();
	//~ofxSurfingMidi();

	void setup(ofAbstractParameter & parameters) override;//default

	void setupImGui();
	void drawImGui();
	ofxSurfing_ImGui_Manager guiManager; // In MODE A ofxGui will be instatiated inside the class

	void init();
	void startup();
	
	void update(ofEventArgs& e) override;

	//void update();
	//void draw();
	//void update(ofEventArgs & args);
	//void draw(ofEventArgs & args);
	
	//void keyPressed(ofKeyEventArgs &eventArgs);
	void exit();

	void setBool(bool b);
	bool getBool();

	ofParameterGroup params_MidiPorts{ "PORTS" };
	ofParameterGroup params_AppState{ "AppState" };

private:

};

