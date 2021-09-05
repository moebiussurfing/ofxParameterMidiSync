#include "ofxSurfingMidi.h"
////--------------------------------------------------------------
//ofxSurfingMidi::ofxSurfingMidi() {
//	//ofAddListener(ofEvents().update, this, &ofxSurfingMidi::update);
//	//ofAddListener(ofEvents().draw, this, &ofxSurfingMidi::draw);
//	//ofAddListener(ofEvents().keyPressed, this, &ofxSurfingMidi::keyPressed);
//}
//
////--------------------------------------------------------------
//ofxSurfingMidi::~ofxSurfingMidi() {
//	//ofRemoveListener(ofEvents().update, this, &ofxSurfingMidi::update);
//	//ofRemoveListener(ofEvents().draw, this, &ofxSurfingMidi::draw);
//	//ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingMidi::keyPressed);
//}

//-----------------------------------------------------
void ofxSurfingMidi::setup(ofAbstractParameter & parameters) {
	init();
	ofxParameterMidiSync::setup(portNum.get(), parameters, false, false);

	//startup();
}

//--------------------------------------------------------------
void ofxSurfingMidi::init() {
	path_Global = "ofxSurfingMidi/";
	ofxSurfingHelpers::CheckFolder(path_Global);

	path_Ports = path_Global + "Midi-Ports.xml";
	path_AppState = path_Global + "AppState.xml";

	setupImGui();

	ofAddListener(ofEvents().update, this, &ofxSurfingMidi::update);
}

//--------------------------------------------------------------
void ofxSurfingMidi::startup() {

	ofLogNotice(__FUNCTION__);

	//settings

	params_MidiPorts.add(portNum);
	params_MidiPorts.add(portName);

	params_AppState.add(bMidiEnabled);
	params_AppState.add(bSmoothingEnabled);

	ofxSurfingHelpers::loadGroup(params_MidiPorts, path_Ports);
	ofxSurfingHelpers::loadGroup(params_AppState, path_AppState);

	//-

	//styles
	guiManager.clearStyles();
	guiManager.AddStyle(bLoad, OFX_IM_BUTTON_BIG);
	guiManager.AddStyle(bSave, OFX_IM_BUTTON_BIG);
	guiManager.AddStyle(bReset, OFX_IM_BUTTON_SMALL);
	guiManager.AddStyle(bLearning, OFX_IM_TOGGLE_BIG);
	guiManager.AddStyle(bUnlearning, OFX_IM_BUTTON_SMALL);
	guiManager.AddStyle(bMidiEnabled, OFX_IM_TOGGLE_BIG);
	guiManager.AddStyle(bSmoothingEnabled, OFX_IM_TOGGLE_SMALL);
	guiManager.AddStyle(smoothing, OFX_IM_DEFAULT);
}

//-----------------------------------------------------
void ofxSurfingMidi::update(ofEventArgs& e) {
	if (ofGetFrameNum() == 1) startup();

	//ofLogNotice(__FUNCTION__);

	for (map<int, shared_ptr<ofParameterMidiInfo> > ::iterator it = synced.begin(); it != synced.end(); ++it) {
		it->second->updateSmoothing(smoothing);
	}
}

////--------------------------------------------------------------
//void ofxSurfingMidi::update(){
//}
//
////--------------------------------------------------------------
//void ofxSurfingMidi::update(ofEventArgs & args)
//{
//}
//
////--------------------------------------------------------------
//void ofxSurfingMidi::draw(){
//}
////--------------------------------------------------------------
//void ofxSurfingMidi::keyPressed(ofKeyEventArgs &eventArgs)
//{
//	const int key = eventArgs.key;
//
//	// modifiers
//	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
//	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
//	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
//	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);
//
//	ofLogNotice(__FUNCTION__) << " : " << key;
//
//	//--
//
//	if (false){}
//
//	else if (key == 'G' && !mod_ALT)
//	{
//	}
//}

//--------------------------------------------------------------
void ofxSurfingMidi::exit() {
	ofxSurfingHelpers::saveGroup(params_MidiPorts, path_Ports);
	ofxSurfingHelpers::saveGroup(params_AppState, path_AppState);
}

//--------------------------------------------------------------
void ofxSurfingMidi::setBool(bool b) {
	ofLogNotice(__FUNCTION__) << ofToString(b ? "true" : "false");
}

//--------------------------------------------------------------
bool ofxSurfingMidi::getBool() {
	return true;
}

//--------------------------------------------------------------
void ofxSurfingMidi::setupImGui()
{
	guiManager.setup(IM_GUI_MODE_INSTANTIATED);
}

