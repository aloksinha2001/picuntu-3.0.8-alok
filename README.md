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
- No it does not yet have support for MX1 wifi, RK802III/s - wifi - I am still hunting for the source.

=======

