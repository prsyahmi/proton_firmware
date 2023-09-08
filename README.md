# Proton IHU APK Sideload Status

This repo doesn't show method on how to install APKs to your IHU. But only serve as a place to track status of it and others technical information.

Some car like EXORA doesn't have direct link between car system and IHU and it should be safe to meddle with it. However, for other such as X70 you must exercise caution and I recommend against it.

# Architecture
The first firmware (GKUI) was developed by ECARX and its development is continued by ACOTech with the release of GKUI19 and ATLAS OS.

Some car will have IHU and MCU comes together while other will have extra TBOX that connects all of the car hardware together. This gave the car ability to stay connected when not using it.

Since I haven't dissect the hardware, the next statement is only speculation:
The MCU add another layer of interface between IHU and the user/hardware for example the speaker connects to MCU rather than IHU directly.

When the device boot, it will send a series of command to MCU to initialize it. We can observe sometimes that the car will have its speaker silent not producing sound even we set it to max in IHU until restarted. This is due to the init code is not properly sent to MCU or interrupted or if MCU is not ready to accept command. For example, when cranking car engine one of the IHU/MCU initialization may be interrupted.

# Firmware & Status

The firmware is usually sent to user via OTA and optionally Proton can update the firmware at SC. With OTA, some of the firmware will arrive to the user in its full form while some will arrive in diff mode.

When hacking the IHU, I strongly discourage to write to `/system` partition directly as this may refuse future update for diff mode or if the updates was forced, it may corrupt the filesystem.

## [EXORA](EXORA) / PERSONA / IRIZ - GKUI
|Model|Firmware|Method|Root Privilege|Comment|
|-----|--------|------|-------|-------|
|EXORA | GKUI (all version) V3.1.3E V3.1.5E V3.2.6E | Loophole | Available | Use exec on Factory Settings
|PERSONA/IRIZ | GKUI (all version) V3.1.5E | Loophole | Available | Use exec on Factory Settings

## PERSONA MC2 / IRIZ MC3 - ATLAS
|Model|Firmware|Method|Root Privilege|Comment|
|-----|--------|------|-------|-------|
|IRIZ | ATLAS | Loophole | ? | Hole from one of the app
|PERSONA MC2 | ATLAS SWPERSO1213H1803 | Loophole | ? | Hole from one of the app 

## X70 - [GKUI](X70_GKUI) / [ATLAS](X70_ATLAS)
|Model|Firmware|Method|Root Privilege|Comment|
|-----|--------|------|-------|-------|
|X70 | GKUI (CBU/Older unknown version) | Engineer Mode | ? | Enter engineer mode via dialer code and select install APK
|X70 | GKUI V4.0.3(IC4) and below | Exploit | Available | Crafting special file
|X70 | ATLAS SWNL3RB0814H1091 | Simple Flags | ? | |
|X70 | ATLAS SWNL3RB1018H1735.00153 | Simple Flags | ? | |
|X70 | ATLAS SWNL3RB0406H11750.00228 | Downgrade | ? | |
|X70 | ATLAS SWNL3RB050411751.00224 | Downgrade | ? | This updates the MCU only |

## X50 (H = Premium/Flagship, L = Executive/Standard) [GKUI](X50_GKUI) / [ATLAS](X50_ATLAS)
|HW|OS|Firmware|Method|Root Privilege|Comment|
|--|--|--------|------|-------|-------|
|H090 | GKUI19 | SW0SXRA0911H5063.00374 | | 
|H090 | GKUI19 | SW0SXRA1201H5064.00379 | Init Script | Available | USB\b832bc61472727635baffcf25dd28e9f239273e2\...sh
|H200 | GKUI19 | SW0SXRA0121H5167.00411 | Init Script | Available | USB\b832bc61472727635baffcf25dd28e9f239273e2\...sh
|H200 | GKUI19 | SW0SXRA0121L5167.00412 | Init Script | Available | USB\b832bc61472727635baffcf25dd28e9f239273e2\...sh
|H090/H200 | ATLAS | SW0SXRA0616H5170.00581 | | | 
|H090/H200 | ATLAS | SW0SXRA0616L5170.00582 | | | 
| | ATLAS | SW0SXRA0929H5170.00598 | | | 
|H090/H200 | ATLAS | SW0SXRA1124H5170.00617 SW0SXRA1124L5170.00618 (v1) | Init Script | Available | Able to install APKs with simple file copy, but cannot invoke through installer due to APK installer protection in place
|H090/H200 | ATLAS | SW0SXRA1124H5170.00617 SW0SXRA1124L5170.00618 (v2) | Downgrade | ? | Same version number, but ACOTech silently push the updates with removal of init script backdoor
|H090/H200 | ATLAS | SW0SXRA0413H5170.00644 SW0SXRA0413L5170.00645 | Downgrade | ? | 

## X90
|HW|SW|FW|Method|Root Privilege|Comment|
|-----|--------|------|-------|-------|-------|
|HW0VX110506H0101|SWVX11R0224H8CWH.00553|IHU523P-00-CWH| | | |

# Group
[Click here](GROUP)
