//
//  ofxParameterMidiSync.cpp
//  emptyExample
//
//  Created by Roy Macdonald on 18-08-15.
//
//

#include "ofxParameterMidiSync.h"
#include "nanoKontrolConstants.h"

#include "ofxParamMidiSyncUtils.h"

//-----------------------------------------------------
ofxParameterMidiSync::ofxParameterMidiSync() :bMidiEnabled(false), portNum(-1), bLearning(false), learningParameter(nullptr), bIsSetup(false), bUnlearning(false), bParameterGroupSetup(false) {

	path_MappingSettings = "ofxParameterMidiSync_Mapping.xml";
	//path_MappingSettings = "ofxParameterMidiSyncSettings.xml";
}

//-----------------------------------------------------
ofxParameterMidiSync::~ofxParameterMidiSync() {
	ofLogNotice(__FUNCTION__);

	enableMidi(false);
	learningParameter = nullptr;

	exit();
}

//-----------------------------------------------------
std::shared_ptr<ofxMidiIn> ofxParameterMidiSync::getMidiIn() {
	if (!midiIn) {
		midiIn = std::make_shared<ofxMidiIn>();
	}
	return midiIn;
}

//-----------------------------------------------------
std::shared_ptr<ofxMidiOut> ofxParameterMidiSync::getMidiOut() {
	if (!midiOut) {
		midiOut = std::make_shared<ofxMidiOut>();
	}
	return midiOut;
}

//-----------------------------------------------------
void ofxParameterMidiSync::setup(ofAbstractParameter & parameters) {

	setup(portNum.get(), parameters, false, false);
}

//-----------------------------------------------------
void ofxParameterMidiSync::setup(int portNum, ofAbstractParameter & parameters, bool bUseRecorder, bool bUsePlayer) {
	if (ofxParamMidiSync::isParameterGroup(&parameters)) {
		setup(portNum, static_cast<ofParameterGroup&>(parameters), bUseRecorder, bUsePlayer);
	}
}

//-----------------------------------------------------
void ofxParameterMidiSync::setup(int portNum, ofParameterGroup & parameters, bool bUseRecorder, bool bUsePlayer) {
	setSyncGroup(parameters);
	setup(portNum, bUseRecorder, bUsePlayer);
}

//-----------------------------------------------------
void ofxParameterMidiSync::setup(int portNum, bool bUseRecorder, bool bUsePlayer) {
	bIsSetup = true;
	parameters.setName("ofParameterMidiSync");

	//parameters.add(bLoad.set("Load"));
	//parameters.add(bSave.set("Save"));
	//parameters.add(bReset.set("Reset"));
	parameters.add(bLoad.set("Load", false));
	parameters.add(bSave.set("Save", false));
	parameters.add(bReset.set("Reset", false));

	parameters.add(bLearning.set("Learn", false));
	parameters.add(bUnlearning.set("Unlearn", false));
	bMidiEnabled.set("MidiEnabled", false);
	//parameters.add(bMidiEnabled.set("MidiEnabled", false));
	parameters.add(bMidiEnabled_Settings.set("Enable Midi", false));
	parameters.add(bSmoothingEnabled.set("Enable Smoothing", false));
	parameters.add(smoothing.set("Smoothing", 0.5, 0, 0.95));
	parameters.add(this->portNum.set("Midi Port", portNum, 0, getMidiIn()->getNumInPorts() - 1));
	parameters.add(this->portName.set("Port Name", ""));
	//paramsListeners.push(portNum.newListener(this, &ofxParameterMidiSync::refreshPort));

	//paramsListeners.push(bLoad.newListener([&]() { load(); }));
	//paramsListeners.push(bSave.newListener([&]() { save(); }));
	//paramsListeners.push(bReset.newListener(this, &ofxParameterMidiSync::reset));

	paramsListeners.push(bLoad.newListener([&](bool &) {
		if (bLoad) {
			bLoad = false;
			load();
		}
	}));

	paramsListeners.push(bSave.newListener([&](bool &) {
		if (bSave) {
			bSave = false;
			save();
		}
	}));

	paramsListeners.push(bReset.newListener([&](bool &) {
		if (bReset) {
			bReset = false;
			reset();
		}
	}));
	//paramsListeners.push(bReset.newListener(this, &ofxParameterMidiSync::reset));

	//-

	paramsListeners.push(bLearning.newListener([&](bool &) {
		if (bLearning && bIsSetup) {
			bUnlearning = false;
		}
	}));

	paramsListeners.push(bUnlearning.newListener([&](bool &) {
		if (bUnlearning && bIsSetup) {
			if (bLearning) {
				learningParameter = nullptr;
				bLearning = false;
			}
		}
	}));

	//-

	paramsListeners.push(bMidiEnabled.newListener([&](bool &) {
		if (bMidiEnabled) {
			openMidi();
		}
		else {
			closeMidi();
		}

		////TODO:
		//if (bMidiEnabled_Settings != bMidiEnabled)
		//	bMidiEnabled_Settings = bMidiEnabled;
	}));

	paramsListeners.push(bMidiEnabled_Settings.newListener([&](bool &) {
		bMidiEnabled = bMidiEnabled_Settings;
	}));

	paramsListeners.push(bSmoothingEnabled.newListener([&](bool &) {
		if (bSmoothingEnabled) {
			updateListener = ofEvents().update.newListener(this, &ofxParameterMidiSync::update);
		}
		else {
			updateListener.unsubscribe();
		}
	}));

	paramsListeners.push(this->portNum.newListener([&](int &) {
		if (bMidiOpened) {
			//this will close the current opened midi and reopen using the new port number
			bMidiEnabled = false;
			bMidiEnabled = true;
		}
	}));

	//-

	if (bUsePlayer || bUseRecorder) {
		kontrolButtons = std::make_shared<ofxMidiNanoKontrolButtons>();
		kontrolButtons->setup(getMidiOut());
	}

	if (bUsePlayer) {
		player = std::make_shared<ofxMidiPlayer>();
		player->setup(this);
		player->kontrolButtons = kontrolButtons;
	}

	if (bUseRecorder) {
		recorder = std::make_shared<ofxMidiRecorder>();
		recorder->kontrolButtons = kontrolButtons;
	}

	enableMidi(true);
}

