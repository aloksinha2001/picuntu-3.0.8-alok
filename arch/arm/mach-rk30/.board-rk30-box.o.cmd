cmd_arch/arm/mach-rk30/board-rk30-box.o := /mnt/disk2/Dev/UG802/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/arm-eabi-gcc -Wp,-MD,arch/arm/mach-rk30/.board-rk30-box.o.d  -nostdinc -isystem /mnt/disk2/Dev/UG802/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/../lib/gcc/arm-eabi/4.6.2/include -I/mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include include/generated/autoconf.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-rk30/include -Iarch/arm/plat-rk/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -marm -fno-dwarf2-cfi-asm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(board_rk30_box)"  -D"KBUILD_MODNAME=KBUILD_STR(board_rk30_box)" -c -o arch/arm/mach-rk30/board-rk30-box.o arch/arm/mach-rk30/board-rk30-box.c

source_arch/arm/mach-rk30/board-rk30-box.o := arch/arm/mach-rk30/board-rk30-box.c

deps_arch/arm/mach-rk30/board-rk30-box.o := \
    $(wildcard include/config/hdmi/rk30.h) \
    $(wildcard include/config/spim/rk29.h) \
    $(wildcard include/config/android/timed/gpio.h) \
    $(wildcard include/config/rk/remotectl.h) \
    $(wildcard include/config/three/fb/buffer.h) \
    $(wildcard include/config/box/fb/1080p.h) \
    $(wildcard include/config/video/rk29.h) \
    $(wildcard include/config/video/adv7180.h) \
    $(wildcard include/config/sensor/0.h) \
    $(wildcard include/config/sensor/iic/addr/0.h) \
    $(wildcard include/config/sensor/iic/adapter/id/0.h) \
    $(wildcard include/config/sensor/cif/index/0.h) \
    $(wildcard include/config/sensor/orientation/0.h) \
    $(wildcard include/config/sensor/power/pin/0.h) \
    $(wildcard include/config/sensor/reset/pin/0.h) \
    $(wildcard include/config/sensor/powerdn/pin/0.h) \
    $(wildcard include/config/sensor/falsh/pin/0.h) \
    $(wildcard include/config/sensor/poweractive/level/0.h) \
    $(wildcard include/config/sensor/resetactive/level/0.h) \
    $(wildcard include/config/sensor/powerdnactive/level/0.h) \
    $(wildcard include/config/sensor/flashactive/level/0.h) \
    $(wildcard include/config/sensor/qcif/fps/fixed/0.h) \
    $(wildcard include/config/sensor/240x160/fps/fixed/0.h) \
    $(wildcard include/config/sensor/qvga/fps/fixed/0.h) \
    $(wildcard include/config/sensor/cif/fps/fixed/0.h) \
    $(wildcard include/config/sensor/vga/fps/fixed/0.h) \
    $(wildcard include/config/sensor/480p/fps/fixed/0.h) \
    $(wildcard include/config/sensor/svga/fps/fixed/0.h) \
    $(wildcard include/config/sensor/720p/fps/fixed/0.h) \
    $(wildcard include/config/sensor/01.h) \
    $(wildcard include/config/sensor/iic/addr/01.h) \
    $(wildcard include/config/sensor/cif/index/01.h) \
    $(wildcard include/config/sensor/iic/adapter/id/01.h) \
    $(wildcard include/config/sensor/orientation/01.h) \
    $(wildcard include/config/sensor/power/pin/01.h) \
    $(wildcard include/config/sensor/reset/pin/01.h) \
    $(wildcard include/config/sensor/powerdn/pin/01.h) \
    $(wildcard include/config/sensor/falsh/pin/01.h) \
    $(wildcard include/config/sensor/poweractive/level/01.h) \
    $(wildcard include/config/sensor/resetactive/level/01.h) \
    $(wildcard include/config/sensor/powerdnactive/level/01.h) \
    $(wildcard include/config/sensor/flashactive/level/01.h) \
    $(wildcard include/config/sensor/qcif/fps/fixed/01.h) \
    $(wildcard include/config/sensor/240x160/fps/fixed/01.h) \
    $(wildcard include/config/sensor/qvga/fps/fixed/01.h) \
    $(wildcard include/config/sensor/cif/fps/fixed/01.h) \
    $(wildcard include/config/sensor/vga/fps/fixed/01.h) \
    $(wildcard include/config/sensor/480p/fps/fixed/01.h) \
    $(wildcard include/config/sensor/svga/fps/fixed/01.h) \
    $(wildcard include/config/sensor/720p/fps/fixed/01.h) \
    $(wildcard include/config/sensor/02.h) \
    $(wildcard include/config/sensor/iic/addr/02.h) \
    $(wildcard include/config/sensor/cif/index/02.h) \
    $(wildcard include/config/sensor/iic/adapter/id/02.h) \
    $(wildcard include/config/sensor/orientation/02.h) \
    $(wildcard include/config/sensor/power/pin/02.h) \
    $(wildcard include/config/sensor/reset/pin/02.h) \
    $(wildcard include/config/sensor/powerdn/pin/02.h) \
    $(wildcard include/config/sensor/falsh/pin/02.h) \
    $(wildcard include/config/sensor/poweractive/level/02.h) \
    $(wildcard include/config/sensor/resetactive/level/02.h) \
    $(wildcard include/config/sensor/powerdnactive/level/02.h) \
    $(wildcard include/config/sensor/flashactive/level/02.h) \
    $(wildcard include/config/sensor/qcif/fps/fixed/02.h) \
    $(wildcard include/config/sensor/240x160/fps/fixed/02.h) \
    $(wildcard include/config/sensor/qvga/fps/fixed/02.h) \
    $(wildcard include/config/sensor/cif/fps/fixed/02.h) \
    $(wildcard include/config/sensor/vga/fps/fixed/02.h) \
    $(wildcard include/config/sensor/480p/fps/fixed/02.h) \
    $(wildcard include/config/sensor/svga/fps/fixed/02.h) \
    $(wildcard include/config/sensor/720p/fps/fixed/02.h) \
    $(wildcard include/config/sensor/1.h) \
    $(wildcard include/config/sensor/iic/addr/1.h) \
    $(wildcard include/config/sensor/iic/adapter/id/1.h) \
    $(wildcard include/config/sensor/cif/index/1.h) \
    $(wildcard include/config/sensor/orientation/1.h) \
    $(wildcard include/config/sensor/power/pin/1.h) \
    $(wildcard include/config/sensor/reset/pin/1.h) \
    $(wildcard include/config/sensor/powerdn/pin/1.h) \
    $(wildcard include/config/sensor/falsh/pin/1.h) \
    $(wildcard include/config/sensor/poweractive/level/1.h) \
    $(wildcard include/config/sensor/resetactive/level/1.h) \
    $(wildcard include/config/sensor/powerdnactive/level/1.h) \
    $(wildcard include/config/sensor/flashactive/level/1.h) \
    $(wildcard include/config/sensor/qcif/fps/fixed/1.h) \
    $(wildcard include/config/sensor/240x160/fps/fixed/1.h) \
    $(wildcard include/config/sensor/qvga/fps/fixed/1.h) \
    $(wildcard include/config/sensor/cif/fps/fixed/1.h) \
    $(wildcard include/config/sensor/vga/fps/fixed/1.h) \
    $(wildcard include/config/sensor/480p/fps/fixed/1.h) \
    $(wildcard include/config/sensor/svga/fps/fixed/1.h) \
    $(wildcard include/config/sensor/720p/fps/fixed/1.h) \
    $(wildcard include/config/sensor/11.h) \
    $(wildcard include/config/sensor/iic/addr/11.h) \
    $(wildcard include/config/sensor/iic/adapter/id/11.h) \
    $(wildcard include/config/sensor/cif/index/11.h) \
    $(wildcard include/config/sensor/orientation/11.h) \
    $(wildcard include/config/sensor/power/pin/11.h) \
    $(wildcard include/config/sensor/reset/pin/11.h) \
    $(wildcard include/config/sensor/powerdn/pin/11.h) \
    $(wildcard include/config/sensor/falsh/pin/11.h) \
    $(wildcard include/config/sensor/poweractive/level/11.h) \
    $(wildcard include/config/sensor/resetactive/level/11.h) \
    $(wildcard include/config/sensor/powerdnactive/level/11.h) \
    $(wildcard include/config/sensor/flashactive/level/11.h) \
    $(wildcard include/config/sensor/qcif/fps/fixed/11.h) \
    $(wildcard include/config/sensor/240x160/fps/fixed/11.h) \
    $(wildcard include/config/sensor/qvga/fps/fixed/11.h) \
    $(wildcard include/config/sensor/cif/fps/fixed/11.h) \
    $(wildcard include/config/sensor/vga/fps/fixed/11.h) \
    $(wildcard include/config/sensor/480p/fps/fixed/11.h) \
    $(wildcard include/config/sensor/svga/fps/fixed/11.h) \
    $(wildcard include/config/sensor/720p/fps/fixed/11.h) \
    $(wildcard include/config/sensor/12.h) \
    $(wildcard include/config/sensor/iic/addr/12.h) \
    $(wildcard include/config/sensor/iic/adapter/id/12.h) \
    $(wildcard include/config/sensor/cif/index/12.h) \
    $(wildcard include/config/sensor/orientation/12.h) \
    $(wildcard include/config/sensor/power/pin/12.h) \
    $(wildcard include/config/sensor/reset/pin/12.h) \
    $(wildcard include/config/sensor/powerdn/pin/12.h) \
    $(wildcard include/config/sensor/falsh/pin/12.h) \
    $(wildcard include/config/sensor/poweractive/level/12.h) \
    $(wildcard include/config/sensor/resetactive/level/12.h) \
    $(wildcard include/config/sensor/powerdnactive/level/12.h) \
    $(wildcard include/config/sensor/flashactive/level/12.h) \
    $(wildcard include/config/sensor/qcif/fps/fixed/12.h) \
    $(wildcard include/config/sensor/240x160/fps/fixed/12.h) \
    $(wildcard include/config/sensor/qvga/fps/fixed/12.h) \
    $(wildcard include/config/sensor/cif/fps/fixed/12.h) \
    $(wildcard include/config/sensor/vga/fps/fixed/12.h) \
    $(wildcard include/config/sensor/480p/fps/fixed/12.h) \
    $(wildcard include/config/sensor/svga/fps/fixed/12.h) \
    $(wildcard include/config/sensor/720p/fps/fixed/12.h) \
    $(wildcard include/config/sensor/power/ioctl/usr.h) \
    $(wildcard include/config/sensor/reset/ioctl/usr.h) \
    $(wildcard include/config/sensor/powerdown/ioctl/usr.h) \
    $(wildcard include/config/sensor/flash/ioctl/usr.h) \
    $(wildcard include/config/touchscreen/gt8xx.h) \
    $(wildcard include/config/backlight/rk29/bl.h) \
    $(wildcard include/config/rk29/support/modem.h) \
    $(wildcard include/config/gs/mma8452.h) \
    $(wildcard include/config/gs/lis3dh.h) \
    $(wildcard include/config/compass/ak8975.h) \
    $(wildcard include/config/gyro/l3g4200d.h) \
    $(wildcard include/config/ls/cm3217.h) \
    $(wildcard include/config/fb/rockchip.h) \
    $(wildcard include/config/mach/rk30/box/pizza.h) \
    $(wildcard include/config/mach/rk30/box.h) \
    $(wildcard include/config/lcdc0/rk30.h) \
    $(wildcard include/config/lcdc1/rk30.h) \
    $(wildcard include/config/leds/gpio/platform.h) \
    $(wildcard include/config/display/key/led/control.h) \
    $(wildcard include/config/hdmi/itv.h) \
    $(wildcard include/config/rk/vga.h) \
    $(wildcard include/config/ch7025/7026/tvout/ypbpr.h) \
    $(wildcard include/config/rk610/tvout/ypbpr.h) \
    $(wildcard include/config/rk1000/tvout/ypbpr.h) \
    $(wildcard include/config/ch7025/7026/tvout/cvbs.h) \
    $(wildcard include/config/rk610/tvout/cvbs.h) \
    $(wildcard include/config/rk1000/tvout/cvbs.h) \
    $(wildcard include/config/rk/irda.h) \
    $(wildcard include/config/rk/irda/net.h) \
    $(wildcard include/config/ion.h) \
    $(wildcard include/config/sdmmc/rk29.h) \
    $(wildcard include/config/sdmmc0/rk29/write/protect.h) \
    $(wildcard include/config/sdmmc1/rk29/write/protect.h) \
    $(wildcard include/config/sdmmc0/rk29.h) \
    $(wildcard include/config/sdmmc/rk29/old.h) \
    $(wildcard include/config/sdmmc0/use/dma.h) \
    $(wildcard include/config/sdmmc1/rk29.h) \
    $(wildcard include/config/sdmmc1/use/dma.h) \
    $(wildcard include/config/use/sdmmc1/for/wifi/develop/board.h) \
    $(wildcard include/config/wifi/control/func.h) \
    $(wildcard include/config/battery/rk30/adc.h) \
    $(wildcard include/config/snd/soc/rk/hdmi/codec.h) \
    $(wildcard include/config/snd/rk/soc/hdmi.h) \
    $(wildcard include/config/rk30/pwm/regulator.h) \
    $(wildcard include/config/rk29/vmac.h) \
    $(wildcard include/config/rfkill/rk.h) \
    $(wildcard include/config/rk1000/tvout.h) \
    $(wildcard include/config/rk610/tvout.h) \
    $(wildcard include/config/ch7025/7026/tvout.h) \
    $(wildcard include/config/i2c0/rk30.h) \
    $(wildcard include/config/snd/soc/rt5631.h) \
    $(wildcard include/config/mfd/rk610.h) \
    $(wildcard include/config/rk610/hdmi.h) \
    $(wildcard include/config/snd/soc/rk610.h) \
    $(wildcard include/config/eeprom/at24c16.h) \
    $(wildcard include/config/i2c1/rk30.h) \
    $(wildcard include/config/mfd/wm831x/i2c.h) \
    $(wildcard include/config/mfd/tps65910.h) \
    $(wildcard include/config/i2c2/rk30.h) \
    $(wildcard include/config/mfd/rk1000.h) \
    $(wildcard include/config/snd/soc/rk1000.h) \
    $(wildcard include/config/sii902x.h) \
    $(wildcard include/config/i2c3/rk30.h) \
    $(wildcard include/config/i2c4/rk30.h) \
    $(wildcard include/config/i2c/gpio/rk30.h) \
    $(wildcard include/config/mfd/wm831x.h) \
    $(wildcard include/config/dual/disp/in/kernel.h) \
    $(wildcard include/config/mach/rk30/box/hotdog.h) \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  /mnt/disk2/Dev/UG802/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/../lib/gcc/arm-eabi/4.6.2/include/stdarg.h \
  include/linux/linkage.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/linkage.h \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/posix_types.h \
  include/linux/bitops.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/system.h \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/irqflags.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/hwcap.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/div64.h \
  include/linux/platform_device.h \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/suspend.h) \
    $(wildcard include/config/hibernate/callbacks.h) \
  include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  include/linux/ioport.h \
  include/linux/kobject.h \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/const.h \
  include/linux/sysfs.h \
    $(wildcard include/config/debug/lock/alloc.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/errno.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/kobject_ns.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
  include/asm-generic/atomic-long.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/spinlock_types.h \
  include/linux/rwlock_types.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/spinlock.h \
    $(wildcard include/config/thumb2/kernel.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/arm/errata/754327.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/hw_breakpoint.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_smp.h \
    $(wildcard include/config/inline/spin/lock.h) \
    $(wildcard include/config/inline/spin/lock/bh.h) \
    $(wildcard include/config/inline/spin/lock/irq.h) \
    $(wildcard include/config/inline/spin/lock/irqsave.h) \
    $(wildcard include/config/inline/spin/trylock.h) \
    $(wildcard include/config/inline/spin/trylock/bh.h) \
    $(wildcard include/config/inline/spin/unlock.h) \
    $(wildcard include/config/inline/spin/unlock/bh.h) \
    $(wildcard include/config/inline/spin/unlock/irq.h) \
    $(wildcard include/config/inline/spin/unlock/irqrestore.h) \
  include/linux/rwlock_api_smp.h \
    $(wildcard include/config/inline/read/lock.h) \
    $(wildcard include/config/inline/write/lock.h) \
    $(wildcard include/config/inline/read/lock/bh.h) \
    $(wildcard include/config/inline/write/lock/bh.h) \
    $(wildcard include/config/inline/read/lock/irq.h) \
    $(wildcard include/config/inline/write/lock/irq.h) \
    $(wildcard include/config/inline/read/lock/irqsave.h) \
    $(wildcard include/config/inline/write/lock/irqsave.h) \
    $(wildcard include/config/inline/read/trylock.h) \
    $(wildcard include/config/inline/write/trylock.h) \
    $(wildcard include/config/inline/read/unlock.h) \
    $(wildcard include/config/inline/write/unlock.h) \
    $(wildcard include/config/inline/read/unlock/bh.h) \
    $(wildcard include/config/inline/write/unlock/bh.h) \
    $(wildcard include/config/inline/read/unlock/irq.h) \
    $(wildcard include/config/inline/write/unlock/irq.h) \
    $(wildcard include/config/inline/read/unlock/irqrestore.h) \
    $(wildcard include/config/inline/write/unlock/irqrestore.h) \
  include/linux/kref.h \
  include/linux/wait.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/current.h \
  include/linux/klist.h \
  include/linux/module.h \
    $(wildcard include/config/symbol/prefix.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/debug/set/module/ronx.h) \
  include/linux/stat.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/seqlock.h \
  include/linux/math64.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/no/bootmem.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/nodemask.h \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/string.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/generated/bounds.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/page.h \
    $(wildcard include/config/cpu/copy/v3.h) \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/glue.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/memory.h \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/arm/patch/phys/virt/16bit.h) \
  arch/arm/mach-rk30/include/mach/memory.h \
  include/linux/version.h \
  arch/arm/mach-rk30/include/mach/io.h \
    $(wildcard include/config/rk/debug/uart.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/sizes.h \
  include/asm-generic/sizes.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
  include/linux/notifier.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
    $(wildcard include/config/sched/book.h) \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/smp.h \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
  include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/linux/param.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/timex.h \
  arch/arm/mach-rk30/include/mach/timex.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/sysctl.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/no/hz.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/preempt/rt.h) \
  include/linux/completion.h \
  include/linux/rcutree.h \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/elf.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/user.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/jump_label.h \
    $(wildcard include/config/jump/label.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/module.h \
    $(wildcard include/config/arm/unwind.h) \
  include/trace/events/module.h \
  include/trace/define_trace.h \
  include/linux/pm.h \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/runtime.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/device.h \
    $(wildcard include/config/dmabounce.h) \
  include/linux/pm_wakeup.h \
  include/linux/mod_devicetable.h \
  include/linux/input.h \
  include/linux/fs.h \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/bit_spinlock.h \
  include/linux/path.h \
  include/linux/radix-tree.h \
  include/linux/prio_tree.h \
  include/linux/pid.h \
  include/linux/capability.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/percpu_counter.h \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/nfs.h \
  include/linux/sunrpc/msg_prot.h \
  include/linux/inet.h \
  include/linux/fcntl.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/err.h \
  include/linux/io.h \
    $(wildcard include/config/has/ioport.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/io.h \
  include/linux/delay.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/delay.h \
  include/linux/i2c.h \
    $(wildcard include/config/i2c.h) \
    $(wildcard include/config/plat/rk.h) \
    $(wildcard include/config/i2c/boardinfo.h) \
  include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/lockup/detector.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/generic/hardirqs.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/proc/fs.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/mm/owner.h) \
  include/linux/rbtree.h \
  include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/transparent/hugepage.h) \
  include/linux/auxvec.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/auxvec.h \
  include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/mmu.h \
    $(wildcard include/config/cpu/has/asid.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/cputime.h \
  include/asm-generic/cputime.h \
  include/linux/sem.h \
  include/linux/ipc.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/ipcbuf.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/sembuf.h \
  include/linux/signal.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/signal.h \
  include/asm-generic/signal-defs.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/sigcontext.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/siginfo.h \
  include/asm-generic/siginfo.h \
  include/linux/proportions.h \
  include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  include/linux/resource.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/resource.h \
  include/asm-generic/resource.h \
  include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/timerfd.h) \
  include/linux/timerqueue.h \
  include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  include/linux/latencytop.h \
  include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/user/ns.h) \
  include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  include/linux/aio.h \
  include/linux/aio_abi.h \
  include/linux/uio.h \
  include/linux/of.h \
    $(wildcard include/config/sparc.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/of/dynamic.h) \
  include/linux/skbuff.h \
    $(wildcard include/config/nf/conntrack.h) \
    $(wildcard include/config/bridge/netfilter.h) \
    $(wildcard include/config/nf/defrag/ipv4.h) \
    $(wildcard include/config/nf/defrag/ipv6.h) \
    $(wildcard include/config/xfrm.h) \
    $(wildcard include/config/net/sched.h) \
    $(wildcard include/config/net/cls/act.h) \
    $(wildcard include/config/ipv6/ndisc/nodetype.h) \
    $(wildcard include/config/net/dma.h) \
    $(wildcard include/config/network/secmark.h) \
    $(wildcard include/config/network/phy/timestamping.h) \
  include/linux/kmemcheck.h \
  include/linux/net.h \
  include/linux/socket.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/socket.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/sockios.h \
  include/linux/sockios.h \
  include/linux/random.h \
  include/linux/irqnr.h \
  include/linux/textsearch.h \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/slab.h) \
  include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/slub/debug.h) \
  include/linux/kmemleak.h \
    $(wildcard include/config/debug/kmemleak.h) \
  include/net/checksum.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/uaccess.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/unified.h \
    $(wildcard include/config/arm/asm/unified.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/checksum.h \
  include/linux/in6.h \
  include/linux/dmaengine.h \
    $(wildcard include/config/async/tx/enable/channel/switch.h) \
    $(wildcard include/config/dma/engine.h) \
    $(wildcard include/config/async/tx/dma.h) \
  include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/have/dma/attrs.h) \
    $(wildcard include/config/need/dma/map/state.h) \
  include/linux/dma-attrs.h \
  include/linux/bug.h \
  include/linux/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/scatterlist.h \
  include/asm-generic/scatterlist.h \
    $(wildcard include/config/need/sg/dma/length.h) \
  include/linux/mm.h \
    $(wildcard include/config/ksm.h) \
    $(wildcard include/config/debug/pagealloc.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/hugetlbfs.h) \
  include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  include/linux/range.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/pgtable.h \
    $(wildcard include/config/highpte.h) \
  include/asm-generic/4level-fixup.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/proc-fns.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/glue-proc.h \
    $(wildcard include/config/cpu/arm610.h) \
    $(wildcard include/config/cpu/arm7tdmi.h) \
    $(wildcard include/config/cpu/arm710.h) \
    $(wildcard include/config/cpu/arm720t.h) \
    $(wildcard include/config/cpu/arm740t.h) \
    $(wildcard include/config/cpu/arm9tdmi.h) \
    $(wildcard include/config/cpu/arm920t.h) \
    $(wildcard include/config/cpu/arm922t.h) \
    $(wildcard include/config/cpu/arm925t.h) \
    $(wildcard include/config/cpu/arm926t.h) \
    $(wildcard include/config/cpu/arm940t.h) \
    $(wildcard include/config/cpu/arm946e.h) \
    $(wildcard include/config/cpu/arm1020.h) \
    $(wildcard include/config/cpu/arm1020e.h) \
    $(wildcard include/config/cpu/arm1022.h) \
    $(wildcard include/config/cpu/arm1026.h) \
    $(wildcard include/config/cpu/mohawk.h) \
    $(wildcard include/config/cpu/feroceon.h) \
    $(wildcard include/config/cpu/v6k.h) \
    $(wildcard include/config/cpu/v7.h) \
  arch/arm/mach-rk30/include/mach/vmalloc.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/pgtable-hwdef.h \
  include/asm-generic/pgtable.h \
  include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/s390.h) \
  include/linux/huge_mm.h \
  include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  include/linux/vm_event_item.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/dma-mapping.h \
  include/linux/dma-debug.h \
    $(wildcard include/config/dma/api/debug.h) \
  include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  include/linux/spi/spi.h \
    $(wildcard include/config/spi.h) \
  include/linux/mmc/host.h \
    $(wildcard include/config/mmc/clkgate.h) \
    $(wildcard include/config/mmc/debug.h) \
    $(wildcard include/config/leds/triggers.h) \
    $(wildcard include/config/regulator.h) \
    $(wildcard include/config/mmc/embedded/sdio.h) \
  include/linux/leds.h \
    $(wildcard include/config/leds/trigger/ide/disk.h) \
  include/linux/wakelock.h \
    $(wildcard include/config/has/wakelock.h) \
    $(wildcard include/config/wakelock/stat.h) \
  include/linux/mmc/core.h \
  include/linux/interrupt.h \
    $(wildcard include/config/irq/forced/threading.h) \
    $(wildcard include/config/generic/irq/probe.h) \
  include/linux/irqreturn.h \
  include/linux/hardirq.h \
  include/linux/ftrace_irq.h \
    $(wildcard include/config/ftrace/nmi/enter.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/hardirq.h \
    $(wildcard include/config/local/timers.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/irq.h \
  arch/arm/mach-rk30/include/mach/irqs.h \
  include/linux/irq_cpustat.h \
  include/trace/events/irq.h \
  include/linux/mmc/pm.h \
  include/linux/ion.h \
  include/linux/cpufreq.h \
    $(wildcard include/config/cpu/freq.h) \
    $(wildcard include/config/cpu/freq/gov/performance.h) \
    $(wildcard include/config/cpu/freq/default/gov/performance.h) \
    $(wildcard include/config/cpu/freq/default/gov/powersave.h) \
    $(wildcard include/config/cpu/freq/default/gov/userspace.h) \
    $(wildcard include/config/cpu/freq/default/gov/ondemand.h) \
    $(wildcard include/config/cpu/freq/default/gov/conservative.h) \
    $(wildcard include/config/cpu/freq/default/gov/interactive.h) \
    $(wildcard include/config/cpu/freq/default/gov/hotplug.h) \
  include/linux/clk.h \
  arch/arm/mach-rk30/include/mach/dvfs.h \
    $(wildcard include/config/dvfs.h) \
  arch/arm/mach-rk30/include/mach/clock.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/setup.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/mach-types.h \
  include/generated/mach-types.h \
    $(wildcard include/config/arch/ebsa110.h) \
    $(wildcard include/config/arch/rpc.h) \
    $(wildcard include/config/arch/ebsa285.h) \
    $(wildcard include/config/arch/netwinder.h) \
    $(wildcard include/config/arch/cats.h) \
    $(wildcard include/config/arch/shark.h) \
    $(wildcard include/config/sa1100/brutus.h) \
    $(wildcard include/config/arch/personal/server.h) \
    $(wildcard include/config/arch/l7200.h) \
    $(wildcard include/config/sa1100/pleb.h) \
    $(wildcard include/config/arch/integrator.h) \
    $(wildcard include/config/sa1100/h3600.h) \
    $(wildcard include/config/arch/p720t.h) \
    $(wildcard include/config/sa1100/assabet.h) \
    $(wildcard include/config/sa1100/lart.h) \
    $(wildcard include/config/sa1100/graphicsclient.h) \
    $(wildcard include/config/sa1100/xp860.h) \
    $(wildcard include/config/sa1100/cerf.h) \
    $(wildcard include/config/sa1100/nanoengine.h) \
    $(wildcard include/config/sa1100/jornada720.h) \
    $(wildcard include/config/arch/edb7211.h) \
    $(wildcard include/config/sa1100/pfs168.h) \
    $(wildcard include/config/sa1100/flexanet.h) \
    $(wildcard include/config/sa1100/simpad.h) \
    $(wildcard include/config/arch/lubbock.h) \
    $(wildcard include/config/arch/clep7212.h) \
    $(wildcard include/config/sa1100/shannon.h) \
    $(wildcard include/config/sa1100/consus.h) \
    $(wildcard include/config/arch/aaed2000.h) \
    $(wildcard include/config/arch/cdb89712.h) \
    $(wildcard include/config/sa1100/graphicsmaster.h) \
    $(wildcard include/config/sa1100/adsbitsy.h) \
    $(wildcard include/config/arch/pxa/idp.h) \
    $(wildcard include/config/sa1100/pt/system3.h) \
    $(wildcard include/config/arch/autcpu12.h) \
    $(wildcard include/config/sa1100/h3100.h) \
    $(wildcard include/config/sa1100/collie.h) \
    $(wildcard include/config/sa1100/badge4.h) \
    $(wildcard include/config/arch/fortunet.h) \
    $(wildcard include/config/arch/mx1ads.h) \
    $(wildcard include/config/arch/h7201.h) \
    $(wildcard include/config/arch/h7202.h) \
    $(wildcard include/config/arch/iq80321.h) \
    $(wildcard include/config/arch/ks8695.h) \
    $(wildcard include/config/arch/smdk2410.h) \
    $(wildcard include/config/arch/ceiva.h) \
    $(wildcard include/config/mach/voiceblue.h) \
    $(wildcard include/config/arch/h5400.h) \
    $(wildcard include/config/mach/omap/innovator.h) \
    $(wildcard include/config/arch/ixdp2400.h) \
    $(wildcard include/config/arch/ixdp2800.h) \
    $(wildcard include/config/arch/ixdp425.h) \
    $(wildcard include/config/sa1100/hackkit.h) \
    $(wildcard include/config/arch/ixcdp1100.h) \
    $(wildcard include/config/arch/at91rm9200dk.h) \
    $(wildcard include/config/arch/cintegrator.h) \
    $(wildcard include/config/arch/viper.h) \
    $(wildcard include/config/arch/adi/coyote.h) \
    $(wildcard include/config/arch/ixdp2401.h) \
    $(wildcard include/config/arch/ixdp2801.h) \
    $(wildcard include/config/arch/iq31244.h) \
    $(wildcard include/config/arch/bast.h) \
    $(wildcard include/config/arch/h1940.h) \
    $(wildcard include/config/arch/enp2611.h) \
    $(wildcard include/config/arch/s3c2440.h) \
    $(wildcard include/config/arch/gumstix.h) \
    $(wildcard include/config/mach/omap/h2.h) \
    $(wildcard include/config/mach/e740.h) \
    $(wildcard include/config/arch/iq80331.h) \
    $(wildcard include/config/arch/versatile/pb.h) \
    $(wildcard include/config/mach/kev7a400.h) \
    $(wildcard include/config/mach/lpd7a400.h) \
    $(wildcard include/config/mach/lpd7a404.h) \
    $(wildcard include/config/mach/csb337.h) \
    $(wildcard include/config/mach/mainstone.h) \
    $(wildcard include/config/mach/xcep.h) \
    $(wildcard include/config/mach/arcom/vulcan.h) \
    $(wildcard include/config/mach/nomadik.h) \
    $(wildcard include/config/mach/corgi.h) \
    $(wildcard include/config/mach/poodle.h) \
    $(wildcard include/config/mach/armcore.h) \
    $(wildcard include/config/mach/mx31ads.h) \
    $(wildcard include/config/mach/himalaya.h) \
    $(wildcard include/config/mach/edb9312.h) \
    $(wildcard include/config/mach/omap/generic.h) \
    $(wildcard include/config/mach/edb9301.h) \
    $(wildcard include/config/mach/edb9315.h) \
    $(wildcard include/config/mach/vr1000.h) \
    $(wildcard include/config/mach/omap/perseus2.h) \
    $(wildcard include/config/mach/e800.h) \
    $(wildcard include/config/mach/e750.h) \
    $(wildcard include/config/mach/scb9328.h) \
    $(wildcard include/config/mach/omap/h3.h) \
    $(wildcard include/config/mach/omap/h4.h) \
    $(wildcard include/config/mach/omap/osk.h) \
    $(wildcard include/config/mach/tosa.h) \
    $(wildcard include/config/mach/avila.h) \
    $(wildcard include/config/mach/edb9302.h) \
    $(wildcard include/config/mach/husky.h) \
    $(wildcard include/config/mach/shepherd.h) \
    $(wildcard include/config/mach/h4700.h) \
    $(wildcard include/config/mach/rx3715.h) \
    $(wildcard include/config/mach/nslu2.h) \
    $(wildcard include/config/mach/e400.h) \
    $(wildcard include/config/mach/ixdpg425.h) \
    $(wildcard include/config/mach/versatile/ab.h) \
    $(wildcard include/config/mach/edb9307.h) \
    $(wildcard include/config/mach/kb9200.h) \
    $(wildcard include/config/mach/sx1.h) \
    $(wildcard include/config/mach/ixdp465.h) \
    $(wildcard include/config/mach/ixdp2351.h) \
    $(wildcard include/config/mach/iq80332.h) \
    $(wildcard include/config/mach/gtwx5715.h) \
    $(wildcard include/config/mach/csb637.h) \
    $(wildcard include/config/mach/n30.h) \
    $(wildcard include/config/mach/nec/mp900.h) \
    $(wildcard include/config/mach/kafa.h) \
    $(wildcard include/config/mach/ts72xx.h) \
    $(wildcard include/config/mach/otom.h) \
    $(wildcard include/config/mach/nexcoder/2440.h) \
    $(wildcard include/config/mach/eco920.h) \
    $(wildcard include/config/mach/roadrunner.h) \
    $(wildcard include/config/mach/at91rm9200ek.h) \
    $(wildcard include/config/mach/spitz.h) \
    $(wildcard include/config/mach/adssphere.h) \
    $(wildcard include/config/mach/colibri.h) \
    $(wildcard include/config/mach/gateway7001.h) \
    $(wildcard include/config/mach/pcm027.h) \
    $(wildcard include/config/mach/anubis.h) \
    $(wildcard include/config/mach/akita.h) \
    $(wildcard include/config/mach/e330.h) \
    $(wildcard include/config/mach/nokia770.h) \
    $(wildcard include/config/mach/carmeva.h) \
    $(wildcard include/config/mach/edb9315a.h) \
    $(wildcard include/config/mach/stargate2.h) \
    $(wildcard include/config/mach/intelmote2.h) \
    $(wildcard include/config/mach/trizeps4.h) \
    $(wildcard include/config/mach/pnx4008.h) \
    $(wildcard include/config/mach/cpuat91.h) \
    $(wildcard include/config/mach/iq81340sc.h) \
    $(wildcard include/config/mach/iq81340mc.h) \
    $(wildcard include/config/mach/micro9.h) \
    $(wildcard include/config/mach/micro9l.h) \
    $(wildcard include/config/mach/omap/palmte.h) \
    $(wildcard include/config/mach/realview/eb.h) \
    $(wildcard include/config/mach/borzoi.h) \
    $(wildcard include/config/mach/palmld.h) \
    $(wildcard include/config/mach/ixdp28x5.h) \
    $(wildcard include/config/mach/omap/palmtt.h) \
    $(wildcard include/config/mach/arcom/zeus.h) \
    $(wildcard include/config/mach/osiris.h) \
    $(wildcard include/config/mach/palmte2.h) \
    $(wildcard include/config/mach/mx27ads.h) \
    $(wildcard include/config/mach/at91sam9261ek.h) \
    $(wildcard include/config/mach/loft.h) \
    $(wildcard include/config/mach/mx21ads.h) \
    $(wildcard include/config/mach/ams/delta.h) \
    $(wildcard include/config/mach/nas100d.h) \
    $(wildcard include/config/mach/magician.h) \
    $(wildcard include/config/mach/nxdkn.h) \
    $(wildcard include/config/mach/palmtx.h) \
    $(wildcard include/config/mach/s3c2413.h) \
    $(wildcard include/config/mach/wg302v2.h) \
    $(wildcard include/config/mach/omap/2430sdp.h) \
    $(wildcard include/config/mach/davinci/evm.h) \
    $(wildcard include/config/mach/palmz72.h) \
    $(wildcard include/config/mach/nxdb500.h) \
    $(wildcard include/config/mach/palmt5.h) \
    $(wildcard include/config/mach/palmtc.h) \
    $(wildcard include/config/mach/omap/apollon.h) \
    $(wildcard include/config/mach/ateb9200.h) \
    $(wildcard include/config/mach/n35.h) \
    $(wildcard include/config/mach/logicpd/pxa270.h) \
    $(wildcard include/config/mach/nxeb500hmi.h) \
    $(wildcard include/config/mach/espresso.h) \
    $(wildcard include/config/mach/rx1950.h) \
    $(wildcard include/config/mach/gesbc9312.h) \
    $(wildcard include/config/mach/picotux2xx.h) \
    $(wildcard include/config/mach/dsmg600.h) \
    $(wildcard include/config/mach/omap/fsample.h) \
    $(wildcard include/config/mach/snapper/cl15.h) \
    $(wildcard include/config/mach/omap/palmz71.h) \
    $(wildcard include/config/mach/smdk2412.h) \
    $(wildcard include/config/mach/smdk2413.h) \
    $(wildcard include/config/mach/aml/m5900.h) \
    $(wildcard include/config/mach/balloon3.h) \
    $(wildcard include/config/mach/ecbat91.h) \
    $(wildcard include/config/mach/onearm.h) \
    $(wildcard include/config/mach/smdk2443.h) \
    $(wildcard include/config/mach/fsg.h) \
    $(wildcard include/config/mach/at91sam9260ek.h) \
    $(wildcard include/config/mach/glantank.h) \
    $(wildcard include/config/mach/n2100.h) \
    $(wildcard include/config/mach/qt2410.h) \
    $(wildcard include/config/mach/kixrp435.h) \
    $(wildcard include/config/mach/cc9p9360dev.h) \
    $(wildcard include/config/mach/edb9302a.h) \
    $(wildcard include/config/mach/edb9307a.h) \
    $(wildcard include/config/mach/omap/3430sdp.h) \
    $(wildcard include/config/mach/vstms.h) \
    $(wildcard include/config/mach/micro9m.h) \
    $(wildcard include/config/mach/bug.h) \
    $(wildcard include/config/mach/at91sam9263ek.h) \
    $(wildcard include/config/mach/em7210.h) \
    $(wildcard include/config/mach/vpac270.h) \
    $(wildcard include/config/mach/treo680.h) \
    $(wildcard include/config/mach/zylonite.h) \
    $(wildcard include/config/mach/mx31lite.h) \
    $(wildcard include/config/mach/mioa701.h) \
    $(wildcard include/config/mach/armadillo5x0.h) \
    $(wildcard include/config/mach/cc9p9360js.h) \
    $(wildcard include/config/mach/smdk6400.h) \
    $(wildcard include/config/mach/nokia/n800.h) \
    $(wildcard include/config/mach/ep80219.h) \
    $(wildcard include/config/mach/goramo/mlr.h) \
    $(wildcard include/config/mach/em/x270.h) \
    $(wildcard include/config/mach/neo1973/gta02.h) \
    $(wildcard include/config/mach/at91sam9rlek.h) \
    $(wildcard include/config/mach/colibri320.h) \
    $(wildcard include/config/mach/cam60.h) \
    $(wildcard include/config/mach/at91eb01.h) \
    $(wildcard include/config/mach/db88f5281.h) \
    $(wildcard include/config/mach/csb726.h) \
    $(wildcard include/config/mach/davinci/dm6467/evm.h) \
    $(wildcard include/config/mach/davinci/dm355/evm.h) \
    $(wildcard include/config/mach/littleton.h) \
    $(wildcard include/config/mach/realview/pb11mp.h) \
    $(wildcard include/config/mach/mx27/3ds.h) \
    $(wildcard include/config/mach/halibut.h) \
    $(wildcard include/config/mach/trout.h) \
    $(wildcard include/config/mach/tct/hammer.h) \
    $(wildcard include/config/mach/herald.h) \
    $(wildcard include/config/mach/sim/one.h) \
    $(wildcard include/config/mach/jive.h) \
    $(wildcard include/config/mach/sam9/l9260.h) \
    $(wildcard include/config/mach/realview/pb1176.h) \
    $(wildcard include/config/mach/yl9200.h) \
    $(wildcard include/config/mach/rd88f5182.h) \
    $(wildcard include/config/mach/kurobox/pro.h) \
    $(wildcard include/config/mach/mx31/3ds.h) \
    $(wildcard include/config/mach/qong.h) \
    $(wildcard include/config/mach/omap2evm.h) \
    $(wildcard include/config/mach/omap3evm.h) \
    $(wildcard include/config/mach/dns323.h) \
    $(wildcard include/config/mach/omap3/beagle.h) \
    $(wildcard include/config/mach/nokia/n810.h) \
    $(wildcard include/config/mach/pcm038.h) \
    $(wildcard include/config/mach/ts209.h) \
    $(wildcard include/config/mach/at91cap9adk.h) \
    $(wildcard include/config/mach/mx31moboard.h) \
    $(wildcard include/config/mach/terastation/pro2.h) \
    $(wildcard include/config/mach/linkstation/pro.h) \
    $(wildcard include/config/mach/e350.h) \
    $(wildcard include/config/mach/ts409.h) \
    $(wildcard include/config/mach/cm/x300.h) \
    $(wildcard include/config/mach/at91sam9g20ek.h) \
    $(wildcard include/config/mach/smdk6410.h) \
    $(wildcard include/config/mach/u300.h) \
    $(wildcard include/config/mach/wrt350n/v2.h) \
    $(wildcard include/config/mach/omap/ldp.h) \
    $(wildcard include/config/mach/mx35/3ds.h) \
    $(wildcard include/config/mach/neuros/osd2.h) \
    $(wildcard include/config/mach/trizeps4wl.h) \
    $(wildcard include/config/mach/ts78xx.h) \
    $(wildcard include/config/mach/sffsdr.h) \
    $(wildcard include/config/mach/pcm037.h) \
    $(wildcard include/config/mach/db88f6281/bp.h) \
    $(wildcard include/config/mach/rd88f6192/nas.h) \
    $(wildcard include/config/mach/rd88f6281.h) \
    $(wildcard include/config/mach/db78x00/bp.h) \
    $(wildcard include/config/mach/smdk2416.h) \
    $(wildcard include/config/mach/wbd111.h) \
    $(wildcard include/config/mach/mv2120.h) \
    $(wildcard include/config/mach/mx51/3ds.h) \
    $(wildcard include/config/mach/imx27lite.h) \
    $(wildcard include/config/mach/usb/a9260.h) \
    $(wildcard include/config/mach/usb/a9263.h) \
    $(wildcard include/config/mach/qil/a9260.h) \
    $(wildcard include/config/mach/kzm/arm11/01.h) \
    $(wildcard include/config/mach/nokia/n810/wimax.h) \
    $(wildcard include/config/mach/sapphire.h) \
    $(wildcard include/config/mach/stmp37xx.h) \
    $(wildcard include/config/mach/stmp378x.h) \
    $(wildcard include/config/mach/ezx/a780.h) \
    $(wildcard include/config/mach/ezx/e680.h) \
    $(wildcard include/config/mach/ezx/a1200.h) \
    $(wildcard include/config/mach/ezx/e6.h) \
    $(wildcard include/config/mach/ezx/e2.h) \
    $(wildcard include/config/mach/ezx/a910.h) \
    $(wildcard include/config/mach/edmini/v2.h) \
    $(wildcard include/config/mach/zipit2.h) \
    $(wildcard include/config/mach/omap3/pandora.h) \
    $(wildcard include/config/mach/mss2.h) \
    $(wildcard include/config/mach/lb88rc8480.h) \
    $(wildcard include/config/mach/mx25/3ds.h) \
    $(wildcard include/config/mach/omap3530/lv/som.h) \
    $(wildcard include/config/mach/davinci/da830/evm.h) \
    $(wildcard include/config/mach/at572d940hfeb.h) \
    $(wildcard include/config/mach/dove/db.h) \
    $(wildcard include/config/mach/overo.h) \
    $(wildcard include/config/mach/at2440evb.h) \
    $(wildcard include/config/mach/neocore926.h) \
    $(wildcard include/config/mach/wnr854t.h) \
    $(wildcard include/config/mach/rd88f5181l/ge.h) \
    $(wildcard include/config/mach/rd88f5181l/fxo.h) \
    $(wildcard include/config/mach/stamp9g20.h) \
    $(wildcard include/config/mach/smdkc100.h) \
    $(wildcard include/config/mach/tavorevb.h) \
    $(wildcard include/config/mach/saar.h) \
    $(wildcard include/config/mach/at91sam9m10g45ek.h) \
    $(wildcard include/config/mach/mxlads.h) \
    $(wildcard include/config/mach/linkstation/mini.h) \
    $(wildcard include/config/mach/afeb9260.h) \
    $(wildcard include/config/mach/imx27ipcam.h) \
    $(wildcard include/config/mach/rd88f6183ap/ge.h) \
    $(wildcard include/config/mach/realview/pba8.h) \
    $(wildcard include/config/mach/realview/pbx.h) \
    $(wildcard include/config/mach/micro9s.h) \
    $(wildcard include/config/mach/rut100.h) \
    $(wildcard include/config/mach/g3evm.h) \
    $(wildcard include/config/mach/w90p910evb.h) \
    $(wildcard include/config/mach/w90p950evb.h) \
    $(wildcard include/config/mach/w90n960evb.h) \
    $(wildcard include/config/mach/mv88f6281gtw/ge.h) \
    $(wildcard include/config/mach/ncp.h) \
    $(wildcard include/config/mach/davinci/dm365/evm.h) \
    $(wildcard include/config/mach/centro.h) \
    $(wildcard include/config/mach/nokia/rx51.h) \
    $(wildcard include/config/mach/omap/zoom2.h) \
    $(wildcard include/config/mach/cpuat9260.h) \
    $(wildcard include/config/mach/cpuimx27.h) \
    $(wildcard include/config/mach/acs5k.h) \
    $(wildcard include/config/mach/snapper/9260.h) \
    $(wildcard include/config/mach/dsm320.h) \
    $(wildcard include/config/mach/exeda.h) \
    $(wildcard include/config/mach/mini2440.h) \
    $(wildcard include/config/mach/colibri300.h) \
    $(wildcard include/config/mach/linkstation/ls/hgl.h) \
    $(wildcard include/config/mach/cpuat9g20.h) \
    $(wildcard include/config/mach/smdk6440.h) \
    $(wildcard include/config/mach/nas4220b.h) \
    $(wildcard include/config/mach/zylonite2.h) \
    $(wildcard include/config/mach/aspenite.h) \
    $(wildcard include/config/mach/ttc/dkb.h) \
    $(wildcard include/config/mach/pcm043.h) \
    $(wildcard include/config/mach/sheevaplug.h) \
    $(wildcard include/config/mach/avengers/lite.h) \
    $(wildcard include/config/mach/mx51/babbage.h) \
    $(wildcard include/config/mach/rd78x00/masa.h) \
    $(wildcard include/config/mach/dm355/leopard.h) \
    $(wildcard include/config/mach/ts219.h) \
    $(wildcard include/config/mach/pca100.h) \
    $(wildcard include/config/mach/davinci/da850/evm.h) \
    $(wildcard include/config/mach/at91sam9g10ek.h) \
    $(wildcard include/config/mach/omap/4430sdp.h) \
    $(wildcard include/config/mach/magx/zn5.h) \
    $(wildcard include/config/mach/btmavb101.h) \
    $(wildcard include/config/mach/btmawb101.h) \
    $(wildcard include/config/mach/omap3/torpedo.h) \
    $(wildcard include/config/mach/anw6410.h) \
    $(wildcard include/config/mach/imx27/visstrim/m10.h) \
    $(wildcard include/config/mach/portuxg20.h) \
    $(wildcard include/config/mach/smdkc110.h) \
    $(wildcard include/config/mach/omap3517evm.h) \
    $(wildcard include/config/mach/netspace/v2.h) \
    $(wildcard include/config/mach/netspace/max/v2.h) \
    $(wildcard include/config/mach/d2net/v2.h) \
    $(wildcard include/config/mach/net2big/v2.h) \
    $(wildcard include/config/mach/net5big/v2.h) \
    $(wildcard include/config/mach/inetspace/v2.h) \
    $(wildcard include/config/mach/at91sam9g45ekes.h) \
    $(wildcard include/config/mach/pc7302.h) \
    $(wildcard include/config/mach/spear600.h) \
    $(wildcard include/config/mach/spear300.h) \
    $(wildcard include/config/mach/lilly1131.h) \
    $(wildcard include/config/mach/hmt.h) \
    $(wildcard include/config/mach/vexpress.h) \
    $(wildcard include/config/mach/d2net.h) \
    $(wildcard include/config/mach/bigdisk.h) \
    $(wildcard include/config/mach/at91sam9g20ek/2mmc.h) \
    $(wildcard include/config/mach/bcmring.h) \
    $(wildcard include/config/mach/dp6xx.h) \
    $(wildcard include/config/mach/mahimahi.h) \
    $(wildcard include/config/mach/smdk6442.h) \
    $(wildcard include/config/mach/openrd/base.h) \
    $(wildcard include/config/mach/devkit8000.h) \
    $(wildcard include/config/mach/mx51/efikamx.h) \
    $(wildcard include/config/mach/cm/t35.h) \
    $(wildcard include/config/mach/net2big.h) \
    $(wildcard include/config/mach/igep0020.h) \
    $(wildcard include/config/mach/nuc932evb.h) \
    $(wildcard include/config/mach/openrd/client.h) \
    $(wildcard include/config/mach/u8500.h) \
    $(wildcard include/config/mach/mx51/efikasb.h) \
    $(wildcard include/config/mach/marvell/jasper.h) \
    $(wildcard include/config/mach/flint.h) \
    $(wildcard include/config/mach/tavorevb3.h) \
    $(wildcard include/config/mach/touchbook.h) \
    $(wildcard include/config/mach/raumfeld/rc.h) \
    $(wildcard include/config/mach/raumfeld/connector.h) \
    $(wildcard include/config/mach/raumfeld/speaker.h) \
    $(wildcard include/config/mach/tnetv107x.h) \
    $(wildcard include/config/mach/smdkv210.h) \
    $(wildcard include/config/mach/omap/zoom3.h) \
    $(wildcard include/config/mach/omap/3630sdp.h) \
    $(wildcard include/config/mach/smartq7.h) \
    $(wildcard include/config/mach/watson/efm/plugin.h) \
    $(wildcard include/config/mach/g4evm.h) \
    $(wildcard include/config/mach/omapl138/hawkboard.h) \
    $(wildcard include/config/mach/ts41x.h) \
    $(wildcard include/config/mach/phy3250.h) \
    $(wildcard include/config/mach/mini6410.h) \
    $(wildcard include/config/mach/mx28evk.h) \
    $(wildcard include/config/mach/smartq5.h) \
    $(wildcard include/config/mach/davinci/dm6467tevm.h) \
    $(wildcard include/config/mach/mxt/td60.h) \
    $(wildcard include/config/mach/riot/bei2.h) \
    $(wildcard include/config/mach/riot/x37.h) \
    $(wildcard include/config/mach/capc7117.h) \
    $(wildcard include/config/mach/icontrol.h) \
    $(wildcard include/config/mach/qsd8x50a/st1/5.h) \
    $(wildcard include/config/mach/mx23evk.h) \
    $(wildcard include/config/mach/ap4evb.h) \
    $(wildcard include/config/mach/mityomapl138.h) \
    $(wildcard include/config/mach/guruplug.h) \
    $(wildcard include/config/mach/spear310.h) \
    $(wildcard include/config/mach/spear320.h) \
    $(wildcard include/config/mach/aquila.h) \
    $(wildcard include/config/mach/esata/sheevaplug.h) \
    $(wildcard include/config/mach/msm7x30/surf.h) \
    $(wildcard include/config/mach/ea2478devkit.h) \
    $(wildcard include/config/mach/terastation/wxl.h) \
    $(wildcard include/config/mach/msm7x25/surf.h) \
    $(wildcard include/config/mach/msm7x25/ffa.h) \
    $(wildcard include/config/mach/msm7x27/surf.h) \
    $(wildcard include/config/mach/msm7x27/ffa.h) \
    $(wildcard include/config/mach/msm7x30/ffa.h) \
    $(wildcard include/config/mach/qsd8x50/surf.h) \
    $(wildcard include/config/mach/mx53/evk.h) \
    $(wildcard include/config/mach/igep0030.h) \
    $(wildcard include/config/mach/sbc3530.h) \
    $(wildcard include/config/mach/saarb.h) \
    $(wildcard include/config/mach/harmony.h) \
    $(wildcard include/config/mach/msm7x30/fluid.h) \
    $(wildcard include/config/mach/cm/t3517.h) \
    $(wildcard include/config/mach/wbd222.h) \
    $(wildcard include/config/mach/msm8x60/surf.h) \
    $(wildcard include/config/mach/msm8x60/sim.h) \
    $(wildcard include/config/mach/tcc8000/sdk.h) \
    $(wildcard include/config/mach/nanos.h) \
    $(wildcard include/config/mach/stamp9g45.h) \
    $(wildcard include/config/mach/cns3420vb.h) \
    $(wildcard include/config/mach/omap4/panda.h) \
    $(wildcard include/config/mach/ti8168evm.h) \
    $(wildcard include/config/mach/teton/bga.h) \
    $(wildcard include/config/mach/eukrea/cpuimx25.h) \
    $(wildcard include/config/mach/eukrea/cpuimx35.h) \
    $(wildcard include/config/mach/eukrea/cpuimx51sd.h) \
    $(wildcard include/config/mach/eukrea/cpuimx51.h) \
    $(wildcard include/config/mach/smdkc210.h) \
    $(wildcard include/config/mach/omap3/braillo.h) \
    $(wildcard include/config/mach/spyplug.h) \
    $(wildcard include/config/mach/ginger.h) \
    $(wildcard include/config/mach/tny/t3530.h) \
    $(wildcard include/config/mach/pca102.h) \
    $(wildcard include/config/mach/spade.h) \
    $(wildcard include/config/mach/mxc25/topaz.h) \
    $(wildcard include/config/mach/t5325.h) \
    $(wildcard include/config/mach/gw2361.h) \
    $(wildcard include/config/mach/elog.h) \
    $(wildcard include/config/mach/income.h) \
    $(wildcard include/config/mach/bcm589x.h) \
    $(wildcard include/config/mach/etna.h) \
    $(wildcard include/config/mach/hawks.h) \
    $(wildcard include/config/mach/meson.h) \
    $(wildcard include/config/mach/xsbase255.h) \
    $(wildcard include/config/mach/pvm2030.h) \
    $(wildcard include/config/mach/mioa502.h) \
    $(wildcard include/config/mach/vvbox/sdorig2.h) \
    $(wildcard include/config/mach/vvbox/sdlite2.h) \
    $(wildcard include/config/mach/vvbox/sdpro4.h) \
    $(wildcard include/config/mach/htc/spv/m700.h) \
    $(wildcard include/config/mach/mx257sx.h) \
    $(wildcard include/config/mach/goni.h) \
    $(wildcard include/config/mach/msm8x55/svlte/ffa.h) \
    $(wildcard include/config/mach/msm8x55/svlte/surf.h) \
    $(wildcard include/config/mach/quickstep.h) \
    $(wildcard include/config/mach/dmw96.h) \
    $(wildcard include/config/mach/hammerhead.h) \
    $(wildcard include/config/mach/trident.h) \
    $(wildcard include/config/mach/lightning.h) \
    $(wildcard include/config/mach/iconnect.h) \
    $(wildcard include/config/mach/autobot.h) \
    $(wildcard include/config/mach/coconut.h) \
    $(wildcard include/config/mach/durian.h) \
    $(wildcard include/config/mach/cayenne.h) \
    $(wildcard include/config/mach/fuji.h) \
    $(wildcard include/config/mach/synology/6282.h) \
    $(wildcard include/config/mach/em1sy.h) \
    $(wildcard include/config/mach/m502.h) \
    $(wildcard include/config/mach/matrix518.h) \
    $(wildcard include/config/mach/tiny/gurnard.h) \
    $(wildcard include/config/mach/spear1310.h) \
    $(wildcard include/config/mach/bv07.h) \
    $(wildcard include/config/mach/mxt/td61.h) \
    $(wildcard include/config/mach/openrd/ultimate.h) \
    $(wildcard include/config/mach/devixp.h) \
    $(wildcard include/config/mach/miccpt.h) \
    $(wildcard include/config/mach/mic256.h) \
    $(wildcard include/config/mach/as1167.h) \
    $(wildcard include/config/mach/omap3/ibiza.h) \
    $(wildcard include/config/mach/u5500.h) \
    $(wildcard include/config/mach/davinci/picto.h) \
    $(wildcard include/config/mach/mecha.h) \
    $(wildcard include/config/mach/bubba3.h) \
    $(wildcard include/config/mach/pupitre.h) \
    $(wildcard include/config/mach/tegra/vogue.h) \
    $(wildcard include/config/mach/tegra/e1165.h) \
    $(wildcard include/config/mach/simplenet.h) \
    $(wildcard include/config/mach/ec4350tbm.h) \
    $(wildcard include/config/mach/pec/tc.h) \
    $(wildcard include/config/mach/pec/hc2.h) \
    $(wildcard include/config/mach/esl/mobilis/a.h) \
    $(wildcard include/config/mach/esl/mobilis/b.h) \
    $(wildcard include/config/mach/esl/wave/a.h) \
    $(wildcard include/config/mach/esl/wave/b.h) \
    $(wildcard include/config/mach/unisense/mmm.h) \
    $(wildcard include/config/mach/blueshark.h) \
    $(wildcard include/config/mach/e10.h) \
    $(wildcard include/config/mach/app3k/robin.h) \
    $(wildcard include/config/mach/pov15hd.h) \
    $(wildcard include/config/mach/stella.h) \
    $(wildcard include/config/mach/linkstation/lschl.h) \
    $(wildcard include/config/mach/netwalker.h) \
    $(wildcard include/config/mach/acsx106.h) \
    $(wildcard include/config/mach/atlas5/c1.h) \
    $(wildcard include/config/mach/nsb3ast.h) \
    $(wildcard include/config/mach/gnet/slc.h) \
    $(wildcard include/config/mach/af4000.h) \
    $(wildcard include/config/mach/ark9431.h) \
    $(wildcard include/config/mach/fs/s5pc100.h) \
    $(wildcard include/config/mach/omap3505nova8.h) \
    $(wildcard include/config/mach/omap3621/edp1.h) \
    $(wildcard include/config/mach/oratisaes.h) \
    $(wildcard include/config/mach/smdkv310.h) \
    $(wildcard include/config/mach/siemens/l0.h) \
    $(wildcard include/config/mach/ventana.h) \
    $(wildcard include/config/mach/wm8505/7in/netbook.h) \
    $(wildcard include/config/mach/rk29.h) \
    $(wildcard include/config/arch/rk30.h) \
    $(wildcard include/config/mach/mimas.h) \
    $(wildcard include/config/mach/titan.h) \
    $(wildcard include/config/mach/craneboard.h) \
    $(wildcard include/config/mach/es2440.h) \
    $(wildcard include/config/mach/najay/a9263.h) \
    $(wildcard include/config/mach/htctornado.h) \
    $(wildcard include/config/mach/dimm/mx257.h) \
    $(wildcard include/config/mach/jigen.h) \
    $(wildcard include/config/mach/smdk6450.h) \
    $(wildcard include/config/mach/meno/qng.h) \
    $(wildcard include/config/mach/ns2416.h) \
    $(wildcard include/config/mach/rpc353.h) \
    $(wildcard include/config/mach/tq6410.h) \
    $(wildcard include/config/mach/sky6410.h) \
    $(wildcard include/config/mach/dynasty.h) \
    $(wildcard include/config/mach/vivo.h) \
    $(wildcard include/config/mach/bury/bl7582.h) \
    $(wildcard include/config/mach/bury/bps5270.h) \
    $(wildcard include/config/mach/basi.h) \
    $(wildcard include/config/mach/tn200.h) \
    $(wildcard include/config/mach/c2mmi.h) \
    $(wildcard include/config/mach/meson/6236m.h) \
    $(wildcard include/config/mach/meson/8626m.h) \
    $(wildcard include/config/mach/tube.h) \
    $(wildcard include/config/mach/messina.h) \
    $(wildcard include/config/mach/mx50/arm2.h) \
    $(wildcard include/config/mach/cetus9263.h) \
    $(wildcard include/config/mach/brownstone.h) \
    $(wildcard include/config/mach/vmx25.h) \
    $(wildcard include/config/mach/vmx51.h) \
    $(wildcard include/config/mach/abacus.h) \
    $(wildcard include/config/mach/cm4745.h) \
    $(wildcard include/config/mach/oratislink.h) \
    $(wildcard include/config/mach/davinci/dm365/dvr.h) \
    $(wildcard include/config/mach/netviz.h) \
    $(wildcard include/config/mach/flexibity.h) \
    $(wildcard include/config/mach/wlan/computer.h) \
    $(wildcard include/config/mach/lpc24xx.h) \
    $(wildcard include/config/mach/spica.h) \
    $(wildcard include/config/mach/gpsdisplay.h) \
    $(wildcard include/config/mach/bipnet.h) \
    $(wildcard include/config/mach/overo/ctu/inertial.h) \
    $(wildcard include/config/mach/davinci/dm355/mmm.h) \
    $(wildcard include/config/mach/pc9260/v2.h) \
    $(wildcard include/config/mach/ptx7545.h) \
    $(wildcard include/config/mach/tm/efdc.h) \
    $(wildcard include/config/mach/omap3/waldo1.h) \
    $(wildcard include/config/mach/flyer.h) \
    $(wildcard include/config/mach/tornado3240.h) \
    $(wildcard include/config/mach/soli/01.h) \
    $(wildcard include/config/mach/omapl138/europalc.h) \
    $(wildcard include/config/mach/helios/v1.h) \
    $(wildcard include/config/mach/netspace/lite/v2.h) \
    $(wildcard include/config/mach/ssc.h) \
    $(wildcard include/config/mach/premierwave/en.h) \
    $(wildcard include/config/mach/wasabi.h) \
    $(wildcard include/config/mach/mx50/rdp.h) \
    $(wildcard include/config/mach/universal/c210.h) \
    $(wildcard include/config/mach/real6410.h) \
    $(wildcard include/config/mach/spx/sakura.h) \
    $(wildcard include/config/mach/ij3k/2440.h) \
    $(wildcard include/config/mach/omap3/bc10.h) \
    $(wildcard include/config/mach/thebe.h) \
    $(wildcard include/config/mach/rv082.h) \
    $(wildcard include/config/mach/armlguest.h) \
    $(wildcard include/config/mach/tjinc1000.h) \
    $(wildcard include/config/mach/dockstar.h) \
    $(wildcard include/config/mach/ax8008.h) \
    $(wildcard include/config/mach/gnet/sgce.h) \
    $(wildcard include/config/mach/pxwnas/500/1000.h) \
    $(wildcard include/config/mach/ea20.h) \
    $(wildcard include/config/mach/awm2.h) \
    $(wildcard include/config/mach/ti8148evm.h) \
    $(wildcard include/config/mach/seaboard.h) \
    $(wildcard include/config/mach/linkstation/chlv2.h) \
    $(wildcard include/config/mach/tera/pro2/rack.h) \
    $(wildcard include/config/mach/rubys.h) \
    $(wildcard include/config/mach/aquarius.h) \
    $(wildcard include/config/mach/mx53/ard.h) \
    $(wildcard include/config/mach/mx53/smd.h) \
    $(wildcard include/config/mach/lswxl.h) \
    $(wildcard include/config/mach/dove/avng/v3.h) \
    $(wildcard include/config/mach/sdi/ess/9263.h) \
    $(wildcard include/config/mach/jocpu550.h) \
    $(wildcard include/config/mach/msm8x60/rumi3.h) \
    $(wildcard include/config/mach/msm8x60/ffa.h) \
    $(wildcard include/config/mach/yanomami.h) \
    $(wildcard include/config/mach/gta04.h) \
    $(wildcard include/config/mach/cm/a510.h) \
    $(wildcard include/config/mach/omap3/rfs200.h) \
    $(wildcard include/config/mach/kx33xx.h) \
    $(wildcard include/config/mach/ptx7510.h) \
    $(wildcard include/config/mach/top9000.h) \
    $(wildcard include/config/mach/teenote.h) \
    $(wildcard include/config/mach/ts3.h) \
    $(wildcard include/config/mach/a0.h) \
    $(wildcard include/config/mach/fsm9xxx/surf.h) \
    $(wildcard include/config/mach/fsm9xxx/ffa.h) \
    $(wildcard include/config/mach/frrhwcdma60w.h) \
    $(wildcard include/config/mach/remus.h) \
    $(wildcard include/config/mach/at91cap7xdk.h) \
    $(wildcard include/config/mach/at91cap7stk.h) \
    $(wildcard include/config/mach/kt/sbc/sam9/1.h) \
    $(wildcard include/config/mach/armada/xp/db.h) \
    $(wildcard include/config/mach/spdm.h) \
    $(wildcard include/config/mach/gtib.h) \
    $(wildcard include/config/mach/dgm3240.h) \
    $(wildcard include/config/mach/htcmega.h) \
    $(wildcard include/config/mach/tricorder.h) \
    $(wildcard include/config/mach/tx28.h) \
    $(wildcard include/config/mach/bstbrd.h) \
    $(wildcard include/config/mach/pwb3090.h) \
    $(wildcard include/config/mach/idea6410.h) \
    $(wildcard include/config/mach/qbc9263.h) \
    $(wildcard include/config/mach/borabora.h) \
    $(wildcard include/config/mach/valdez.h) \
    $(wildcard include/config/mach/ls9g20.h) \
    $(wildcard include/config/mach/mios/v1.h) \
    $(wildcard include/config/mach/s5pc110/crespo.h) \
    $(wildcard include/config/mach/controltek9g20.h) \
    $(wildcard include/config/mach/tin307.h) \
    $(wildcard include/config/mach/tin510.h) \
    $(wildcard include/config/mach/bluecheese.h) \
    $(wildcard include/config/mach/tem3x30.h) \
    $(wildcard include/config/mach/harvest/desoto.h) \
    $(wildcard include/config/mach/msm8x60/qrdc.h) \
    $(wildcard include/config/mach/spear900.h) \
    $(wildcard include/config/mach/pcontrol/g20.h) \
    $(wildcard include/config/mach/rdstor.h) \
    $(wildcard include/config/mach/usdloader.h) \
    $(wildcard include/config/mach/tsoploader.h) \
    $(wildcard include/config/mach/kronos.h) \
    $(wildcard include/config/mach/ffcore.h) \
    $(wildcard include/config/mach/mone.h) \
    $(wildcard include/config/mach/unit2s.h) \
    $(wildcard include/config/mach/acer/a5.h) \
    $(wildcard include/config/mach/etherpro/isp.h) \
    $(wildcard include/config/mach/stretchs7000.h) \
    $(wildcard include/config/mach/p87/smartsim.h) \
    $(wildcard include/config/mach/tulip.h) \
    $(wildcard include/config/mach/sunflower.h) \
    $(wildcard include/config/mach/rib.h) \
    $(wildcard include/config/mach/clod.h) \
    $(wildcard include/config/mach/rump.h) \
    $(wildcard include/config/mach/tenderloin.h) \
    $(wildcard include/config/mach/shortloin.h) \
    $(wildcard include/config/mach/antares.h) \
    $(wildcard include/config/mach/wb40n.h) \
    $(wildcard include/config/mach/herring.h) \
    $(wildcard include/config/mach/naxy400.h) \
    $(wildcard include/config/mach/naxy1200.h) \
    $(wildcard include/config/mach/vpr200.h) \
    $(wildcard include/config/mach/bug20.h) \
    $(wildcard include/config/mach/goflexnet.h) \
    $(wildcard include/config/mach/torbreck.h) \
    $(wildcard include/config/mach/saarb/mg1.h) \
    $(wildcard include/config/mach/callisto.h) \
    $(wildcard include/config/mach/multhsu.h) \
    $(wildcard include/config/mach/saluda.h) \
    $(wildcard include/config/mach/pemp/omap3/apollo.h) \
    $(wildcard include/config/mach/vc0718.h) \
    $(wildcard include/config/mach/mvblx.h) \
    $(wildcard include/config/mach/inhand/apeiron.h) \
    $(wildcard include/config/mach/inhand/fury.h) \
    $(wildcard include/config/mach/inhand/siren.h) \
    $(wildcard include/config/mach/hdnvp.h) \
    $(wildcard include/config/mach/softwinner.h) \
    $(wildcard include/config/mach/prima2/evb.h) \
    $(wildcard include/config/mach/nas6210.h) \
    $(wildcard include/config/mach/unisdev.h) \
    $(wildcard include/config/mach/sbca11.h) \
    $(wildcard include/config/mach/saga.h) \
    $(wildcard include/config/mach/ns/k330.h) \
    $(wildcard include/config/mach/tanna.h) \
    $(wildcard include/config/mach/imate8502.h) \
    $(wildcard include/config/mach/aspen.h) \
    $(wildcard include/config/mach/daintree/cwac.h) \
    $(wildcard include/config/mach/zmx25.h) \
    $(wildcard include/config/mach/maple1.h) \
    $(wildcard include/config/mach/qsd8x72/surf.h) \
    $(wildcard include/config/mach/qsd8x72/ffa.h) \
    $(wildcard include/config/mach/abilene.h) \
    $(wildcard include/config/mach/eigen/ttr.h) \
    $(wildcard include/config/mach/iomega/ix2/200.h) \
    $(wildcard include/config/mach/coretec/vcx7400.h) \
    $(wildcard include/config/mach/santiago.h) \
    $(wildcard include/config/mach/mx257sol.h) \
    $(wildcard include/config/mach/strasbourg.h) \
    $(wildcard include/config/mach/msm8x60/fluid.h) \
    $(wildcard include/config/mach/smartqv5.h) \
    $(wildcard include/config/mach/smartqv3.h) \
    $(wildcard include/config/mach/smartqv7.h) \
    $(wildcard include/config/mach/paz00.h) \
    $(wildcard include/config/mach/acmenetusfoxg20.h) \
    $(wildcard include/config/mach/fwbd/0404.h) \
    $(wildcard include/config/mach/hdgu.h) \
    $(wildcard include/config/mach/pyramid.h) \
    $(wildcard include/config/mach/epiphan.h) \
    $(wildcard include/config/mach/omap/bender.h) \
    $(wildcard include/config/mach/gurnard.h) \
    $(wildcard include/config/mach/gtl/it5100.h) \
    $(wildcard include/config/mach/bcm2708.h) \
    $(wildcard include/config/mach/mx51/ggc.h) \
    $(wildcard include/config/mach/sharespace.h) \
    $(wildcard include/config/mach/haba/knx/explorer.h) \
    $(wildcard include/config/mach/simtec/kirkmod.h) \
    $(wildcard include/config/mach/crux.h) \
    $(wildcard include/config/mach/mx51/bravo.h) \
    $(wildcard include/config/mach/charon.h) \
    $(wildcard include/config/mach/picocom3.h) \
    $(wildcard include/config/mach/picocom4.h) \
    $(wildcard include/config/mach/serrano.h) \
    $(wildcard include/config/mach/doubleshot.h) \
    $(wildcard include/config/mach/evsy.h) \
    $(wildcard include/config/mach/huashan.h) \
    $(wildcard include/config/mach/lausanne.h) \
    $(wildcard include/config/mach/emerald.h) \
    $(wildcard include/config/mach/tqma35.h) \
    $(wildcard include/config/mach/marvel.h) \
    $(wildcard include/config/mach/manuae.h) \
    $(wildcard include/config/mach/chacha.h) \
    $(wildcard include/config/mach/lemon.h) \
    $(wildcard include/config/mach/csc.h) \
    $(wildcard include/config/mach/gira/knxip/router.h) \
    $(wildcard include/config/mach/t20.h) \
    $(wildcard include/config/mach/hdmini.h) \
    $(wildcard include/config/mach/sciphone/g2.h) \
    $(wildcard include/config/mach/express.h) \
    $(wildcard include/config/mach/express/kt.h) \
    $(wildcard include/config/mach/maximasp.h) \
    $(wildcard include/config/mach/nitrogen/imx51.h) \
    $(wildcard include/config/mach/nitrogen/imx53.h) \
    $(wildcard include/config/mach/sunfire.h) \
    $(wildcard include/config/mach/arowana.h) \
    $(wildcard include/config/mach/tegra/daytona.h) \
    $(wildcard include/config/mach/tegra/swordfish.h) \
    $(wildcard include/config/mach/edison.h) \
    $(wildcard include/config/mach/svp8500v1.h) \
    $(wildcard include/config/mach/svp8500v2.h) \
    $(wildcard include/config/mach/svp5500.h) \
    $(wildcard include/config/mach/b5500.h) \
    $(wildcard include/config/mach/s5500.h) \
    $(wildcard include/config/mach/icon.h) \
    $(wildcard include/config/mach/elephant.h) \
    $(wildcard include/config/mach/shooter.h) \
    $(wildcard include/config/mach/spade/lte.h) \
    $(wildcard include/config/mach/philhwani.h) \
    $(wildcard include/config/mach/gsncomm.h) \
    $(wildcard include/config/mach/strasbourg/a2.h) \
    $(wildcard include/config/mach/mmm.h) \
    $(wildcard include/config/mach/davinci/dm365/bv.h) \
    $(wildcard include/config/mach/ag5evm.h) \
    $(wildcard include/config/mach/sc575plc.h) \
    $(wildcard include/config/mach/sc575ipc.h) \
    $(wildcard include/config/mach/omap3/tdm3730.h) \
    $(wildcard include/config/mach/top9000/eval.h) \
    $(wildcard include/config/mach/top9000/su.h) \
    $(wildcard include/config/mach/utm300.h) \
    $(wildcard include/config/mach/tsunagi.h) \
    $(wildcard include/config/mach/ts75xx.h) \
    $(wildcard include/config/mach/ts47xx.h) \
    $(wildcard include/config/mach/da850/k5.h) \
    $(wildcard include/config/mach/ax502.h) \
    $(wildcard include/config/mach/igep0032.h) \
    $(wildcard include/config/mach/antero.h) \
    $(wildcard include/config/mach/synergy.h) \
    $(wildcard include/config/mach/ics/if/voip.h) \
    $(wildcard include/config/mach/wlf/cragg/6410.h) \
    $(wildcard include/config/mach/punica.h) \
    $(wildcard include/config/mach/trimslice.h) \
    $(wildcard include/config/mach/mx27/wmultra.h) \
    $(wildcard include/config/mach/mackerel.h) \
    $(wildcard include/config/mach/fa9x27.h) \
    $(wildcard include/config/mach/ns2816tb.h) \
    $(wildcard include/config/mach/ns2816/ntpad.h) \
    $(wildcard include/config/mach/ns2816/ntnb.h) \
    $(wildcard include/config/mach/kaen.h) \
    $(wildcard include/config/mach/nv1000.h) \
    $(wildcard include/config/mach/nuc950ts.h) \
    $(wildcard include/config/mach/nokia/rm680.h) \
    $(wildcard include/config/mach/ast2200.h) \
    $(wildcard include/config/mach/lead.h) \
    $(wildcard include/config/mach/unino1.h) \
    $(wildcard include/config/mach/greeco.h) \
    $(wildcard include/config/mach/verdi.h) \
    $(wildcard include/config/mach/dm6446/adbox.h) \
    $(wildcard include/config/mach/quad/salsa.h) \
    $(wildcard include/config/mach/abb/gma/1/1.h) \
    $(wildcard include/config/mach/svcid.h) \
    $(wildcard include/config/mach/msm8960/sim.h) \
    $(wildcard include/config/mach/msm8960/rumi3.h) \
    $(wildcard include/config/mach/icon/g.h) \
    $(wildcard include/config/mach/mb3.h) \
    $(wildcard include/config/mach/gsia18s.h) \
    $(wildcard include/config/mach/pivicc.h) \
    $(wildcard include/config/mach/pcm048.h) \
    $(wildcard include/config/mach/dds.h) \
    $(wildcard include/config/mach/chalten/xa1.h) \
    $(wildcard include/config/mach/ts48xx.h) \
    $(wildcard include/config/mach/tonga2/tfttimer.h) \
    $(wildcard include/config/mach/whistler.h) \
    $(wildcard include/config/mach/asl/phoenix.h) \
    $(wildcard include/config/mach/at91sam9263otlite.h) \
    $(wildcard include/config/mach/ddplug.h) \
    $(wildcard include/config/mach/d2plug.h) \
    $(wildcard include/config/mach/kzm9d.h) \
    $(wildcard include/config/mach/verdi/lte.h) \
    $(wildcard include/config/mach/nanozoom.h) \
    $(wildcard include/config/mach/dm3730/som/lv.h) \
    $(wildcard include/config/mach/dm3730/torpedo.h) \
    $(wildcard include/config/mach/anchovy.h) \
    $(wildcard include/config/mach/re2rev20.h) \
    $(wildcard include/config/mach/re2rev21.h) \
    $(wildcard include/config/mach/cns21xx.h) \
    $(wildcard include/config/mach/rider.h) \
    $(wildcard include/config/mach/nsk330.h) \
    $(wildcard include/config/mach/cns2133evb.h) \
    $(wildcard include/config/mach/z3/816x/mod.h) \
    $(wildcard include/config/mach/z3/814x/mod.h) \
    $(wildcard include/config/mach/beect.h) \
    $(wildcard include/config/mach/dma/thunderbug.h) \
    $(wildcard include/config/mach/omn/at91sam9g20.h) \
    $(wildcard include/config/mach/mx25/e2s/uc.h) \
    $(wildcard include/config/mach/mione.h) \
    $(wildcard include/config/mach/top9000/tcu.h) \
    $(wildcard include/config/mach/top9000/bsl.h) \
    $(wildcard include/config/mach/kingdom.h) \
    $(wildcard include/config/mach/armadillo460.h) \
    $(wildcard include/config/mach/lq2.h) \
    $(wildcard include/config/mach/sweda/tms2.h) \
    $(wildcard include/config/mach/mx53/loco.h) \
    $(wildcard include/config/mach/acer/a8.h) \
    $(wildcard include/config/mach/acer/gauguin.h) \
    $(wildcard include/config/mach/guppy.h) \
    $(wildcard include/config/mach/mx61/ard.h) \
    $(wildcard include/config/mach/tx53.h) \
    $(wildcard include/config/mach/omapl138/case/a3.h) \
    $(wildcard include/config/mach/uemd.h) \
    $(wildcard include/config/mach/ccwmx51mut.h) \
    $(wildcard include/config/mach/rockhopper.h) \
    $(wildcard include/config/mach/nookcolor.h) \
    $(wildcard include/config/mach/hkdkc100.h) \
    $(wildcard include/config/mach/ts42xx.h) \
    $(wildcard include/config/mach/aebl.h) \
    $(wildcard include/config/mach/wario.h) \
    $(wildcard include/config/mach/gfs/spm.h) \
    $(wildcard include/config/mach/cm/t3730.h) \
    $(wildcard include/config/mach/isc3.h) \
    $(wildcard include/config/mach/rascal.h) \
    $(wildcard include/config/mach/hrefv60.h) \
    $(wildcard include/config/mach/tpt/2/0.h) \
    $(wildcard include/config/mach/splendor.h) \
    $(wildcard include/config/mach/msm8x60/qt.h) \
    $(wildcard include/config/mach/htc/hd/mini.h) \
    $(wildcard include/config/mach/athene.h) \
    $(wildcard include/config/mach/deep/r/ek/1.h) \
    $(wildcard include/config/mach/vivow/ct.h) \
    $(wildcard include/config/mach/nery/1000.h) \
    $(wildcard include/config/mach/rfl109145/ssrv.h) \
    $(wildcard include/config/mach/nmh.h) \
    $(wildcard include/config/mach/wn802t.h) \
    $(wildcard include/config/mach/dragonet.h) \
    $(wildcard include/config/mach/at91sam9263desk16l.h) \
    $(wildcard include/config/mach/bcmhana/sv.h) \
    $(wildcard include/config/mach/bcmhana/tablet.h) \
    $(wildcard include/config/mach/koi.h) \
    $(wildcard include/config/mach/ts4800.h) \
    $(wildcard include/config/mach/tqma9263.h) \
    $(wildcard include/config/mach/holiday.h) \
    $(wildcard include/config/mach/dma6410.h) \
    $(wildcard include/config/mach/pcats/overlay.h) \
    $(wildcard include/config/mach/hwgw6410.h) \
    $(wildcard include/config/mach/shenzhou.h) \
    $(wildcard include/config/mach/cwme9210.h) \
    $(wildcard include/config/mach/cwme9210js.h) \
    $(wildcard include/config/mach/pgs/sitara.h) \
    $(wildcard include/config/mach/colibri/tegra2.h) \
    $(wildcard include/config/mach/w21.h) \
    $(wildcard include/config/mach/polysat1.h) \
    $(wildcard include/config/mach/dataway.h) \
    $(wildcard include/config/mach/cobral138.h) \
    $(wildcard include/config/mach/roverpcs8.h) \
    $(wildcard include/config/mach/marvelc.h) \
    $(wildcard include/config/mach/navefihid.h) \
    $(wildcard include/config/mach/dm365/cv100.h) \
    $(wildcard include/config/mach/able.h) \
    $(wildcard include/config/mach/legacy.h) \
    $(wildcard include/config/mach/icong.h) \
    $(wildcard include/config/mach/rover/g8.h) \
    $(wildcard include/config/mach/t5388p.h) \
    $(wildcard include/config/mach/dingo.h) \
    $(wildcard include/config/mach/goflexhome.h) \
    $(wildcard include/config/mach/lanreadyfn511.h) \
    $(wildcard include/config/mach/omap3/baia.h) \
    $(wildcard include/config/mach/omap3smartdisplay.h) \
    $(wildcard include/config/mach/xilinx.h) \
    $(wildcard include/config/mach/a2f.h) \
    $(wildcard include/config/mach/sky25.h) \
    $(wildcard include/config/mach/ccmx53.h) \
    $(wildcard include/config/mach/ccmx53js.h) \
    $(wildcard include/config/mach/ccwmx53.h) \
    $(wildcard include/config/mach/ccwmx53js.h) \
    $(wildcard include/config/mach/frisms.h) \
    $(wildcard include/config/mach/msm7x27a/ffa.h) \
    $(wildcard include/config/mach/msm7x27a/surf.h) \
    $(wildcard include/config/mach/msm7x27a/rumi3.h) \
    $(wildcard include/config/mach/dimmsam9g20.h) \
    $(wildcard include/config/mach/dimm/imx28.h) \
    $(wildcard include/config/mach/amk/a4.h) \
    $(wildcard include/config/mach/gnet/sgme.h) \
    $(wildcard include/config/mach/shooter/u.h) \
    $(wildcard include/config/mach/vmx53.h) \
    $(wildcard include/config/mach/rhino.h) \
    $(wildcard include/config/mach/armlex4210.h) \
    $(wildcard include/config/mach/swarcoextmodem.h) \
    $(wildcard include/config/mach/snowball.h) \
    $(wildcard include/config/mach/pcm049.h) \
    $(wildcard include/config/mach/vigor.h) \
    $(wildcard include/config/mach/oslo/amundsen.h) \
    $(wildcard include/config/mach/gsl/diamond.h) \
    $(wildcard include/config/mach/cv2201.h) \
    $(wildcard include/config/mach/cv2202.h) \
    $(wildcard include/config/mach/cv2203.h) \
    $(wildcard include/config/mach/vit/ibox.h) \
    $(wildcard include/config/mach/dm6441/esp.h) \
    $(wildcard include/config/mach/at91sam9x5ek.h) \
    $(wildcard include/config/mach/libra.h) \
    $(wildcard include/config/mach/easycrrh.h) \
    $(wildcard include/config/mach/tripel.h) \
    $(wildcard include/config/mach/endian/mini.h) \
    $(wildcard include/config/mach/xilinx/ep107.h) \
    $(wildcard include/config/mach/nuri.h) \
    $(wildcard include/config/mach/janus.h) \
    $(wildcard include/config/mach/ddnas.h) \
    $(wildcard include/config/mach/tag.h) \
    $(wildcard include/config/mach/tagw.h) \
    $(wildcard include/config/mach/nitrogen/vm/imx51.h) \
    $(wildcard include/config/mach/viprinet.h) \
    $(wildcard include/config/mach/bockw.h) \
    $(wildcard include/config/mach/eva2000.h) \
    $(wildcard include/config/mach/steelyard.h) \
    $(wildcard include/config/mach/mach/sdh001.h) \
    $(wildcard include/config/mach/nsslsboard.h) \
    $(wildcard include/config/mach/geneva/b5.h) \
    $(wildcard include/config/mach/spear1340.h) \
    $(wildcard include/config/mach/rexmas.h) \
    $(wildcard include/config/mach/msm8960/cdp.h) \
    $(wildcard include/config/mach/msm8960/mdp.h) \
    $(wildcard include/config/mach/msm8960/fluid.h) \
    $(wildcard include/config/mach/msm8960/apq.h) \
    $(wildcard include/config/mach/helios/v2.h) \
    $(wildcard include/config/mach/mif10p.h) \
    $(wildcard include/config/mach/iam28.h) \
    $(wildcard include/config/mach/picasso.h) \
    $(wildcard include/config/mach/mr301a.h) \
    $(wildcard include/config/mach/notle.h) \
    $(wildcard include/config/mach/eelx2.h) \
    $(wildcard include/config/mach/moon.h) \
    $(wildcard include/config/mach/ruby.h) \
    $(wildcard include/config/mach/goldengate.h) \
    $(wildcard include/config/mach/ctbu/gen2.h) \
    $(wildcard include/config/mach/kmp/am17/01.h) \
    $(wildcard include/config/mach/wtplug.h) \
    $(wildcard include/config/mach/mx27su2.h) \
    $(wildcard include/config/mach/nb31.h) \
    $(wildcard include/config/mach/hjsdu.h) \
    $(wildcard include/config/mach/td3/rev1.h) \
    $(wildcard include/config/mach/eag/ci4000.h) \
    $(wildcard include/config/mach/net5big/nand/v2.h) \
    $(wildcard include/config/mach/cpx2.h) \
    $(wildcard include/config/mach/net2big/nand/v2.h) \
    $(wildcard include/config/mach/ecuv5.h) \
    $(wildcard include/config/mach/hsgx6d.h) \
    $(wildcard include/config/mach/dawad7.h) \
    $(wildcard include/config/mach/sam9repeater.h) \
    $(wildcard include/config/mach/gt/i5700.h) \
    $(wildcard include/config/mach/ctera/plug/c2.h) \
    $(wildcard include/config/mach/marvelct.h) \
    $(wildcard include/config/mach/ag11005.h) \
    $(wildcard include/config/mach/vangogh.h) \
    $(wildcard include/config/mach/matrix505.h) \
    $(wildcard include/config/mach/oce/nigma.h) \
    $(wildcard include/config/mach/t55.h) \
    $(wildcard include/config/mach/bio3k.h) \
    $(wildcard include/config/mach/expressct.h) \
    $(wildcard include/config/mach/cardhu.h) \
    $(wildcard include/config/mach/aruba.h) \
    $(wildcard include/config/mach/bonaire.h) \
    $(wildcard include/config/mach/nuc700evb.h) \
    $(wildcard include/config/mach/nuc710evb.h) \
    $(wildcard include/config/mach/nuc740evb.h) \
    $(wildcard include/config/mach/nuc745evb.h) \
    $(wildcard include/config/mach/transcede.h) \
    $(wildcard include/config/mach/mora.h) \
    $(wildcard include/config/mach/nda/evm.h) \
    $(wildcard include/config/mach/timu.h) \
    $(wildcard include/config/mach/expressh.h) \
    $(wildcard include/config/mach/veridis/a300.h) \
    $(wildcard include/config/mach/dm368/leopard.h) \
    $(wildcard include/config/mach/omap/mcop.h) \
    $(wildcard include/config/mach/tritip.h) \
    $(wildcard include/config/mach/sm1k.h) \
    $(wildcard include/config/mach/monch.h) \
    $(wildcard include/config/mach/curacao.h) \
    $(wildcard include/config/mach/origen.h) \
    $(wildcard include/config/mach/epc10.h) \
    $(wildcard include/config/mach/sgh/i740.h) \
    $(wildcard include/config/mach/tuna.h) \
    $(wildcard include/config/mach/mx51/tulip.h) \
    $(wildcard include/config/mach/mx51/aster7.h) \
    $(wildcard include/config/mach/acro37xbrd.h) \
    $(wildcard include/config/mach/elke.h) \
    $(wildcard include/config/mach/sbc6000x.h) \
    $(wildcard include/config/mach/r1801e.h) \
    $(wildcard include/config/mach/h1600.h) \
    $(wildcard include/config/mach/mini210.h) \
    $(wildcard include/config/mach/mini8168.h) \
    $(wildcard include/config/mach/pc7308.h) \
    $(wildcard include/config/mach/kmm2m01.h) \
    $(wildcard include/config/mach/mx51erebus.h) \
    $(wildcard include/config/mach/wm8650refboard.h) \
    $(wildcard include/config/mach/tuxrail.h) \
    $(wildcard include/config/mach/arthur.h) \
    $(wildcard include/config/mach/doorboy.h) \
    $(wildcard include/config/mach/xarina.h) \
    $(wildcard include/config/mach/roverx7.h) \
    $(wildcard include/config/mach/sdvr.h) \
    $(wildcard include/config/mach/acer/maya.h) \
    $(wildcard include/config/mach/pico.h) \
    $(wildcard include/config/mach/cwmx233.h) \
    $(wildcard include/config/mach/cwam1808.h) \
    $(wildcard include/config/mach/cwdm365.h) \
    $(wildcard include/config/mach/mx51/moray.h) \
    $(wildcard include/config/mach/thales/cbc.h) \
    $(wildcard include/config/mach/bluepoint.h) \
    $(wildcard include/config/mach/dir665.h) \
    $(wildcard include/config/mach/acmerover1.h) \
    $(wildcard include/config/mach/shooter/ct.h) \
    $(wildcard include/config/mach/bliss.h) \
    $(wildcard include/config/mach/blissc.h) \
    $(wildcard include/config/mach/thales/adc.h) \
    $(wildcard include/config/mach/ubisys/p9d/evp.h) \
    $(wildcard include/config/mach/atdgp318.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/mach/arch.h \
    $(wildcard include/config/multi/irq/handler.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/mach/map.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/mach/flash.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/hardware/gic.h \
  arch/arm/mach-rk30/include/mach/board.h \
    $(wildcard include/config/touchscreen/ft5306.h) \
    $(wildcard include/config/touchscreen/ft5306/wpx2.h) \
  arch/arm/plat-rk/include/plat/board.h \
    $(wildcard include/config/usb/gadget.h) \
    $(wildcard include/config/rk/early/printk.h) \
  include/linux/rk_screen.h \
    $(wildcard include/config/hdmi/dual/disp.h) \
  arch/arm/mach-rk30/include/mach/sram.h \
  arch/arm/plat-rk/include/plat/sram.h \
  include/linux/i2c-gpio.h \
  arch/arm/mach-rk30/include/mach/hardware.h \
  arch/arm/mach-rk30/include/mach/gpio.h \
    $(wildcard include/config/spi/fpga/gpio.h) \
    $(wildcard include/config/spi/fpga/gpio/num.h) \
    $(wildcard include/config/ioextend/tca6424.h) \
    $(wildcard include/config/gpio/wm831x.h) \
    $(wildcard include/config/gpio/wm8994.h) \
    $(wildcard include/config/gpio/wm8994/num.h) \
    $(wildcard include/config/gpio/tps65910.h) \
    $(wildcard include/config/gpio/tps65910/num.h) \
  include/asm-generic/gpio.h \
    $(wildcard include/config/gpiolib.h) \
    $(wildcard include/config/of/gpio.h) \
    $(wildcard include/config/gpio/sysfs.h) \
  arch/arm/mach-rk30/include/mach/iomux.h \
  include/linux/fb.h \
    $(wildcard include/config/mach/rk29/itv/hotdog.h) \
    $(wildcard include/config/mach/rk29/itv.h) \
    $(wildcard include/config/fb/backlight.h) \
    $(wildcard include/config/fb/deferred/io.h) \
    $(wildcard include/config/fb/tileblitting.h) \
    $(wildcard include/config/fb/foreign/endian.h) \
    $(wildcard include/config/fb/both/endian.h) \
    $(wildcard include/config/fb/big/endian.h) \
    $(wildcard include/config/fb/little/endian.h) \
  include/linux/kgdb.h \
    $(wildcard include/config/have/arch/kgdb.h) \
    $(wildcard include/config/kgdb.h) \
  include/linux/serial_8250.h \
  include/linux/serial_core.h \
    $(wildcard include/config/console/poll.h) \
    $(wildcard include/config/type.h) \
    $(wildcard include/config/irq.h) \
    $(wildcard include/config/serial/core/console.h) \
    $(wildcard include/config/hard/pps.h) \
  include/linux/serial.h \
  include/linux/circ_buf.h \
  include/linux/tty.h \
  include/linux/major.h \
  include/linux/termios.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/termios.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/termbits.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/ioctls.h \
  include/asm-generic/ioctls.h \
  include/linux/tty_driver.h \
  include/linux/cdev.h \
  include/linux/tty_ldisc.h \
  include/linux/pps_kernel.h \
    $(wildcard include/config/ntp/pps.h) \
  include/linux/pps.h \
  include/linux/sysrq.h \
    $(wildcard include/config/magic/sysrq.h) \
  include/linux/tty_flip.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/kgdb.h \
  include/linux/ptrace.h \
  include/linux/backlight.h \
  include/linux/regulator/machine.h \
  include/linux/regulator/consumer.h \
  include/linux/suspend.h \
    $(wildcard include/config/vt.h) \
    $(wildcard include/config/vt/console.h) \
  include/linux/swap.h \
    $(wildcard include/config/cgroup/mem/res/ctlr/swap.h) \
  include/linux/memcontrol.h \
    $(wildcard include/config/cgroup/mem/cont.h) \
  include/linux/cgroup.h \
  include/linux/cgroupstats.h \
  include/linux/taskstats.h \
  include/linux/prio_heap.h \
  include/linux/idr.h \
  include/linux/cgroup_subsys.h \
    $(wildcard include/config/cgroup/debug.h) \
    $(wildcard include/config/cgroup/cpuacct.h) \
    $(wildcard include/config/cgroup/device.h) \
    $(wildcard include/config/cgroup/freezer.h) \
    $(wildcard include/config/net/cls/cgroup.h) \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/cgroup/perf.h) \
  include/linux/node.h \
    $(wildcard include/config/memory/hotplug/sparse.h) \
  include/linux/sysdev.h \
  include/linux/rfkill-rk.h \
  include/linux/rfkill.h \
    $(wildcard include/config/rfkill.h) \
  include/linux/sensor-dev.h \
    $(wildcard include/config/has/earlysuspend.h) \
  include/linux/miscdevice.h \
  include/linux/earlysuspend.h \
  include/linux/display-sys.h \
  include/linux/mfd/tps65910.h \
  include/linux/gpio.h \
    $(wildcard include/config/generic/gpio.h) \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/gpio.h \
  include/linux/regulator/rk29-pwm-regulator.h \
  arch/arm/mach-rk30/../../../drivers/video/rockchip/hdmi/rk_hdmi.h \
    $(wildcard include/config/video.h) \
    $(wildcard include/config/audio.h) \
    $(wildcard include/config/none.h) \
    $(wildcard include/config/color.h) \
    $(wildcard include/config/hdcp.h) \
    $(wildcard include/config/enable.h) \
    $(wildcard include/config/disable.h) \
    $(wildcard include/config/display.h) \
  arch/arm/mach-rk30/include/mach/remotectl.h \
  arch/arm/mach-rk30/../../../drivers/media/video/rk30_camera.c \
    $(wildcard include/config/video/rk29/work/ipp.h) \
    $(wildcard include/config/video/rk.h) \
  include/media/soc_camera.h \
  include/linux/videodev2.h \
    $(wildcard include/config/video/adv/debug.h) \
  include/media/videobuf-core.h \
    $(wildcard include/config/video/rk29xx/vout.h) \
  include/linux/poll.h \
  /mnt/disk2/Dev/UG802/kernel/picuntu-3.0.8-alok/arch/arm/include/asm/poll.h \
  include/asm-generic/poll.h \
  include/media/videobuf2-core.h \
  include/media/v4l2-device.h \
    $(wildcard include/config/media/controller.h) \
  include/media/media-device.h \
  include/media/media-devnode.h \
  include/media/media-entity.h \
  include/linux/media.h \
  include/media/v4l2-subdev.h \
    $(wildcard include/config/video/v4l2/subdev/api.h) \
  include/linux/v4l2-subdev.h \
  include/linux/v4l2-mediabus.h \
  include/media/v4l2-common.h \
  include/media/v4l2-dev.h \
  include/media/v4l2-fh.h \
  include/media/v4l2-mediabus.h \
  include/linux/android_pmem.h \
    $(wildcard include/config/android/pmem.h) \
  arch/arm/mach-rk30/include/mach/rk30_camera.h \
  arch/arm/plat-rk/include/plat/rk_camera.h \
  arch/arm/mach-rk30/../../../drivers/media/video/../../../arch/arm/plat-rk/rk_camera.c \
    $(wildcard include/config/video/rkcif/work/simul/off.h) \
    $(wildcard include/config/arch/rk29.h) \
    $(wildcard include/config/video/rk29/cammem/ion.h) \
  arch/arm/mach-rk30/board-rk30-sdk-sdmmc.c \
  arch/arm/mach-rk30/board-rk30-sdk-tps65910.c \
    $(wildcard include/config/rtl8192cu.h) \
    $(wildcard include/config/rtl8188eus.h) \
    $(wildcard include/config/clk/switch/to/32k.h) \
  include/linux/i2c/twl.h \
    $(wildcard include/config/twl4030/core.h) \
    $(wildcard include/config/undef.h) \
    $(wildcard include/config/twl4030/power.h) \
    $(wildcard include/config/twl6030/power.h) \
  include/linux/input/matrix_keypad.h \

arch/arm/mach-rk30/board-rk30-box.o: $(deps_arch/arm/mach-rk30/board-rk30-box.o)

$(deps_arch/arm/mach-rk30/board-rk30-box.o):
