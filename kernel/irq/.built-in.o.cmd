cmd_kernel/irq/built-in.o :=  /mnt/disk2/Dev/UG802/kernel/toolchains/arm-eabi-linaro-4.6.2/bin/arm-eabi-ld.bfd -EL    -r -o kernel/irq/built-in.o kernel/irq/irqdesc.o kernel/irq/handle.o kernel/irq/manage.o kernel/irq/spurious.o kernel/irq/resend.o kernel/irq/chip.o kernel/irq/dummychip.o kernel/irq/devres.o kernel/irq/autoprobe.o kernel/irq/proc.o kernel/irq/pm.o 