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

MODULE_ALIAS("usb:v0105p145Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3401d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3402d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3407d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3409d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v079Bp004Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07B8p6001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEp1211d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEpA211d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B05p170Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B3Bp1630d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B3Bp5630d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p9071d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p9075d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v126FpA006d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v129Bp1666d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13B1p001Ed*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1435p0711d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14EApAB10d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v14EApAB13d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v157Ep300Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v157Ep300Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v157Ep3204d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v157Ep3207d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1740p2000d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v6891pA727d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0053p5301d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0409p0248d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0411p00DAd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0471p1236d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0471p1237d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v050Dp705Cd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v054Cp0257d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p340Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p340Fd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3410d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3412d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0586p3413d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v079Bp0062d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07B8p6001d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v07FAp1196d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083Ap4505d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApE501d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApE503d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v083ApE506d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEp1215d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEpB215d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0B05p171Bd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0BAFp0121d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0CDEp001Ad*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0DF6p0036d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v129Bp1667d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v13B1p0024d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v157Ep300Dd*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1582p6003d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2019p5303d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v2019pED01d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEp2011d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v0ACEp20FFd*dc*dsc*dp*ic*isc*ip*");

MODULE_INFO(srcversion, "767ED313977C634DCEF8B5A");
