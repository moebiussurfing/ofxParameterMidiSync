#include "ofxSurfingMidi.h"

////--------------------------------------------------------------
//ofxSurfingMidi::ofxSurfingMidi() {
//	//ofAddListener(ofEvents().update, this, &ofxSurfingMidi::update);
//	//ofAddListener(ofEvents().draw, this, &ofxSurfingMidi::draw);
//	//ofAddListener(ofEvents().keyPressed, this, &ofxSurfingMidi::keyPressed);
//}

//--------------------------------------------------------------
ofxSurfingMidi::~ofxSurfingMidi() {
	ofLogNotice(__FUNCTION__);

	//ofxParameterMidiSync::~ofxParameterMidiSync();
	enableMidi(false);
	learningParameter = nullptr;

	exit();

	//ofRemoveListener(ofEvents().update, this, &ofxSurfingMidi::update);
	//ofRemoveListener(ofEvents().draw, this, &ofxSurfingMidi::draw);
	//ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingMidi::keyPressed);
}

//-----------------------------------------------------
void ofxSurfingMidi::setup(ofAbstractParameter & parameters) {
	init();

	ofxParameterMidiSync::setup(portNum.get(), parameters, false, false);

	startup();
}

//-----------------------------------------------------
void ofxSurfingMidi::reset(ofAbstractParameter & parameters) {
	ofxParameterMidiSync::reset();
	syncGroup.clear();
	setSyncGroup(parameters);
}

//--------------------------------------------------------------
void ofxSurfingMidi::init() {

	ofLogNotice(__FUNCTION__);

	path_Global = "ofxSurfingMidi/";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_Ports = path_Global + "Midi-Ports.xml";
	path_AppState = path_Global + "AppState.xml";

	//setFilePath(path_Global + "Midi-Mapping.xml");

	setupImGui();

	//ofAddListener(ofEvents().update, this, &ofxSurfingMidi::update);
}

//--------------------------------------------------------------
void ofxSurfingMidi::startup() {

	ofLogNotice(__FUNCTION__);

	// Settings

	// Midi Ports
	params_MidiPorts.add(portNum);
	params_MidiPorts.add(portName);

	//-

	bMidiEnabled_Settings.set("Enable", true);
	
	//TODO:
	bMidiEnabled_Settings.makeReferenceTo(bMidiEnabled);

	// App
	bGui.set("MIDI", true);
	bGui_User.set("MIDI Control", true);
	bGui_Params.set("MIDI Params", false);

	params_AppState.add(bGui);
	params_AppState.add(bGui_User);
	params_AppState.add(bGui_Params);

	params_AppState.add(bMidiEnabled_Settings);
	params_AppState.add(bSmoothingEnabled);
	params_AppState.add(smoothing);

	// Load
	ofxSurfingHelpers::loadGroup(params_MidiPorts, path_Ports);
	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	setFilePath(path_Global + "Midi-Mapping_" + syncGroup.getName() + ".xml");

	ofxParameterMidiSync::load();

	//-

	{
		auto ss = midiIn->getInPortList();
		strDebugPorts = "In Ports:\n";
		for (unsigned int i = 0; i < ss.size(); ++i) {
			strDebugPorts += ofToString(i) + ": " + ss[i] + "\n";
		}
		strDebugPorts += "\n";
	}
}

//--------------------------------------------------------------
void ofxSurfingMidi::exit() {
	ofLogNotice(__FUNCTION__);

	ofxSurfingHelpers::saveGroup(params_MidiPorts, path_Ports);
	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);

	ofxParameterMidiSync::save();
}

//--------------------------------------------------------------
void ofxSurfingMidi::setupImGui()
{
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);
}

//--------------------------------------------------------------
void ofxSurfingMidi::drawImGui()
{
	if (!bGui) return;

	// 1. Control

	guiManager.begin();
	{
		if (bGui_User)
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
			if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

			guiManager.beginWindow(bGui_User, window_flags);
			{
				guiManager.Add(guiManager.bMinimize, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);

				guiManager.Add(bMidiEnabled_Settings, OFX_IM_TOGGLE_BIG);
				if (bMidiEnabled_Settings)
				{
					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5); // fix oversizes
					{
						guiManager.Add(portNum);
						////guiManager.Add(portName, OFX_IM_TEXT_DISPLAY);

						ImGui::PushID("##port");
						ofxImGuiSurfing::AddCombo(portNum, namesPortsMidiIn);
						ImGui::PopItemWidth();
					}
					ImGui::PopID();
				}

				ImGui::Spacing();

				{
					bool bOpen = false;
					ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
					if (ImGui::CollapsingHeader("Mapping", _flagw))
					{
						guiManager.refreshLayout();

						if (bLearning)guiManager.Add(bLearning, OFX_IM_TOGGLE_BIG_BORDER_BLINK, 2, true);
						else guiManager.Add(bLearning, OFX_IM_TOGGLE_BIG, 2, true);

						if (bUnlearning)guiManager.Add(bUnlearning, OFX_IM_TOGGLE_BIG_BORDER_BLINK, 2);
						else guiManager.Add(bUnlearning, OFX_IM_TOGGLE_BIG, 2);

						if (!guiManager.bMinimize)
						{
							guiManager.Add(bLoad, OFX_IM_BUTTON_SMALL, 2, true);
							guiManager.Add(bSave, OFX_IM_BUTTON_SMALL, 2);
							guiManager.Add(bReset, OFX_IM_BUTTON_SMALL);

							ImGui::Spacing();

							guiManager.Add(bSmoothingEnabled, OFX_IM_TOGGLE_BUTTON_ROUNDED_SMALL);
							if (bSmoothingEnabled)
							{
								guiManager.Add(smoothing, OFX_IM_DEFAULT, 2);
							}
						}
					}
				}

				//-

				// Log Debug

				if (!guiManager.bMinimize)
				{
					bool bOpen = false;
					ImGuiColorEditFlags _flagw = (bOpen ? ImGuiWindowFlags_NoCollapse : ImGuiWindowFlags_None);
					if (ImGui::CollapsingHeader("Log", _flagw))
					{
						ImGui::Indent();
						std::string str;
						str += "Learning: " + (std::string)(bLearning ? "YES" : "NO") + "\n";
						str += "Parameter: " + (std::string)((learningParameter == nullptr) ? "nullptr" : learningParameter->getName()) + "\n";
						str += "CC: " + ofToString(midiMessage.control) + "\n";
						str += "Message: \n" + midiMessage.toString() + "\n";
						//	str += "is Recording: " +(string)(?"YES":"NO");
						strDebug = strDebugPorts + str;
						ImGui::TextWrapped(strDebug.c_str());
						ImGui::Unindent();
					}
				}

				//--

				if (!guiManager.bMinimize)
				{
					ofxImGuiSurfing::AddToggleRoundedButton(bGui_Params);
				}
			}
			guiManager.endWindow();
		}

		//---

		// 2. Parameters

		if (!guiManager.bMinimize)
			if (bGui_Params)
				if (bParameterGroupSetup)
				{
					ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
					if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

					guiManager.beginWindow(bGui_Params, window_flags);
					{
						guiManager.AddGroup(syncGroup);

						//ofxParamMidiSync::printParamGroupElements(syncGroup);

						ofxImGuiSurfing::AddToggleRoundedButton(bGui_User);
					}
					guiManager.endWindow();
				}
	}
	guiManager.end();
}