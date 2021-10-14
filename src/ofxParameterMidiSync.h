//
//  ofxParameterMidiSync.h
//  emptyExample
//
//  Created by Roy Macdonald on 18-08-15.
//	Modified by moebiusSurfing: small modifications, added ImGui and settings.
//

#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxMidiRecorder.h"
#include "ofxParameterMidiInfo.h"
#include "nanoKontrolConstants.h"
#include "ofxGui.h"


class ofxParameterMidiSync:  public ofxMidiListener {
public:
    ofxParameterMidiSync();
    ~ofxParameterMidiSync();
	
	virtual void exit() {};
	virtual void setup(ofAbstractParameter & parameters);//default

    void setup(int portNum, bool bUseRecorder = true , bool bUsePlayer = true);
	void setup(int portNum, ofAbstractParameter & parameters, bool bUseRecorder = true , bool bUsePlayer = true);
    void setup(int portNum, ofParameterGroup & parameters, bool bUseRecorder = true , bool bUsePlayer = true);
    
	void setSyncGroup( ofAbstractParameter & parameters);
    void setSyncGroup( ofParameterGroup & parameters);
    void enableMidi(bool b = true);

    bool linkMidiToOfParameter(ofxMidiMessage& msg, ofAbstractParameter& param);
    
    void learn(bool bLearn = true);
    void unlearn(bool bUnlearn = true);
    
	bool load();
	void save();
	bool load(string path);
    void save(string path);
    
    void reset();
    
    void drawDebug();
	
	void setupGui(float x = 10, float y = 10);
	void setGuiPosition(float x, float y );
	void drawGui();
	
    bool isLearning(){return bLearning;}
    bool isUnlearning(){return bUnlearning;}
    
    std::shared_ptr<ofxMidiRecorder> recorder = nullptr;
    std::shared_ptr<ofxMidiPlayer> player = nullptr;
    ofParameter<float> smoothing;
	
	void enableSmoothing();
	void disableSmoothing();
	bool isSmoothingEnabled();
	
	ofParameterGroup parameters;
	
	void setFilePath(std::string path);
	std::string getFilePath();
	
	virtual void update(ofEventArgs& e);
	
	//-

	std::shared_ptr<ofxMidiIn> getMidiIn();
	std::shared_ptr<ofxMidiOut> getMidiOut();

public:
	ofParameter<int> portNum;
	ofParameter<string> portName;
	std::vector<std::string> namesPortsMidiIn;
	std::vector<std::string> namesPortsMidiOut;

	ofParameter<bool> bMidiEnabled;

	//-

	std::string path_Ports;
	std::string path_AppState;
	std::string path_Global;
	std::string path_MappingSettings;

protected:
	std::shared_ptr<ofxPanel> syncSettingsGui;
	
	void openMidi();
	void closeMidi();
	
	ofParameter<bool> bLoad, bSave, bReset;

	ofParameter<bool> bLearning, bUnlearning, bSmoothingEnabled;
	ofParameter<bool> bMidiEnabled_Settings;
	
    void newMidiMessage(ofxMidiMessage& eventArgs);

    bool linkMidiToOfParameter(ofxMidiMessage& msg, ofAbstractParameter* param);

    std::shared_ptr<ofxMidiIn> midiIn;
	std::shared_ptr<ofxMidiOut> midiOut;
    ofxMidiMessage midiMessage;

    void parameterChanged( ofAbstractParameter & parameter );
    ofParameterGroup syncGroup;
    map<int, shared_ptr<ofParameterMidiInfo> > synced;

	bool bIsSetup;
    bool bParameterGroupSetup;
    ofAbstractParameter * learningParameter;
   
    shared_ptr<ofxMidiNanoKontrolButtons> kontrolButtons;

	string strDebug;
	string strDebugPorts;

private:
	bool bMidiOpened = false;
	ofEventListener updateListener;
	ofEventListeners paramsListeners;
	
};
