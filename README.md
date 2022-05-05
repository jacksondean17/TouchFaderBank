![Header](https://user-images.githubusercontent.com/19520849/166882946-8b9f3ceb-d19b-4c20-9c4b-a202c960e142.png)

# Touch Fader Bank

Our Touch Fader Bank is an innovative fader bank based on capacitive touch sensing and LED level metering. Compared to conventional mechanical faders, these touch-based faders offer the following key advantages:

- *Synchronizable* — Touch faders are soft controls that can automatically update their values to represent multiple parameter groups or to reflect changes from other input sources.
- *Interactive* — Touch faders are a highly interactive interface technology that can accept a wide range of input gestures for versatile and intuitive control. LED level meters provider richer visual feedback compared to conventional mechanical faders.
- *Robust* — Touch faders are less susceptible to damage and have greater longevity compared to conventional mechanical faders.

The fader bank is for use as a peripheral controller to any digital audio workstation (DAW). The device communicates with a DAW over USB using a Type-C connector. Its four general-purpose touch faders plus a touch button can be mapped to any MIDI-mappable parameter in the DAW.

Created by students at the University of Utah for *ECE 5780: Embedded System Design*.

## Authors
- Brady Hartog
- Jackson Dean

## Device Usage
1. Power the fader bank using a 5V DC power supply.
2. Connect the fader bank to your computer using a USB-C cable.
3. In your DAW software, map MIDI control channels to the faders on the device. This process is different for different DAWs. To your computer the fader bank will appear as a USB audio device called "Alef Audio Fader Bank."

## Repository Usage
The fader bank is implemented using the STM32F072 microcontroller. Project directories are configured and automatically generated using STM32CubeMX. Projects are developed using the Keil MDK toolchain. As part of this toolchain, project code is compiled and flashed to the microcontroller using the Keil μVision IDE and ST-Link programmer/debugger.

## Repository Organization
`FaderBankv1` is the project directory of the fader bank firmware. The `Verification` directory contains other project directories used for developing essential features such as capacitive sensing and LED level metering.

## Resources
The fader bank firmware depends on the following resources:
- [STM32 MIDI Brain](https://github.com/samjkent/stm32f4-midi-brain) by Sam Kent
- Various STM32 HAL libraries
