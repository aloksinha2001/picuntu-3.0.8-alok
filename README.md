picuntu-3.0.8-alok
==================

rk3066-kernel - derived from mix of andrew source, omegamoon, 0.3a kernel, driver sources

Integrated kernel for Picuntu 3.0.8-alok+ 
=============

Linux/Android kernel for the Rockchip RK3066 SoC

Changelog - Kernel - 3.0.8-alok+
- Cleaned the previous kernel
- Removed unwanted TV Tuner cards
- Bluetooth support retained
- All cpufreq governors now available - Try userspace, and hav fun.. (Read warning below)
- Default governor Ondemand
- Added Serial modules - FTD-SIO, pl2303, Garmin - for serial devices,
- Added Phone modems - For connecting Data card dongles
- Added UVC Camera support - Many users have been asking for this
- Added oprofiler support
- Added iptables
- Added NAT/Firewall (inc GRE) support - Now you can make your PicUntu a complete firewall
- Added USB 1.0 support - remember how your old USB hubs were not being used for bootup, well they _should_ work now.
- DW_OTG_ debug report suppressed
- Added USB SCSI adapter - I have lots of SCSI harddisk lying around, and USB adapter for them, but they seemed not to work earlier.
- Added Pegasus network support
- FB set - all options enabled - you should have better control for fb setting.
- Added auxilary Video - If you have a USB LED,LCD screen lying around - try connecting to PIcUntu, should work. (in addition to HDMI) - required for in-car applications.
- Modules for USB Printer - now you should be able to connect USB printers to PicUntu
- Serial Modem drivers - Qualcom, NAVMAN,
- Added Joliet/UDF support - You should be able to connect USB CDROM
- NFS Client, NFS Server support for V3 , V4 added
- Ofcourse CIFS continues
- Integrated BCM40181, rtl8188eu support - so that you no longer need to separately look for modules for basic wifi
- Three graphic driver kernels - 1080p, 720, VGA are released.
- No it does not yet have support for MX1 wifi, RK802III/s - wifi - I am still hunting for the source. - A Kirby from Rikomagic was successful in getting us the source code for MT. (wifi + BT) trying to get that integrated now.

=======

Apr 18th, 2013
Merged into aloksinha's kernel the following ones:

Omegamoon, with several CPU and I/O governors (https://github.com/omegamoon/rockchip-rk30xx-mk808)
olegk0, with Mali HW 3D (https://github.com/olegk0/rk3066-kernel)
and specially the more up to date Jelly Bean RK3066 kernel from https://github.com/kelvinchengxiwu/rk\_kernel (look for rknand\_base\_ko.c, picuntu version is 4.34 from Apr2012, whereas here it is 4.40 from Aug2012), mostly anywhere you look at in this kernel you can spot bugfixes and enhancements. Also added webcam used by Measy U2C, Kimdecent B12, and NX003II sticks.
WARNING: The .config disables many wireless drivers, re-enable them as modules or in-kernel. Also, my memory is DDR3 1333 (667 MHz), change that in the .config.
The board-rk30-box.c has been updated with max GPU speed 400 MHz (instead of 266), and max CPU frequency 1608 MHz (instead of 1200), both values allowed by Rockchip (and my Measy U2C stick).

==========