//--------------------------------------------------------------
void ofxSurfingMidi::drawImGui()
{
	guiManager.begin(); // global begin
	{
		{
			string n = "ofxSurfingMidi";
			static bool bOpen0 = true;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
			if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

			guiManager.beginWindow(n.c_str(), &bOpen0, window_flags);
			{
				//ImGui::Button("Button");

				guiManager.AddGroup(parameters);

				ImGui::Spacing();
				ImGui::TextWrapped(strDebug.c_str());

				//				float _h = WIDGETS_HEIGHT;
				//				float _w100 = getWidgetsWidth(1);
				//				float _w50 = getWidgetsWidth(2);
				//
				//				ImGui::Text("MIDI INPUT");
				//
				//				ofxImGuiSurfing::AddBigToggle(bShowMapping, _w100, _h / 2, false);
				//				ofxImGuiSurfing::AddBigButton(bLoad, _w50, _h / 2);
				//				ImGui::SameLine();
				//				ofxImGuiSurfing::AddBigButton(bSave, _w50, _h / 2);
				//
				//				//-
				//
				//				bool bOpen = false;
				//				ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				//				_flagt |= ImGuiTreeNodeFlags_Framed;
				//
				//				if (ImGui::TreeNodeEx("MIDI", _flagt))
				//				{
				//					{
				//						bool bOpen = true;
				//						ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				//						_flagt |= ImGuiTreeNodeFlags_Framed;
				//
				//						if (ImGui::TreeNodeEx("CONNECT", _flagt))
				//						{
				//							_w100 = getWidgetsWidth(1);
				//							_w50 = getWidgetsWidth(2);
				//
				//							string ni = "INPUT:  " + midiIn_Port_name.get();
				//							ImGui::Text(ni.c_str());
				//							if (ImGui::Button("ON", ImVec2(_w50, _h / 2)))
				//							{
				//								midiIn_Port = midiIn_Port;
				//
				//								//midiIn.openPort(midiIn_Port);
				//								//midiIn_Port_name = midiIn.getName();
				//							}
				//							ImGui::SameLine();
				//							if (ImGui::Button("OFF", ImVec2(_w50, _h / 2)))
				//							{
				//								disconnect();
				//								midiIn_Port_name = "";
				//							}
				//
				//							//-
				//
				//							string no = "OUTPUT: " + midiOut_Port_name.get();
				//							ImGui::Text(no.c_str());
				//							if (ImGui::Button("ON ", ImVec2(_w50, _h / 2)))
				//							{
				//								midiOut_Port = midiOut_Port;
				//
				//								//midiOut.openPort(midiOut_Port);
				//								//midiOut_Port_name = midiOut.getName();
				//							}
				//							ImGui::SameLine();
				//							if (ImGui::Button("OFF ", ImVec2(_w50, _h / 2)))
				//							{
				//								midiOut.closePort();
				//								midiOut_Port_name = "";
				//							}
				//							ImGui::TreePop();
				//						}
				//					}
				//
				//					//-
				//
				//					{
				//						bool bOpen = false;
				//						ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				//						_flagt |= ImGuiTreeNodeFlags_Framed;
				//
				//						ofxImGuiSurfing::AddGroup(params_MidiPorts, _flagt);
				//					}
				//
				//					ImGui::TreePop();
				//				}
				//
				//				//-
				//
				//				ImGui::Dummy(ImVec2(0, 5)); // spacing
				//
				//				static bool bLockMappgingPanel = true;
				//
				//				ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bExtra);
				//				if (guiManager.bExtra)
				//				{
				//					ImGui::Indent();
				//
				//					_w100 = getWidgetsWidth(1);
				//					_w50 = getWidgetsWidth(2);
				//
				//					ofxImGuiSurfing::ToggleRoundedButton("Lock Mapping Panel", &bLockMappgingPanel);
				//					ofxImGuiSurfing::AddToggleRoundedButton(bAutoReconnect);
				//					//ofxImGuiSurfing::AddBigButton(bPopulate, _w100, _h / 2);
				//					//static bool bClear = false;
				//					//ofxImGuiSurfing::ToggleRoundedButton("Clear", &bClear);
				//					//{
				//					//	if (bClear) bClear = false;
				//					//	clear();
				//					//}
				//					ofxImGuiSurfing::AddToggleRoundedButton(bAutoSave);
				//
				//					bool bOpen = false;
				//					ImGuiTreeNodeFlags _flagt = (bOpen ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
				//					_flagt |= ImGuiTreeNodeFlags_Framed;
				//					if (ImGui::TreeNodeEx("PATH SETTINGS", _flagt))
				//					{
				//						ImGui::Indent();
				//						ImGui::Text(path_Global.c_str());
				//						ImGui::Text(filenameSettings.c_str());
				//						ImGui::Unindent();
				//						ImGui::TreePop();
				//					}
				//
				//					ImGui::Dummy(ImVec2(0, 5)); // spacing
				//
				//					//-
				//
				//#ifdef USE_MIDI_PARAMS__VIDEO_SKIP
				//					static bool bPopulateMidiToggles = false;
				//					if (ofxImGuiSurfing::ToggleRoundedButton("Populate Midi", &bPopulateMidiToggles))
				//					{
				//						mMidiParams.add(presets.getParametersSelectorToggles());
				//					}
				//#endif
				//
				//					//--
				//
				//					/*
				//					ofxImGuiSurfing::AddToggleRoundedButton(guiManager.bAdvanced);
				//					if (guiManager.bExtra) guiManager.drawAdvancedSubPanel();
				//					*/
				//
				//					ImGui::Unindent();
				//				}
				//
				//				//----
				//
				//				// get window position for advanced layout linked position
				//				if (bLockMappgingPanel)
				//				{
				//					float pad = PADDING_PANELS;
				//					auto posx = ImGui::GetWindowPos().x;
				//					auto posy = ImGui::GetWindowPos().y;
				//					float __w = ImGui::GetWindowWidth();
				//					float __h = ImGui::GetWindowHeight();
				//					pos.x = posx + __w + pad;
				//					pos.y = posy;
				//				}
			}
			guiManager.endWindow();
		}

		//-

		if (bParameterGroupSetup)
		{
			string n = "Parameters";
			static bool bOpen0 = true;
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
			if (guiManager.bAutoResize) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

			guiManager.beginWindow(n.c_str(), &bOpen0, window_flags);
			{
				guiManager.AddGroup(syncGroup);
				//ofxParamMidiSync::printParamGroupElements(syncGroup);
				guiManager.endWindow();
			}
		}
	}
	guiManager.end(); // global end
}