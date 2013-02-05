<<<<<<< HEAD
rk3066-kernel
=============

Linux/Android kernel for the Rockchip RK3066 SoC

Changelog - Kernel - 3.0.8-alok+
- Cleaned the previous kernel
- Removed unwanted TV Tuner cards
- Removed Bluetooth support
- All cpufreq governors now available
- Default governor performance
- Added Serial modules - FDD-SIO, pl2303, Garmin - for serial devices,
- Added Phone modems - For connecting Data card dongles
- Added UVC Camera support - Many users have been asking for this
- Added oprofiler support
- Added iptables
- Added NAT/Firewall (inc GRE) support - Now you can make your PicUntu a complete firewall
- Added USB 1.0 support - remember how your old USB hubs were not being used for bootup, well they _should_ work now.
- DW_OTG_ debug report suppressed
- Added USB SCSI adapter - I have lots of SCSI harddisk lying around, and USB adapter for them, but they seemed not to work earlier.
- Added Pegasus network support
- FB set - all options enabled - you should have better control for fb setting. I will also release the 720p kernel soon.
- Added auxilary Video - If you have a USB LED,LCD screen lying around - try connecting to PIcUntu, should work. (in addition to HDMI) - required for in-car applications.
- Modules for USB Printer - now you should be able to connect USB printers to PicUntu
- Serial Modem drivers - Qualcom, NAVMAN,
- ADC
- Added Joliet/UDF support - You should be able to connect USB CDROM
- Ofcourse CIFS continues
- No it does not yet have support for MX1 wifi, RK802III/s - wifi - I am still hunting for the source.

=======
picuntu-3.0.8-alok
==================

Integrated kernel for Picuntu 3.0.8-alok+ 
>>>>>>> 984b34e9ef628f30d34c036ee9aedc1734caefcd
