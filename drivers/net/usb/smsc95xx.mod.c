#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v0424p9500d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9505d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9E00d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9E01d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424pEC00d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9900d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9901d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9902d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9903d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9904d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9905d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9906d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9907d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9908d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9909d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9530d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9730d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0424p9E08d*dc*dsc*dp*ic*isc*ip*");
