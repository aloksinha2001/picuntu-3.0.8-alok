cmd_drivers/usb/dwc_otg/dwc_otg_pcd.o := /mnt/disk2/Dev/UG802/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/arm-eabi-gcc -Wp,-MD,drivers/usb/dwc_otg/.dwc_otg_pcd.o.d  -nostdinc -isystem /mnt/disk2/Dev/UG802/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/../lib/gcc/arm-eabi/4.6.2/include -I/mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-rk30/include -Iarch/arm/plat-rk/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-dwarf2-cfi-asm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -DDEBUG -DDWC_HOST_ONLY    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(dwc_otg_pcd)"  -D"KBUILD_MODNAME=KBUILD_STR(dwc_otg)" -c -o drivers/usb/dwc_otg/dwc_otg_pcd.o drivers/usb/dwc_otg/dwc_otg_pcd.c

source_drivers/usb/dwc_otg/dwc_otg_pcd.o := drivers/usb/dwc_otg/dwc_otg_pcd.c

deps_drivers/usb/dwc_otg/dwc_otg_pcd.o := \
    $(wildcard include/config/arch/rk29.h) \
    $(wildcard include/config/arch/rk30.h) \
    $(wildcard include/config/arch/rk2928.h) \
    $(wildcard include/config/dwc/conn/en.h) \
    $(wildcard include/config/android/power.h) \

drivers/usb/dwc_otg/dwc_otg_pcd.o: $(deps_drivers/usb/dwc_otg/dwc_otg_pcd.o)

$(deps_drivers/usb/dwc_otg/dwc_otg_pcd.o):