//-----------------------------------------------------
void ofxParameterMidiSync::update(ofEventArgs& e) {
	if (bSmoothingEnabled)
		for (map<int, shared_ptr<ofParameterMidiInfo> > ::iterator it = synced.begin(); it != synced.end(); ++it) {
			it->second->updateSmoothing(smoothing);
		}
}

//-----------------------------------------------------
void ofxParameterMidiSync::setSyncGroup(ofAbstractParameter & parameters) {
	if (ofxParamMidiSync::isParameterGroup(&parameters)) {
		setSyncGroup(static_cast<ofParameterGroup&>(parameters));
	}
}

//-----------------------------------------------------
void ofxParameterMidiSync::setSyncGroup(ofParameterGroup & parameters) {
	syncGroup = parameters;
	//	ofxParamMidiSync::printParamGroupElements(syncGroup);
	bParameterGroupSetup = true;
	//    if (bAutoLink) {
	//        synced.clear();
	//        for (int i = 0; i < parameters.size(); i++) {
	//            linkMidiToOfParameter(i, parameters.get(i));
	//        }
	//    }
}

////-----------------------------------------------------
//void ofxParameterMidiSync::refreshPort() {
//}

//-----------------------------------------------------
void ofxParameterMidiSync::reset() {
	bReset = false;

	synced.clear();
	bLearning = false;
	bUnlearning = false;
	learningParameter = nullptr;
}

//--------------------------------------------------------------
void ofxParameterMidiSync::openMidi() {
	if (bIsSetup && bParameterGroupSetup && !bMidiOpened) {
		getMidiIn()->listInPorts();
		bMidiOpened = midiIn->openPort(portNum);
		if (bMidiOpened) {
			midiIn->ignoreTypes(true, true, false);
			midiIn->addListener(this);
			ofAddListener(syncGroup.parameterChangedE(), this, &ofxParameterMidiSync::parameterChanged);

			if (recorder)midiIn->addListener(recorder.get());
			if (player)midiIn->addListener(player.get());
			getMidiOut()->openPort(portNum);

			portName = midiIn->getName();
		}
		else {
			bMidiEnabled.disableEvents();
			bMidiEnabled = false;
			bMidiEnabled.enableEvents();
		}
	}
}

