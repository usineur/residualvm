/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#define FORBIDDEN_SYMBOL_EXCEPTION_printf

#include "common/scummsys.h"
#include "common/config-manager.h"

#include "backends/events/switchsdl/switchsdl-events.h"
#include "backends/fs/posix-drives/posix-drives-fs-factory.h"
#include "backends/fs/posix-drives/posix-drives-fs.h"
#include "backends/platform/sdl/switch/switch.h"
#include "backends/saves/posix/posix-saves.h"

#include <switch.h>

OSystem_Switch::OSystem_Switch(Common::String baseConfigName)
	: _baseConfigName(baseConfigName) {
}

void OSystem_Switch::init() {
	// Initialze File System Factory
	DrivesPOSIXFilesystemFactory *fsFactory = new DrivesPOSIXFilesystemFactory();
	fsFactory->addDrive("sdmc:");
	fsFactory->configureBuffering(DrivePOSIXFilesystemNode::kBufferingModeScummVM, 2048);

	_fsFactory = fsFactory;

	// Invoke parent implementation of this method
	OSystem_SDL::init();
}

void OSystem_Switch::initBackend() {
	ConfMan.registerDefault("fullscreen", true);
	ConfMan.registerDefault("aspect_ratio", true);
	ConfMan.registerDefault("output_rate", 48000);
	ConfMan.registerDefault("touchpad_mouse_mode", false);
	ConfMan.registerDefault("enable_unsupported_game_warning", false);

	ConfMan.setInt("joystick_num", 0);
	ConfMan.setBool("fullscreen", true);
	ConfMan.set("fullscreen_res", "1280x720");

	if (!ConfMan.hasKey("aspect_ratio")) {
		ConfMan.setBool("aspect_ratio", true);
	}
	if (!ConfMan.hasKey("output_rate")) {
		ConfMan.setInt("output_rate", 48000);
	}
	if (!ConfMan.hasKey("touchpad_mouse_mode")) {
		ConfMan.setBool("touchpad_mouse_mode", false);
	}
	if (!ConfMan.hasKey("check_gamedata")) {
		ConfMan.setBool("check_gamedata", false);
	}
	if (!ConfMan.hasKey("enable_unsupported_game_warning")) {
		ConfMan.setBool("enable_unsupported_game_warning", false);
	}

	// Create the savefile manager
	if (_savefileManager == 0) {
		_savefileManager = new POSIXSaveFileManager();
	}

	// Event source
	if (_eventSource == 0) {
		_eventSource = new SwitchSdlEventSource();
	}

	// Invoke parent implementation of this method
	OSystem_SDL::initBackend();
}

bool OSystem_Switch::hasFeature(Feature f) {
	if (f == kFeatureDisplayLogFile)
		return false;
	if (f == kFeatureOpenUrl)
		return false;
	if (f == kFeatureCursorPalette)
		return true;
	if (f == kFeatureFullscreenMode)
		return false;
	if (f == kFeatureTouchpadMode)
		return true;

	return OSystem_SDL::hasFeature(f);
}

void OSystem_Switch::setFeatureState(Feature f, bool enable) {
	switch (f) {
	case kFeatureTouchpadMode:
		ConfMan.setBool("touchpad_mouse_mode", enable);
		break;
	default:
		OSystem_SDL::setFeatureState(f, enable);
		break;
	}
}

bool OSystem_Switch::getFeatureState(Feature f) {
	switch (f) {
	case kFeatureTouchpadMode:
		return ConfMan.getBool("touchpad_mouse_mode");
		break;
	default:
		return OSystem_SDL::getFeatureState(f);
		break;
	}
}

void OSystem_Switch::logMessage(LogMessageType::Type type, const char *message) {
	printf("%s", message);
}

Common::String OSystem_Switch::getDefaultConfigFileName() {
	return _baseConfigName;
}

Common::WriteStream *OSystem_Switch::createLogFile() {
	Common::FSNode file("residualvm.log");
	return file.createWriteStream();
}

Common::String OSystem_Switch::getSystemLanguage() const {
	Common::String lang;
	u64 languageCode = 0;
	SetLanguage language = SetLanguage_ENUS;
	setInitialize();
	setGetSystemLanguage(&languageCode);
	setMakeLanguage(languageCode, &language);
	switch (language) {
	case SetLanguage_JA:
		lang = "jp";
		break;
	case SetLanguage_FR:
	case SetLanguage_FRCA:
		lang = "fr";
		break;
	case SetLanguage_DE:
		lang = "de";
		break;
	case SetLanguage_IT:
		lang = "it";
		break;
	case SetLanguage_ES:
		lang = "es";
		break;
	case SetLanguage_ZHCN:
		lang = "zh-cn";
		break;
	case SetLanguage_KO:
		lang = "kr";
		break;
	case SetLanguage_NL:
		lang = "nl";
		break;
	case SetLanguage_PT:
		lang = "pt";
		break;
	case SetLanguage_RU:
		lang = "ru";
		break;
	case SetLanguage_ZHTW:
		lang = "zh";
		break;
	default:
		lang = "en";
		break;
	}
	return lang;
}
