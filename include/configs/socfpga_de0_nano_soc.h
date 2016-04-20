/*
 * Copyright (C) 2015 Marek Vasut <marex@denx.de>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __CONFIG_TERASIC_DE0_H__
#define __CONFIG_TERASIC_DE0_H__

#include <asm/arch/base_addr_ac5.h>

/* U-Boot Commands */
#define CONFIG_SYS_NO_FLASH
#define CONFIG_DOS_PARTITION
#define CONFIG_FAT_WRITE
#define CONFIG_HW_WATCHDOG

#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_BOOTZ
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_CMD_CACHE
#define CONFIG_CMD_DFU
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_EXT4_WRITE
#define CONFIG_CMD_FAT
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_CMD_GREPENV
#define CONFIG_CMD_MII
#define CONFIG_CMD_MMC
#define CONFIG_CMD_PING
#define CONFIG_CMD_USB
#define CONFIG_CMD_USB_MASS_STORAGE

/* Memory configurations */
#define PHYS_SDRAM_1_SIZE		0x40000000	/* 1GiB */

/* Booting Linux */
#define CONFIG_BOOTDELAY	3
#define CONFIG_BOOTFILE		"fitImage"
#define CONFIG_BOOTARGS		"console=ttyS0," __stringify(CONFIG_BAUDRATE)
#define CONFIG_BOOTCOMMAND	"run mmcboot"
#define CONFIG_LOADADDR		0x01000000
#define CONFIG_SYS_LOAD_ADDR	CONFIG_LOADADDR

/* Ethernet on SoC (EMAC) */
#if defined(CONFIG_CMD_NET)
#define CONFIG_PHY_MICREL
#define CONFIG_PHY_MICREL_KSZ9031
#endif

#define CONFIG_ENV_IS_IN_MMC

/* Extra Environment */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"console=ttyS0,115200\0" \
	"loadaddr=0x01000000\0" \
	"fdt_addr=0x07000000\0" \
	"fdt_file=soc_system.dtb\0" \
	"fdtdir=/lib/firmware/socfpga\0" \
	"rdaddr=0x07080000\0" \
	"interface=mmc\0" \
	"fpgaimage=\0" \
	"fpgadata=0x2000000\0" \
	"fpgaconfig=load ${interface} ${bootpart} ${fpgadata} ${fpgaimage};" \
	"optargs=\0" \
	"cmdline=\0" \
	"mmcdev=0\0" \
	"mmcpart=\0" \
	"mmcroot=/dev/mmcblk0p2 ro\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype} " \
		"${cmdline}\0" \
	"loadimage=load ${interface} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadrd=load ${interface} ${bootpart} ${rdaddr} ${bootdir}/${rdfile}; setenv rdsize ${filesize}\0" \
	"loadfdt=echo loading ${fdtdir}/${fdt_file} ...;  load ${interface} ${bootpart} ${fdt_addr} ${fdtdir}/${fdt_file}\0" \
	"mmcboot=${interface} dev ${mmcdev}; " \
		"if ${interface} rescan; then " \
			"echo Checking for: /boot/uEnv.txt ...;" \
			"for i in 2 3 4 5 6 7 ; do " \
				"setenv mmcpart ${i};" \
				"setenv bootpart ${mmcdev}:${mmcpart};" \
				"if test -e ${interface} ${bootpart} /boot/uEnv.txt; then " \
					"load ${interface} ${bootpart} ${loadaddr} /boot/uEnv.txt;" \
					"env import -t ${loadaddr} ${filesize};" \
					"echo Loaded environment from /boot/uEnv.txt;" \
					"if test -n ${dtb}; then " \
						"setenv fdt_file ${dtb};" \
						"echo Using: dtb=${fdt_file} ...;" \
					"fi;" \
	                                "if test -n $fpgaimage; then "	\
                                                 "echo fpgaimage= not set, skipping FPGA load...;" \
	                                 "else " \
			                         "echo loading FPGA from ${fpgaimage}:;" \
						 "run fpgaconfig;" \
                                        "fi;" \
					"echo Checking if uname_r is set in /boot/uEnv.txt...;" \
					"if test -n ${uname_r}; then " \
						"echo Running uname_boot ...;" \
						"setenv mmcroot /dev/mmcblk${mmcdev}p${mmcpart} ro;" \
						"run uname_boot;" \
					"fi;" \
				"fi;" \
			"done;" \
		"fi;\0" \
	"uname_boot="\
		"setenv bootdir /boot; " \
		"setenv bootfile vmlinuz-${uname_r}; " \
		"if test -e ${interface} ${bootpart} ${bootdir}/${bootfile}; then " \
			"echo loading ${bootdir}/${bootfile} ...; "\
			"run loadimage;" \
			"setenv fdtdir /lib/firmware/socfpga/${uname_r}; " \
			"if test -e ${interface} ${bootpart} ${fdtdir}/${fdt_file}; then " \
				"run loadfdt;" \
			"else " \
				"setenv fdtdir /lib/firmware/socfpga/; " \
				"if test -e ${interface} ${bootpart} ${fdtdir}/${fdt_file}; then " \
					"run loadfdt;" \
				"else " \
					"setenv fdtdir /boot/dtbs/${uname_r}; " \
					"if test -e ${interface} ${bootpart} ${fdtdir}/${fdt_file}; then " \
						"run loadfdt;" \
					"else " \
	                                        "setenv fdtdir /boot/dtbs/; "					\
 					        "if test -e ${interface} ${bootpart} ${fdtdir}/${fdt_file}; then " \
						      "run loadfdt;" \
					        "else " \
					              "echo no dtb found in /lib/firmware/socfpga or /boot/dtbs ...; "\
	                                        "fi;"	 \
	                                "fi;" \
				"fi;" \
			"fi; " \
			"setenv rdfile initrd.img-${uname_r}; " \
			"if test -e ${interface} ${bootpart} ${bootdir}/${rdfile}; then " \
				"echo loading ${bootdir}/${rdfile} ...; "\
				"run loadrd;" \
				"if test -n ${uuid}; then " \
					"setenv mmcroot UUID=${uuid} ro;" \
				"fi;" \
				"run mmcargs;" \
				"echo debug: [${bootargs}] ... ;" \
				"echo debug: [bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdt_addr}] ... ;" \
				"bootz ${loadaddr} ${rdaddr}:${rdsize} ${fdt_addr}; " \
			"else " \
				"run mmcargs;" \
				"echo debug: [${bootargs}] ... ;" \
				"echo debug: [bootz ${loadaddr} - ${fdt_addr}] ... ;" \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"fi;" \
		"fi;\0"


/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

#endif	/* __CONFIG_TERASIC_DE0_H__ */
