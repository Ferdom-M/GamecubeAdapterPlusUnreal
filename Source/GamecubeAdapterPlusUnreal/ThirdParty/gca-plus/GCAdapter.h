
#pragma once

#define WIN32_LEAN_AND_MEAN 

#include <include/libusb-1.0/libusb.h>
#include <sstream>
#include "ControllerStatus.h"

#ifndef GC_CONTROLLER_NAMES
#define GC_CONTROLLER_NAMES

#define PORT_1 0
#define PORT_2 1
#define PORT_3 2
#define PORT_4 3

#endif

#pragma once

namespace gca {

	int AddAdapter(libusb_device *dev);
	int Load();
	int Setup();
	void Process(ControllerStatus* buffer, int arraySize);
	void CheckRumble(bool* tRumble, int arraySize);
	int Stop();
	std::string RawData();
	void Read();
	bool IsAccessible(libusb_device *dev);
	unsigned int GetNthBit(uint8_t number, int n);
	ControllerStatus GetGamepadStatus(uint8_t * _controller_payload, int port);
	const uint16_t GAMECUBE_VID = 0x057E;
	const uint16_t GAMECUBE_PID = 0x0337;
	void ResetCalibration(int port);
}
