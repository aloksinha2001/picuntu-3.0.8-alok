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

MODULE_ALIAS("usb:v*p*d*dc07dsc01dp01ic*isc*ip*");
MODULE_ALIAS("usb:v*p*d*dc07dsc01dp02ic*isc*ip*");
MODULE_ALIAS("usb:v*p*d*dc07dsc01dp03ic*isc*ip*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic07isc01ip01*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic07isc01ip02*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic07isc01ip03*");
MODULE_ALIAS("usb:v04B8p0202d*dc*dsc*dp*ic*isc*ip*");
