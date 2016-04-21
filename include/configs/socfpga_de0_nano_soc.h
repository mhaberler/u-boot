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
#define CONFIG_BOOTCOMMAND	"run importuenv; run mmcload; run mmcboot"
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
	"loadaddr= " __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"ramboot=setenv bootargs " CONFIG_BOOTARGS ";" \
		"bootm ${loadaddr} - ${fdt_addr}\0" \
        "mmcloadcmd=ext4load\0" \
	"interface=mmc\0" \
        "bootenv=/boot/uEnv.txt\0" \
	"bootpart=0:2\0" \
	"bootdir=/\0" \
	"bootimage=/boot/vmlinuz-3.10.37-ltsi-rt37-05841-g81c6be3\0" \
	"fdt_addr=100\0" \
	"fdtimage=/lib/firmware/socfpga/soc_system.dtb\0" \
	"fpgaimage=/lib/firmware/socfpga/soc_system.rbf\0" \
	"fpgadata=0x2000000\0" \
	"fpgaconfig=load ${interface} ${bootpart} ${fpgadata} ${bootdir}/${fpgaimage};" \
	"fpga load 0 ${fpgadata} ${filesize}\0" \
	"bootm ${loadaddr} - ${fdt_addr}\0" \
	"mmcroot=/dev/mmcblk0p2\0" \
	"mmcboot=setenv bootargs " CONFIG_BOOTARGS \
		" root=${mmcroot} rw rootwait ${extra_bootargs};" \
		" echo bootargs=${bootargs};" \
	       "bootz ${loadaddr} - ${fdt_addr}\0" \
	"loadbootenv=load  ${interface} ${bootpart} ${loadaddr} ${bootdir}/${bootenv}\0" \
	"importbootenv=echo Importing environment from ${bootdir}/${bootenv} ...; " \
		"env import -t -r $loadaddr $filesize\0" \
        "lsboot=ext4ls ${interface} ${bootpart} ${bootdir};\0"  \
        "importuenv=if run loadbootenv; then " \
		       "echo Loaded environment from ${bootenv};" \
		       "run importbootenv;" \
		"fi;\0" \
	"mmcload=mmc rescan;"		    \
                "if test -n $uenvcmd; then " \
                        "echo Running uenvcmd: $uenvcmd ...;" \
                        "run uenvcmd;" \
                "fi;" \
		"load ${interface} ${bootpart} ${fpgadata} ${bootdir}/${fpgaimage};" \
		"load ${interface} ${bootpart} ${loadaddr} ${bootdir}/${bootimage};" \
		"load ${interface} ${bootpart} ${fdt_addr} ${bootdir}/${fdtimage}\0" \


/* The rest of the configuration is shared */
#include <configs/socfpga_common.h>

#endif	/* __CONFIG_TERASIC_DE0_H__ */