//--------------------------------------------------------------
void ofxParameterMidiSync::closeMidi() {
	if (bIsSetup && bParameterGroupSetup && bMidiOpened) {
		if (midiIn) {
			midiIn->closePort();
			midiIn->removeListener(this);
			if (recorder)midiIn->removeListener(recorder.get());
			if (player)  midiIn->removeListener(player.get());
		}

		ofRemoveListener(syncGroup.parameterChangedE(), this, &ofxParameterMidiSync::parameterChanged);
		if (midiOut) {
			midiOut->closePort();
		}
		bMidiOpened = false;
	}
}

//-----------------------------------------------------
void ofxParameterMidiSync::enableMidi(bool b) {
	bMidiEnabled = b;
	//    if (bIsSetup && bParameterGroupSetup) {
//        if (b != bMidiEnabled) {
//            if (b) {
//				return openMidi();
//			}else{
//				closeMidi();
//                return true;
//            }
//            bMidiEnabled = b;
//        }
//    }
//    return false;
}

//-----------------------------------------------------
void ofxParameterMidiSync::learn(bool bLearn) {
	//    if ( bIsSetup ) {
	//        if (bLearning != bLearn) {
	bLearning = bLearn;
	//            bUnlearning = false;
	//        }
	//    }
}

//-----------------------------------------------------
void ofxParameterMidiSync::unlearn(bool bUnlearn) {
	//    if ( bIsSetup ) {
	//        if (bUnlearning != bUnlearn) {
	//            if (bLearning) {
	//                learningParameter = nullptr;
	//            }
	//            bLearning = false;
	bUnlearning = bUnlearn;
	//        }
	//    }
}

//-----------------------------------------------------
void ofxParameterMidiSync::parameterChanged(ofAbstractParameter & parameter) {
	if (bLearning) {
		learningParameter = &parameter;
	}
	else {
		for (auto&s : synced) {
			if (s.second && s.second->param->isReferenceTo(parameter)) {
				s.second->sendFeedback(midiOut);
			}
		}
	}
}

//--------------------------------------------------------------
bool ofxParameterMidiSync::linkMidiToOfParameter(ofxMidiMessage& msg, ofAbstractParameter& param) {
	return linkMidiToOfParameter(msg, &param);
}

//--------------------------------------------------------------
bool ofxParameterMidiSync::linkMidiToOfParameter(ofxMidiMessage& msg, ofAbstractParameter* param) {
	if (param == nullptr) return false;

	if (synced.count(msg.control) == 0) {
		auto s = std::make_shared<ofParameterMidiInfo>(param, msg);
		synced[msg.control] = s;
		if (s->isMultiDim()) {
			for (int i = 1; i < s->dims; i++) {
				synced[msg.control + i] = std::make_shared<ofParameterMidiInfo>(param, msg, i);
			}
		}
		return true;
	}
	return false;
}

//--------------------------------------------------------------
bool ofxParameterMidiSync::load(string path) {
	path_MappingSettings = path;
	return load();
}

//--------------------------------------------------------------
bool ofxParameterMidiSync::load() {
	ofXml xml;
	bool bLoad = false;

	ofLogNotice(__FUNCTION__) << "ofxParameterMidiSync::load(" << path_MappingSettings << ");" << endl;

	if (bIsSetup) {
		bLoad = xml.load(path_MappingSettings);
		if (bLoad) {
			auto sync = xml.getChild("ofxParameterMidiSync");
			if (!syncGroup.getName().empty()) {
				auto sg = sync.getChild(syncGroup.getName());
				if (sg) {
					sync = sg;
				}
			}
			auto PMinfo = sync.find("ofParameterMidiInfo");
			for (auto & info : PMinfo) {
				int controlNum = info.getChild("controlNum").getIntValue();
				if (synced.count(controlNum) == 0) {
					ofAbstractParameter* param = ofxParamMidiSync::findParamInGroup(syncGroup, info.getChild("groupHierarchyNames").getValue());
					if (param) {
						auto s = std::make_shared<ofParameterMidiInfo>(param);
						if (s->loadFromXml(info)) {
							synced[controlNum] = s;
						}
					}
				}
			}
		}
	}

	return bLoad;
}

//--------------------------------------------------------------
void ofxParameterMidiSync::save(string path) {
	path_MappingSettings = path;
	save();
}

//--------------------------------------------------------------
void ofxParameterMidiSync::save() {
	ofXml xml;
	auto sync = xml.appendChild("ofxParameterMidiSync");
	if (!syncGroup.getName().empty()) sync = sync.appendChild(syncGroup.getName());

	for (map<int, shared_ptr<ofParameterMidiInfo> > ::iterator i = synced.begin(); i != synced.end(); i++) {
		i->second.get()->saveToXml(sync);
	}

	xml.save(path_MappingSettings);
}

