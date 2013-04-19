obj-$(CONFIG_FB_ROCKCHIP) += rk_fb.o rkfb_sysfs.o
obj-$(CONFIG_LCDC_RK30) += chips/rk30_lcdc.o  
obj-$(CONFIG_RGA_RK30) += rga/
obj-$(CONFIG_HDMI_RK30) += hdmi/
EXTRA_CFLAGS += -DCONFIG_FB_WORK_IPP
ifneq ($(CONFIG_UMP),)
obj-m += disp_ump.o
EXTRA_CFLAGS += -Idrivers/gpu/ump/include
endif
