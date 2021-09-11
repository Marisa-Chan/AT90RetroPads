/*
  Joystick2.cpp

  Copyright (c) 2015, Matthew Heironimus

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Joystick2.h"

#if defined(_USING_HID)

#define JOYSTICK_REPORT_ID  0x03
#define JOYSTICK2_REPORT_ID 0x04
#define JOYSTICK3_REPORT_ID  0x05
#define JOYSTICK4_REPORT_ID  0x06
#define JOYSTICK_STATE_SIZE 3

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
    // Joystick #1 SEGA 1
    0x05, 0x01,               // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,               // USAGE (Joystick)
    0xa1, 0x01,               // COLLECTION (Application)
    0x85, JOYSTICK_REPORT_ID, // REPORT_ID (3)

    // 8 Buttons
    0x05, 0x09,               //   USAGE_PAGE (Button)
    0x19, 0x01,               //   USAGE_MINIMUM (Button 1)
    0x29, 0x08,               //   USAGE_MAXIMUM (Button 8)
    0x15, 0x00,               //   LOGICAL_MINIMUM (0)
    0x25, 0x01,               //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,               //   REPORT_SIZE (1)
    0x95, 0x08,               //   REPORT_COUNT (8)
    0x55, 0x00,               //   UNIT_EXPONENT (0)
    0x65, 0x00,               //   UNIT (None)
    0x81, 0x02,               //   INPUT (Data,Var,Abs)

    // X and Y Axis
    0x05, 0x01,               //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,               //   USAGE (Pointer)
    0x15, 0x81,               //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,               //   LOGICAL_MAXIMUM (127)
    0xA1, 0x00,               //   COLLECTION (Physical)
    0x09, 0x30,               //     USAGE (x)
    0x09, 0x31,               //     USAGE (y)
    0x75, 0x08,               //     REPORT_SIZE (8)
    0x95, 0x02,               //     REPORT_COUNT (2)
    0x81, 0x02,               //     INPUT (Data,Var,Abs)
    0xc0,                     //   END_COLLECTION
    0xc0,                     // END_COLLECTION

    // Joystick #2 SEGA 2
    0x05, 0x01,                // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                // USAGE (Joystick)
    0xa1, 0x01,                // COLLECTION (Application)
    0x85, JOYSTICK2_REPORT_ID, // REPORT_ID (4)

    // 8 Buttons
    0x05, 0x09,                //   USAGE_PAGE (Button)
    0x19, 0x01,                //   USAGE_MINIMUM (Button 1)
    0x29, 0x08,                //   USAGE_MAXIMUM (Button 8)
    0x15, 0x00,                //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                //   REPORT_SIZE (1)
    0x95, 0x08,                //   REPORT_COUNT (8)
    0x55, 0x00,                //   UNIT_EXPONENT (0)
    0x65, 0x00,                //   UNIT (None)
    0x81, 0x02,                //   INPUT (Data,Var,Abs)

    // X and Y Axis
    0x05, 0x01,                //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                //   USAGE (Pointer)
    0x15, 0x81,                //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                //   LOGICAL_MAXIMUM (127)
    0xA1, 0x00,                //   COLLECTION (Physical)
    0x09, 0x30,                //     USAGE (x)
    0x09, 0x31,                //     USAGE (y)
    0x75, 0x08,                //     REPORT_SIZE (8)
    0x95, 0x02,                //     REPORT_COUNT (2)
    0x81, 0x02,                //     INPUT (Data,Var,Abs)
    0xc0,                      //   END_COLLECTION
    0xc0,                       // END_COLLECTION

    // Joystick #3 NES 1
    0x05, 0x01,                // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                // USAGE (Joystick)
    0xa1, 0x01,                // COLLECTION (Application)
    0x85, JOYSTICK3_REPORT_ID, // REPORT_ID (4)

    // 16 Buttons
    0x05, 0x09,                //   USAGE_PAGE (Button)
    0x19, 0x01,                //   USAGE_MINIMUM (Button 1)
    0x29, 0x04,                //   USAGE_MAXIMUM (Button 4)
    0x15, 0x00,                //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                //   REPORT_SIZE (1)
    0x95, 0x04,                //   REPORT_COUNT (4)
    0x55, 0x00,                //   UNIT_EXPONENT (0)
    0x65, 0x00,                //   UNIT (None)
    0x81, 0x02,                //   INPUT (Data,Var,Abs)

    0x75, 0x04,                //   Report Size (4)
    0x95, 0x01,                //   Report Count (1)
    0x81, 0x01,                //   Input (Cnst,Arr,Abs)

    // X and Y Axis
    0x05, 0x01,                //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                //   USAGE (Pointer)
    0x15, 0x81,                //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                //   LOGICAL_MAXIMUM (127)
    0xA1, 0x00,                //   COLLECTION (Physical)
    0x09, 0x30,                //     USAGE (x)
    0x09, 0x31,                //     USAGE (y)
    0x75, 0x08,                //     REPORT_SIZE (8)
    0x95, 0x02,                //     REPORT_COUNT (2)
    0x81, 0x02,                //     INPUT (Data,Var,Abs)
    0xc0,                      //   END_COLLECTION
    0xc0,                       // END_COLLECTION

    // Joystick #4  NES 2
    0x05, 0x01,                // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                // USAGE (Joystick)
    0xa1, 0x01,                // COLLECTION (Application)
    0x85, JOYSTICK4_REPORT_ID, // REPORT_ID (4)

    // 16 Buttons
    0x05, 0x09,                //   USAGE_PAGE (Button)
    0x19, 0x01,                //   USAGE_MINIMUM (Button 1)
    0x29, 0x04,                //   USAGE_MAXIMUM (Button 4)
    0x15, 0x00,                //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                //   REPORT_SIZE (1)
    0x95, 0x04,                //   REPORT_COUNT (4)
    0x55, 0x00,                //   UNIT_EXPONENT (0)
    0x65, 0x00,                //   UNIT (None)
    0x81, 0x02,                //   INPUT (Data,Var,Abs)

    0x75, 0x04,                //   Report Size (4)
    0x95, 0x01,                //   Report Count (1)
    0x81, 0x01,                //   Input (Cnst,Arr,Abs)

    // X and Y Axis
    0x05, 0x01,                //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                //   USAGE (Pointer)
    0x15, 0x81,                //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                //   LOGICAL_MAXIMUM (127)
    0xA1, 0x00,                //   COLLECTION (Physical)
    0x09, 0x30,                //     USAGE (x)
    0x09, 0x31,                //     USAGE (y)
    0x75, 0x08,                //     REPORT_SIZE (8)
    0x95, 0x02,                //     REPORT_COUNT (2)
    0x81, 0x02,                //     INPUT (Data,Var,Abs)
    0xc0,                      //   END_COLLECTION
    0xc0                       // END_COLLECTION
};

Joystick_::Joystick_(uint8_t initJoystickId)
{
    // Setup HID report structure
	static bool usbSetup = false;
	
	if (!usbSetup)
	{
	    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	    HID().AppendDescriptor(&node);
        usbSetup = true;
	}
    
    // Initalize State
    joystickId = initJoystickId;
}

void Joystick_::sendState(int8_t ax, int8_t ay, uint8_t bb)
{
    int8_t data[JOYSTICK_STATE_SIZE] = { bb, ax, ay};

    HID().SendReport(JOYSTICK_REPORT_ID + joystickId, data, JOYSTICK_STATE_SIZE);
}

Joystick_ Joystick[4] =
{
    Joystick_(0),
    Joystick_(1),
    Joystick_(2),
    Joystick_(3)
};

#endif
