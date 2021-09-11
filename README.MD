AT90USB162 RetroPad
2 x MD2 controller + 2 Nes controller to USB converter

############
Scheme.pdf - wire scheme, how it's organised for me.
Arduino Profile - AT90USB162 integration into Arduino IDE
Firmware - device firmware
hvpe - firmware of highvoltage programmer for trebleshoot fuses bugs

############

For uploading firmware I use cheap USBASP programmer.

I use 8Mhz XTAL and my fuses are: E:FE, H:D9, L:FF

If something going wrong with uploading program via SPI and it's will be locked - you may create HVPE programmer with Arduino Mega 2560 (it's also can be done with UNO, but you must modify firmware and disable Serial output)

Content of `boards.txt` (Arduino Profile) must be added into `boards.txt` of your Arduino's avr core. Content of `variants` folder must be placed also into `variants` of avr core.

############



Device firmware created based on:
https://github.com/MHeironimus/ArduinoJoystickLibrary/tree/version-1.0/Joystick2
https://jonthysell.com/2014/09/29/sega-genesis-controllers-and-arduino-revisited/


HVPE firmware is modified version of https://mightyohm.com/blog/wp-content/uploads/2008/09/hvfuse.pde with additional steps and serial output.
Schematic and info is similar to https://mightyohm.com/blog/2008/09/arduino-based-avr-high-voltage-programmer/



VID =0x0e8f and PID =0x3013 ( GREENASIA_DUAL_USB_JOYPAD ) chosed because it's already has HID quirks for multi-HID in Linux.
