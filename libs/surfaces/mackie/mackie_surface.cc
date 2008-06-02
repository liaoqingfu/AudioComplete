/*
	Generated by scripts/generate-surface.rb
*/

#include "mackie_surface.h"

#include "controls.h"
#include "mackie_button_handler.h"

using namespace Mackie;

void Mackie::MackieSurface::init_controls()
{
	// intialise groups and strips
	Group * group = 0;
	
	// make sure there are enough strips
	strips.resize( 8 );
	
	group = new Group ( "user" );
	groups["user"] = group;
	
	group = new Group ( "assignment" );
	groups["assignment"] = group;
	
	group = new Group ( "none" );
	groups["none"] = group;
	
	group = new MasterStrip ( "master", 0 );
	groups["master"] = group;
	strips[0] = dynamic_cast<Strip*>( group );
	
	group = new Strip ( "strip_1", 0 );
	groups["strip_1"] = group;
	strips[0] = dynamic_cast<Strip*>( group );
	
	group = new Group ( "cursor" );
	groups["cursor"] = group;
	
	group = new Strip ( "strip_2", 1 );
	groups["strip_2"] = group;
	strips[1] = dynamic_cast<Strip*>( group );
	
	group = new Group ( "functions" );
	groups["functions"] = group;
	
	group = new Group ( "automation" );
	groups["automation"] = group;
	
	group = new Strip ( "strip_3", 2 );
	groups["strip_3"] = group;
	strips[2] = dynamic_cast<Strip*>( group );
	
	group = new Group ( "display" );
	groups["display"] = group;
	
	group = new Strip ( "strip_4", 3 );
	groups["strip_4"] = group;
	strips[3] = dynamic_cast<Strip*>( group );
	
	group = new Strip ( "strip_5", 4 );
	groups["strip_5"] = group;
	strips[4] = dynamic_cast<Strip*>( group );
	
	group = new Strip ( "strip_6", 5 );
	groups["strip_6"] = group;
	strips[5] = dynamic_cast<Strip*>( group );
	
	group = new Group ( "transport" );
	groups["transport"] = group;
	
	group = new Strip ( "strip_7", 6 );
	groups["strip_7"] = group;
	strips[6] = dynamic_cast<Strip*>( group );
	
	group = new Group ( "modifiers" );
	groups["modifiers"] = group;
	
	group = new Group ( "bank" );
	groups["bank"] = group;
	
	group = new Strip ( "strip_8", 7 );
	groups["strip_8"] = group;
	strips[7] = dynamic_cast<Strip*>( group );
	

	// initialise controls
	Control * control = 0;

	group = groups["strip_1"];
	control = new Fader ( 0, 1, "gain", *group );
	faders[0x00] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Fader ( 1, 2, "gain", *group );
	faders[0x01] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Fader ( 2, 3, "gain", *group );
	faders[0x02] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Fader ( 3, 4, "gain", *group );
	faders[0x03] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Fader ( 4, 5, "gain", *group );
	faders[0x04] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Fader ( 5, 6, "gain", *group );
	faders[0x05] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Fader ( 6, 7, "gain", *group );
	faders[0x06] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Fader ( 7, 8, "gain", *group );
	faders[0x07] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["master"];
	control = new Fader ( 8, 1, "gain", *group );
	faders[0x08] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_1"];
	control = new Pot ( 16, 1, "vpot", *group );
	pots[0x10] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Pot ( 17, 2, "vpot", *group );
	pots[0x11] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Pot ( 18, 3, "vpot", *group );
	pots[0x12] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Pot ( 19, 4, "vpot", *group );
	pots[0x13] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Pot ( 20, 5, "vpot", *group );
	pots[0x14] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Pot ( 21, 6, "vpot", *group );
	pots[0x15] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Pot ( 22, 7, "vpot", *group );
	pots[0x16] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Pot ( 23, 8, "vpot", *group );
	pots[0x17] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["none"];
	control = new Pot ( 60, 1, "jog", *group );
	pots[0x3c] = control;
	controls.push_back( control );
	controls_by_name["jog"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Pot ( 46, 1, "external", *group );
	pots[0x2e] = control;
	controls.push_back( control );
	controls_by_name["external"] = control;
	group->add( *control );

	group = groups["strip_1"];
	control = new Button ( 0, 1, "recenable", *group );
	buttons[0x00] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Button ( 1, 2, "recenable", *group );
	buttons[0x01] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Button ( 2, 3, "recenable", *group );
	buttons[0x02] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Button ( 3, 4, "recenable", *group );
	buttons[0x03] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Button ( 4, 5, "recenable", *group );
	buttons[0x04] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Button ( 5, 6, "recenable", *group );
	buttons[0x05] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Button ( 6, 7, "recenable", *group );
	buttons[0x06] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Button ( 7, 8, "recenable", *group );
	buttons[0x07] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_1"];
	control = new Button ( 8, 1, "solo", *group );
	buttons[0x08] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Button ( 9, 2, "solo", *group );
	buttons[0x09] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Button ( 10, 3, "solo", *group );
	buttons[0x0a] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Button ( 11, 4, "solo", *group );
	buttons[0x0b] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Button ( 12, 5, "solo", *group );
	buttons[0x0c] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Button ( 13, 6, "solo", *group );
	buttons[0x0d] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Button ( 14, 7, "solo", *group );
	buttons[0x0e] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Button ( 15, 8, "solo", *group );
	buttons[0x0f] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_1"];
	control = new Button ( 16, 1, "mute", *group );
	buttons[0x10] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Button ( 17, 2, "mute", *group );
	buttons[0x11] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Button ( 18, 3, "mute", *group );
	buttons[0x12] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Button ( 19, 4, "mute", *group );
	buttons[0x13] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Button ( 20, 5, "mute", *group );
	buttons[0x14] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Button ( 21, 6, "mute", *group );
	buttons[0x15] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Button ( 22, 7, "mute", *group );
	buttons[0x16] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Button ( 23, 8, "mute", *group );
	buttons[0x17] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_1"];
	control = new Button ( 24, 1, "select", *group );
	buttons[0x18] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Button ( 25, 2, "select", *group );
	buttons[0x19] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Button ( 26, 3, "select", *group );
	buttons[0x1a] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Button ( 27, 4, "select", *group );
	buttons[0x1b] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Button ( 28, 5, "select", *group );
	buttons[0x1c] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Button ( 29, 6, "select", *group );
	buttons[0x1d] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Button ( 30, 7, "select", *group );
	buttons[0x1e] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Button ( 31, 8, "select", *group );
	buttons[0x1f] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_1"];
	control = new Button ( 32, 1, "vselect", *group );
	buttons[0x20] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Button ( 33, 2, "vselect", *group );
	buttons[0x21] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Button ( 34, 3, "vselect", *group );
	buttons[0x22] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Button ( 35, 4, "vselect", *group );
	buttons[0x23] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Button ( 36, 5, "vselect", *group );
	buttons[0x24] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Button ( 37, 6, "vselect", *group );
	buttons[0x25] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Button ( 38, 7, "vselect", *group );
	buttons[0x26] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Button ( 39, 8, "vselect", *group );
	buttons[0x27] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["assignment"];
	control = new Button ( 40, 1, "io", *group );
	buttons[0x28] = control;
	controls.push_back( control );
	controls_by_name["io"] = control;
	group->add( *control );

	group = groups["assignment"];
	control = new Button ( 41, 1, "sends", *group );
	buttons[0x29] = control;
	controls.push_back( control );
	controls_by_name["sends"] = control;
	group->add( *control );

	group = groups["assignment"];
	control = new Button ( 42, 1, "pan", *group );
	buttons[0x2a] = control;
	controls.push_back( control );
	controls_by_name["pan"] = control;
	group->add( *control );

	group = groups["assignment"];
	control = new Button ( 43, 1, "plugin", *group );
	buttons[0x2b] = control;
	controls.push_back( control );
	controls_by_name["plugin"] = control;
	group->add( *control );

	group = groups["assignment"];
	control = new Button ( 44, 1, "eq", *group );
	buttons[0x2c] = control;
	controls.push_back( control );
	controls_by_name["eq"] = control;
	group->add( *control );

	group = groups["assignment"];
	control = new Button ( 45, 1, "dyn", *group );
	buttons[0x2d] = control;
	controls.push_back( control );
	controls_by_name["dyn"] = control;
	group->add( *control );

	group = groups["bank"];
	control = new Button ( 46, 1, "left", *group );
	buttons[0x2e] = control;
	controls.push_back( control );
	controls_by_name["left"] = control;
	group->add( *control );

	group = groups["bank"];
	control = new Button ( 47, 1, "right", *group );
	buttons[0x2f] = control;
	controls.push_back( control );
	controls_by_name["right"] = control;
	group->add( *control );

	group = groups["bank"];
	control = new Button ( 48, 1, "channel_left", *group );
	buttons[0x30] = control;
	controls.push_back( control );
	controls_by_name["channel_left"] = control;
	group->add( *control );

	group = groups["bank"];
	control = new Button ( 49, 1, "channel_right", *group );
	buttons[0x31] = control;
	controls.push_back( control );
	controls_by_name["channel_right"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 50, 1, "flip", *group );
	buttons[0x32] = control;
	controls.push_back( control );
	controls_by_name["flip"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 51, 1, "edit", *group );
	buttons[0x33] = control;
	controls.push_back( control );
	controls_by_name["edit"] = control;
	group->add( *control );

	group = groups["display"];
	control = new Button ( 52, 1, "name_value", *group );
	buttons[0x34] = control;
	controls.push_back( control );
	controls_by_name["name_value"] = control;
	group->add( *control );

	group = groups["display"];
	control = new Button ( 53, 1, "smpte_beats", *group );
	buttons[0x35] = control;
	controls.push_back( control );
	controls_by_name["smpte_beats"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 54, 1, "F1", *group );
	buttons[0x36] = control;
	controls.push_back( control );
	controls_by_name["F1"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 55, 1, "F2", *group );
	buttons[0x37] = control;
	controls.push_back( control );
	controls_by_name["F2"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 56, 1, "F3", *group );
	buttons[0x38] = control;
	controls.push_back( control );
	controls_by_name["F3"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 57, 1, "F4", *group );
	buttons[0x39] = control;
	controls.push_back( control );
	controls_by_name["F4"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 58, 1, "F5", *group );
	buttons[0x3a] = control;
	controls.push_back( control );
	controls_by_name["F5"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 59, 1, "F6", *group );
	buttons[0x3b] = control;
	controls.push_back( control );
	controls_by_name["F6"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 60, 1, "F7", *group );
	buttons[0x3c] = control;
	controls.push_back( control );
	controls_by_name["F7"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 61, 1, "F8", *group );
	buttons[0x3d] = control;
	controls.push_back( control );
	controls_by_name["F8"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 62, 1, "F9", *group );
	buttons[0x3e] = control;
	controls.push_back( control );
	controls_by_name["F9"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 63, 1, "F10", *group );
	buttons[0x3f] = control;
	controls.push_back( control );
	controls_by_name["F10"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 64, 1, "F11", *group );
	buttons[0x40] = control;
	controls.push_back( control );
	controls_by_name["F11"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 65, 1, "F12", *group );
	buttons[0x41] = control;
	controls.push_back( control );
	controls_by_name["F12"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 66, 1, "F13", *group );
	buttons[0x42] = control;
	controls.push_back( control );
	controls_by_name["F13"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 67, 1, "F14", *group );
	buttons[0x43] = control;
	controls.push_back( control );
	controls_by_name["F14"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 68, 1, "F15", *group );
	buttons[0x44] = control;
	controls.push_back( control );
	controls_by_name["F15"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 69, 1, "F16", *group );
	buttons[0x45] = control;
	controls.push_back( control );
	controls_by_name["F16"] = control;
	group->add( *control );

	group = groups["modifiers"];
	control = new Button ( 70, 1, "shift", *group );
	buttons[0x46] = control;
	controls.push_back( control );
	controls_by_name["shift"] = control;
	group->add( *control );

	group = groups["modifiers"];
	control = new Button ( 71, 1, "option", *group );
	buttons[0x47] = control;
	controls.push_back( control );
	controls_by_name["option"] = control;
	group->add( *control );

	group = groups["modifiers"];
	control = new Button ( 72, 1, "control", *group );
	buttons[0x48] = control;
	controls.push_back( control );
	controls_by_name["control"] = control;
	group->add( *control );

	group = groups["modifiers"];
	control = new Button ( 73, 1, "cmd_alt", *group );
	buttons[0x49] = control;
	controls.push_back( control );
	controls_by_name["cmd_alt"] = control;
	group->add( *control );

	group = groups["automation"];
	control = new Button ( 74, 1, "on", *group );
	buttons[0x4a] = control;
	controls.push_back( control );
	controls_by_name["on"] = control;
	group->add( *control );

	group = groups["automation"];
	control = new Button ( 75, 1, "rec_ready", *group );
	buttons[0x4b] = control;
	controls.push_back( control );
	controls_by_name["rec_ready"] = control;
	group->add( *control );

	group = groups["functions"];
	control = new Button ( 76, 1, "undo", *group );
	buttons[0x4c] = control;
	controls.push_back( control );
	controls_by_name["undo"] = control;
	group->add( *control );

	group = groups["automation"];
	control = new Button ( 77, 1, "snapshot", *group );
	buttons[0x4d] = control;
	controls.push_back( control );
	controls_by_name["snapshot"] = control;
	group->add( *control );

	group = groups["automation"];
	control = new Button ( 78, 1, "touch", *group );
	buttons[0x4e] = control;
	controls.push_back( control );
	controls_by_name["touch"] = control;
	group->add( *control );

	group = groups["functions"];
	control = new Button ( 79, 1, "redo", *group );
	buttons[0x4f] = control;
	controls.push_back( control );
	controls_by_name["redo"] = control;
	group->add( *control );

	group = groups["functions"];
	control = new Button ( 80, 1, "marker", *group );
	buttons[0x50] = control;
	controls.push_back( control );
	controls_by_name["marker"] = control;
	group->add( *control );

	group = groups["functions"];
	control = new Button ( 81, 1, "enter", *group );
	buttons[0x51] = control;
	controls.push_back( control );
	controls_by_name["enter"] = control;
	group->add( *control );

	group = groups["functions"];
	control = new Button ( 82, 1, "cancel", *group );
	buttons[0x52] = control;
	controls.push_back( control );
	controls_by_name["cancel"] = control;
	group->add( *control );

	group = groups["functions"];
	control = new Button ( 83, 1, "mixer", *group );
	buttons[0x53] = control;
	controls.push_back( control );
	controls_by_name["mixer"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 84, 1, "frm_left", *group );
	buttons[0x54] = control;
	controls.push_back( control );
	controls_by_name["frm_left"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 85, 1, "frm_right", *group );
	buttons[0x55] = control;
	controls.push_back( control );
	controls_by_name["frm_right"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 86, 1, "loop", *group );
	buttons[0x56] = control;
	controls.push_back( control );
	controls_by_name["loop"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 87, 1, "punch_in", *group );
	buttons[0x57] = control;
	controls.push_back( control );
	controls_by_name["punch_in"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 88, 1, "punch_out", *group );
	buttons[0x58] = control;
	controls.push_back( control );
	controls_by_name["punch_out"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 89, 1, "home", *group );
	buttons[0x59] = control;
	controls.push_back( control );
	controls_by_name["home"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 90, 1, "end", *group );
	buttons[0x5a] = control;
	controls.push_back( control );
	controls_by_name["end"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 91, 1, "rewind", *group );
	buttons[0x5b] = control;
	controls.push_back( control );
	controls_by_name["rewind"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 92, 1, "ffwd", *group );
	buttons[0x5c] = control;
	controls.push_back( control );
	controls_by_name["ffwd"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 93, 1, "stop", *group );
	buttons[0x5d] = control;
	controls.push_back( control );
	controls_by_name["stop"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 94, 1, "play", *group );
	buttons[0x5e] = control;
	controls.push_back( control );
	controls_by_name["play"] = control;
	group->add( *control );

	group = groups["transport"];
	control = new Button ( 95, 1, "record", *group );
	buttons[0x5f] = control;
	controls.push_back( control );
	controls_by_name["record"] = control;
	group->add( *control );

	group = groups["cursor"];
	control = new Button ( 96, 1, "cursor_up", *group );
	buttons[0x60] = control;
	controls.push_back( control );
	controls_by_name["cursor_up"] = control;
	group->add( *control );

	group = groups["cursor"];
	control = new Button ( 97, 1, "cursor_down", *group );
	buttons[0x61] = control;
	controls.push_back( control );
	controls_by_name["cursor_down"] = control;
	group->add( *control );

	group = groups["cursor"];
	control = new Button ( 98, 1, "cursor_left", *group );
	buttons[0x62] = control;
	controls.push_back( control );
	controls_by_name["cursor_left"] = control;
	group->add( *control );

	group = groups["cursor"];
	control = new Button ( 99, 1, "cursor_right", *group );
	buttons[0x63] = control;
	controls.push_back( control );
	controls_by_name["cursor_right"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 100, 1, "zoom", *group );
	buttons[0x64] = control;
	controls.push_back( control );
	controls_by_name["zoom"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Button ( 101, 1, "scrub", *group );
	buttons[0x65] = control;
	controls.push_back( control );
	controls_by_name["scrub"] = control;
	group->add( *control );

	group = groups["user"];
	control = new Button ( 102, 1, "user_a", *group );
	buttons[0x66] = control;
	controls.push_back( control );
	controls_by_name["user_a"] = control;
	group->add( *control );

	group = groups["user"];
	control = new Button ( 103, 1, "user_b", *group );
	buttons[0x67] = control;
	controls.push_back( control );
	controls_by_name["user_b"] = control;
	group->add( *control );

	group = groups["strip_1"];
	control = new Button ( 104, 1, "fader_touch", *group );
	buttons[0x68] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_2"];
	control = new Button ( 105, 2, "fader_touch", *group );
	buttons[0x69] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_3"];
	control = new Button ( 106, 3, "fader_touch", *group );
	buttons[0x6a] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_4"];
	control = new Button ( 107, 4, "fader_touch", *group );
	buttons[0x6b] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_5"];
	control = new Button ( 108, 5, "fader_touch", *group );
	buttons[0x6c] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_6"];
	control = new Button ( 109, 6, "fader_touch", *group );
	buttons[0x6d] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_7"];
	control = new Button ( 110, 7, "fader_touch", *group );
	buttons[0x6e] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["strip_8"];
	control = new Button ( 111, 8, "fader_touch", *group );
	buttons[0x6f] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["master"];
	control = new Button ( 112, 1, "fader_touch", *group );
	buttons[0x70] = control;
	controls.push_back( control );
	group->add( *control );

	group = groups["none"];
	control = new Led ( 113, 1, "smpte", *group );
	leds[0x71] = control;
	controls.push_back( control );
	controls_by_name["smpte"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Led ( 114, 1, "beats", *group );
	leds[0x72] = control;
	controls.push_back( control );
	controls_by_name["beats"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Led ( 115, 1, "solo", *group );
	leds[0x73] = control;
	controls.push_back( control );
	controls_by_name["solo"] = control;
	group->add( *control );

	group = groups["none"];
	control = new Led ( 118, 1, "relay_click", *group );
	leds[0x76] = control;
	controls.push_back( control );
	controls_by_name["relay_click"] = control;
	group->add( *control );

}

void Mackie::MackieSurface::handle_button( MackieButtonHandler & mbh, ButtonState bs, Button & button )
{
	if ( bs != press && bs != release )
	{
		mbh.update_led( button, none );
		return;
	}
	
	LedState ls;
	switch ( button.id() )
	{

		case 0x28: // io
			switch ( bs ) {
				case press: ls = mbh.io_press( button ); break;
				case release: ls = mbh.io_release( button ); break;
				case neither: break;
			}
			break;

		case 0x29: // sends
			switch ( bs ) {
				case press: ls = mbh.sends_press( button ); break;
				case release: ls = mbh.sends_release( button ); break;
				case neither: break;
			}
			break;

		case 0x2a: // pan
			switch ( bs ) {
				case press: ls = mbh.pan_press( button ); break;
				case release: ls = mbh.pan_release( button ); break;
				case neither: break;
			}
			break;

		case 0x2b: // plugin
			switch ( bs ) {
				case press: ls = mbh.plugin_press( button ); break;
				case release: ls = mbh.plugin_release( button ); break;
				case neither: break;
			}
			break;

		case 0x2c: // eq
			switch ( bs ) {
				case press: ls = mbh.eq_press( button ); break;
				case release: ls = mbh.eq_release( button ); break;
				case neither: break;
			}
			break;

		case 0x2d: // dyn
			switch ( bs ) {
				case press: ls = mbh.dyn_press( button ); break;
				case release: ls = mbh.dyn_release( button ); break;
				case neither: break;
			}
			break;

		case 0x2e: // left
			switch ( bs ) {
				case press: ls = mbh.left_press( button ); break;
				case release: ls = mbh.left_release( button ); break;
				case neither: break;
			}
			break;

		case 0x2f: // right
			switch ( bs ) {
				case press: ls = mbh.right_press( button ); break;
				case release: ls = mbh.right_release( button ); break;
				case neither: break;
			}
			break;

		case 0x30: // channel_left
			switch ( bs ) {
				case press: ls = mbh.channel_left_press( button ); break;
				case release: ls = mbh.channel_left_release( button ); break;
				case neither: break;
			}
			break;

		case 0x31: // channel_right
			switch ( bs ) {
				case press: ls = mbh.channel_right_press( button ); break;
				case release: ls = mbh.channel_right_release( button ); break;
				case neither: break;
			}
			break;

		case 0x32: // flip
			switch ( bs ) {
				case press: ls = mbh.flip_press( button ); break;
				case release: ls = mbh.flip_release( button ); break;
				case neither: break;
			}
			break;

		case 0x33: // edit
			switch ( bs ) {
				case press: ls = mbh.edit_press( button ); break;
				case release: ls = mbh.edit_release( button ); break;
				case neither: break;
			}
			break;

		case 0x34: // name_value
			switch ( bs ) {
				case press: ls = mbh.name_value_press( button ); break;
				case release: ls = mbh.name_value_release( button ); break;
				case neither: break;
			}
			break;

		case 0x35: // smpte_beats
			switch ( bs ) {
				case press: ls = mbh.smpte_beats_press( button ); break;
				case release: ls = mbh.smpte_beats_release( button ); break;
				case neither: break;
			}
			break;

		case 0x36: // F1
			switch ( bs ) {
				case press: ls = mbh.F1_press( button ); break;
				case release: ls = mbh.F1_release( button ); break;
				case neither: break;
			}
			break;

		case 0x37: // F2
			switch ( bs ) {
				case press: ls = mbh.F2_press( button ); break;
				case release: ls = mbh.F2_release( button ); break;
				case neither: break;
			}
			break;

		case 0x38: // F3
			switch ( bs ) {
				case press: ls = mbh.F3_press( button ); break;
				case release: ls = mbh.F3_release( button ); break;
				case neither: break;
			}
			break;

		case 0x39: // F4
			switch ( bs ) {
				case press: ls = mbh.F4_press( button ); break;
				case release: ls = mbh.F4_release( button ); break;
				case neither: break;
			}
			break;

		case 0x3a: // F5
			switch ( bs ) {
				case press: ls = mbh.F5_press( button ); break;
				case release: ls = mbh.F5_release( button ); break;
				case neither: break;
			}
			break;

		case 0x3b: // F6
			switch ( bs ) {
				case press: ls = mbh.F6_press( button ); break;
				case release: ls = mbh.F6_release( button ); break;
				case neither: break;
			}
			break;

		case 0x3c: // F7
			switch ( bs ) {
				case press: ls = mbh.F7_press( button ); break;
				case release: ls = mbh.F7_release( button ); break;
				case neither: break;
			}
			break;

		case 0x3d: // F8
			switch ( bs ) {
				case press: ls = mbh.F8_press( button ); break;
				case release: ls = mbh.F8_release( button ); break;
				case neither: break;
			}
			break;

		case 0x3e: // F9
			switch ( bs ) {
				case press: ls = mbh.F9_press( button ); break;
				case release: ls = mbh.F9_release( button ); break;
				case neither: break;
			}
			break;

		case 0x3f: // F10
			switch ( bs ) {
				case press: ls = mbh.F10_press( button ); break;
				case release: ls = mbh.F10_release( button ); break;
				case neither: break;
			}
			break;

		case 0x40: // F11
			switch ( bs ) {
				case press: ls = mbh.F11_press( button ); break;
				case release: ls = mbh.F11_release( button ); break;
				case neither: break;
			}
			break;

		case 0x41: // F12
			switch ( bs ) {
				case press: ls = mbh.F12_press( button ); break;
				case release: ls = mbh.F12_release( button ); break;
				case neither: break;
			}
			break;

		case 0x42: // F13
			switch ( bs ) {
				case press: ls = mbh.F13_press( button ); break;
				case release: ls = mbh.F13_release( button ); break;
				case neither: break;
			}
			break;

		case 0x43: // F14
			switch ( bs ) {
				case press: ls = mbh.F14_press( button ); break;
				case release: ls = mbh.F14_release( button ); break;
				case neither: break;
			}
			break;

		case 0x44: // F15
			switch ( bs ) {
				case press: ls = mbh.F15_press( button ); break;
				case release: ls = mbh.F15_release( button ); break;
				case neither: break;
			}
			break;

		case 0x45: // F16
			switch ( bs ) {
				case press: ls = mbh.F16_press( button ); break;
				case release: ls = mbh.F16_release( button ); break;
				case neither: break;
			}
			break;

		case 0x46: // shift
			switch ( bs ) {
				case press: ls = mbh.shift_press( button ); break;
				case release: ls = mbh.shift_release( button ); break;
				case neither: break;
			}
			break;

		case 0x47: // option
			switch ( bs ) {
				case press: ls = mbh.option_press( button ); break;
				case release: ls = mbh.option_release( button ); break;
				case neither: break;
			}
			break;

		case 0x48: // control
			switch ( bs ) {
				case press: ls = mbh.control_press( button ); break;
				case release: ls = mbh.control_release( button ); break;
				case neither: break;
			}
			break;

		case 0x49: // cmd_alt
			switch ( bs ) {
				case press: ls = mbh.cmd_alt_press( button ); break;
				case release: ls = mbh.cmd_alt_release( button ); break;
				case neither: break;
			}
			break;

		case 0x4a: // on
			switch ( bs ) {
				case press: ls = mbh.on_press( button ); break;
				case release: ls = mbh.on_release( button ); break;
				case neither: break;
			}
			break;

		case 0x4b: // rec_ready
			switch ( bs ) {
				case press: ls = mbh.rec_ready_press( button ); break;
				case release: ls = mbh.rec_ready_release( button ); break;
				case neither: break;
			}
			break;

		case 0x4c: // undo
			switch ( bs ) {
				case press: ls = mbh.undo_press( button ); break;
				case release: ls = mbh.undo_release( button ); break;
				case neither: break;
			}
			break;

		case 0x4d: // snapshot
			switch ( bs ) {
				case press: ls = mbh.snapshot_press( button ); break;
				case release: ls = mbh.snapshot_release( button ); break;
				case neither: break;
			}
			break;

		case 0x4e: // touch
			switch ( bs ) {
				case press: ls = mbh.touch_press( button ); break;
				case release: ls = mbh.touch_release( button ); break;
				case neither: break;
			}
			break;

		case 0x4f: // redo
			switch ( bs ) {
				case press: ls = mbh.redo_press( button ); break;
				case release: ls = mbh.redo_release( button ); break;
				case neither: break;
			}
			break;

		case 0x50: // marker
			switch ( bs ) {
				case press: ls = mbh.marker_press( button ); break;
				case release: ls = mbh.marker_release( button ); break;
				case neither: break;
			}
			break;

		case 0x51: // enter
			switch ( bs ) {
				case press: ls = mbh.enter_press( button ); break;
				case release: ls = mbh.enter_release( button ); break;
				case neither: break;
			}
			break;

		case 0x52: // cancel
			switch ( bs ) {
				case press: ls = mbh.cancel_press( button ); break;
				case release: ls = mbh.cancel_release( button ); break;
				case neither: break;
			}
			break;

		case 0x53: // mixer
			switch ( bs ) {
				case press: ls = mbh.mixer_press( button ); break;
				case release: ls = mbh.mixer_release( button ); break;
				case neither: break;
			}
			break;

		case 0x54: // frm_left
			switch ( bs ) {
				case press: ls = mbh.frm_left_press( button ); break;
				case release: ls = mbh.frm_left_release( button ); break;
				case neither: break;
			}
			break;

		case 0x55: // frm_right
			switch ( bs ) {
				case press: ls = mbh.frm_right_press( button ); break;
				case release: ls = mbh.frm_right_release( button ); break;
				case neither: break;
			}
			break;

		case 0x56: // loop
			switch ( bs ) {
				case press: ls = mbh.loop_press( button ); break;
				case release: ls = mbh.loop_release( button ); break;
				case neither: break;
			}
			break;

		case 0x57: // punch_in
			switch ( bs ) {
				case press: ls = mbh.punch_in_press( button ); break;
				case release: ls = mbh.punch_in_release( button ); break;
				case neither: break;
			}
			break;

		case 0x58: // punch_out
			switch ( bs ) {
				case press: ls = mbh.punch_out_press( button ); break;
				case release: ls = mbh.punch_out_release( button ); break;
				case neither: break;
			}
			break;

		case 0x59: // home
			switch ( bs ) {
				case press: ls = mbh.home_press( button ); break;
				case release: ls = mbh.home_release( button ); break;
				case neither: break;
			}
			break;

		case 0x5a: // end
			switch ( bs ) {
				case press: ls = mbh.end_press( button ); break;
				case release: ls = mbh.end_release( button ); break;
				case neither: break;
			}
			break;

		case 0x5b: // rewind
			switch ( bs ) {
				case press: ls = mbh.rewind_press( button ); break;
				case release: ls = mbh.rewind_release( button ); break;
				case neither: break;
			}
			break;

		case 0x5c: // ffwd
			switch ( bs ) {
				case press: ls = mbh.ffwd_press( button ); break;
				case release: ls = mbh.ffwd_release( button ); break;
				case neither: break;
			}
			break;

		case 0x5d: // stop
			switch ( bs ) {
				case press: ls = mbh.stop_press( button ); break;
				case release: ls = mbh.stop_release( button ); break;
				case neither: break;
			}
			break;

		case 0x5e: // play
			switch ( bs ) {
				case press: ls = mbh.play_press( button ); break;
				case release: ls = mbh.play_release( button ); break;
				case neither: break;
			}
			break;

		case 0x5f: // record
			switch ( bs ) {
				case press: ls = mbh.record_press( button ); break;
				case release: ls = mbh.record_release( button ); break;
				case neither: break;
			}
			break;

		case 0x60: // cursor_up
			switch ( bs ) {
				case press: ls = mbh.cursor_up_press( button ); break;
				case release: ls = mbh.cursor_up_release( button ); break;
				case neither: break;
			}
			break;

		case 0x61: // cursor_down
			switch ( bs ) {
				case press: ls = mbh.cursor_down_press( button ); break;
				case release: ls = mbh.cursor_down_release( button ); break;
				case neither: break;
			}
			break;

		case 0x62: // cursor_left
			switch ( bs ) {
				case press: ls = mbh.cursor_left_press( button ); break;
				case release: ls = mbh.cursor_left_release( button ); break;
				case neither: break;
			}
			break;

		case 0x63: // cursor_right
			switch ( bs ) {
				case press: ls = mbh.cursor_right_press( button ); break;
				case release: ls = mbh.cursor_right_release( button ); break;
				case neither: break;
			}
			break;

		case 0x64: // zoom
			switch ( bs ) {
				case press: ls = mbh.zoom_press( button ); break;
				case release: ls = mbh.zoom_release( button ); break;
				case neither: break;
			}
			break;

		case 0x65: // scrub
			switch ( bs ) {
				case press: ls = mbh.scrub_press( button ); break;
				case release: ls = mbh.scrub_release( button ); break;
				case neither: break;
			}
			break;

		case 0x66: // user_a
			switch ( bs ) {
				case press: ls = mbh.user_a_press( button ); break;
				case release: ls = mbh.user_a_release( button ); break;
				case neither: break;
			}
			break;

		case 0x67: // user_b
			switch ( bs ) {
				case press: ls = mbh.user_b_press( button ); break;
				case release: ls = mbh.user_b_release( button ); break;
				case neither: break;
			}
			break;

	}
	mbh.update_led( button, ls );
}
