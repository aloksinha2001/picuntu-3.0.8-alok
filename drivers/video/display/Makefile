# Display drivers

display-objs				:= display-sys.o
//display-objs				:= display-sysfs.o

obj-$(CONFIG_DISPLAY_SUPPORT)		+= display.o
obj-$(CONFIG_DISPLAY_SUPPORT)	    += screen/
obj-$(CONFIG_RK610_LVDS)	    += transmitter/
obj-$(CONFIG_DISPLAY_LCD_SUPPORT)	+= lcd/
obj-$(CONFIG_HDMI_ITV)				+= hdmi/
obj-$(CONFIG_RK_VGA)				+= vga/
obj-y								+= lcd/
obj-y								+= tve/