//--------------------------------------------------------------
void ofxParameterMidiSync::drawDebug() {

	string str = "Learning: " + (string)(bLearning ? "YES" : "NO") + "\n";
	str += "learningParameter: " + (string)((learningParameter == nullptr) ? "nullptr" : learningParameter->getName()) + "\n";
	str += "controlNum: " + ofToString(midiMessage.control) + "\n";
	str += "lastMidiMessage: " + midiMessage.toString() + "\n";
	//	str += "is Recording: " +(string)(?"YES":"NO");
	strDebug = str;

	ofDrawBitmapStringHighlight(strDebug, 20, ofGetHeight() - 60);
}

//--------------------------------------------------------------
void ofxParameterMidiSync::newMidiMessage(ofxMidiMessage& msg) {
	if (bIsSetup) {
		ofxMidiMessage message = msg;
		if (message.status == MIDI_CONTROL_CHANGE) {
			if (learningParameter != nullptr && bLearning) {
				if (bParameterGroupSetup) {
					if (linkMidiToOfParameter(message, learningParameter)) {
						ofLogNotice(__FUNCTION__) << "learned  " << endl;
						learningParameter = nullptr;
						bLearning = false;
					}
				}
			}
			else if (bUnlearning) {
				if (synced.count(message.control) > 0) {
					int dims = synced[message.control]->dims;
					if (dims == 0) dims = 1;
					if (dims > 4) dims = 4;
					for (int i = 0; i < dims; i++) {
						if (synced.count(message.control + i)) {
							synced.erase(message.control + i);
						}
					}
					bUnlearning = false;
					ofLogNotice(__FUNCTION__) << "unlearned  " << endl;
				}
			}
			else {
				//                if (msg.control == NANO_KONTROL_KEY_FFW) {
				////                    if (msg.value == 127) {
				////                        ofNotifyEvent(ffwKeyPressed, this);
				////                    }
				//                }else if(msg.control == NANO_KONTROL_KEY_REW)  {
				//                }else if(msg.control == NANO_KONTROL_KEY_STOP) {
				//                }else if(msg.control == NANO_KONTROL_KEY_PLAY) {
				//                }else if(msg.control == NANO_KONTROL_KEY_REC)  {
				//				
				if (synced.count(message.control)) {
					synced[message.control]->setNewValue(message.value, bSmoothingEnabled);
					//					synced[message.control]->sendFeedback(midiOut);

				}
			}
		}
		midiMessage = message;
	}
}

//--------------------------------------------------------------
void ofxParameterMidiSync::enableSmoothing() {
	bSmoothingEnabled = true;
}

//--------------------------------------------------------------
void ofxParameterMidiSync::disableSmoothing() {
	bSmoothingEnabled = false;
}

//--------------------------------------------------------------
bool ofxParameterMidiSync::isSmoothingEnabled() {
	return bSmoothingEnabled;
}

//--------------------------------------------------------------
void ofxParameterMidiSync::setupGui(float x, float y) {
	syncSettingsGui = std::make_shared<ofxPanel>();
	syncSettingsGui->setup("syncSettingsGui");
	syncSettingsGui->setPosition(x, y);
	syncSettingsGui->add(parameters);
}

//--------------------------------------------------------------
void ofxParameterMidiSync::setGuiPosition(float x, float y) {
	if (syncSettingsGui)syncSettingsGui->setPosition(x, y);
}

//--------------------------------------------------------------
void ofxParameterMidiSync::drawGui() {
	if (syncSettingsGui) {
		syncSettingsGui->draw();

		string str = "Learning: " + (string)(bLearning ? "YES" : "NO") + "\n";
		str += "learningParameter: " + (string)((learningParameter == nullptr) ? "nullptr" : learningParameter->getName()) + "\n";
		str += "controlNum: " + ofToString(midiMessage.control) + "\n";
		str += "lastMidiMessage: " + midiMessage.toString() + "\n";
		//	str += "is Recording: " +(string)(?"YES":"NO");
		strDebug = str;
	}
}

//--------------------------------------------------------------
void ofxParameterMidiSync::setFilePath(std::string path) {
	path_MappingSettings = path;
}

//--------------------------------------------------------------
std::string ofxParameterMidiSync::getFilePath() {
	return path_MappingSettings;
}
