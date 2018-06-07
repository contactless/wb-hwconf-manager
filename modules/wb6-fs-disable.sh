source "$DATADIR/modules/utils.sh"

hook_module_init() {
	sysfs_gpio_export $GPIO_RS485_FS
	sysfs_gpio_set $GPIO_RS485_FS 0
}

hook_module_deinit() {
	sysfs_gpio_direction $GPIO_RS485_FS in
	sysfs_gpio_unexport $GPIO_RS485_FS
}