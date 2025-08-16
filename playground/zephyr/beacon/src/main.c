/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include "flash.h"
#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

/*
 * Set Advertisement data. Based on the Eddystone specification:
 * https://github.com/google/eddystone/blob/master/protocol-specification.md
 * https://github.com/google/eddystone/tree/master/eddystone-url
 */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	BT_DATA_BYTES(BT_DATA_SVC_DATA16,
		      0xaa, 0xfe, /* Eddystone UUID */
		      0x10, /* Eddystone-URL frame type */
		      0x00, /* Calibrated Tx power at 0m */
		      0x00, /* URL Scheme Prefix http://www. */
		      'z', 'e', 'p', 'h', 'y', 'r',
		      'p', 'r', 'o', 'j', 'e', 'c', 't',
		      0x08) /* .org */
};

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};
#define BT_LE_ADV_NCONN_CP BT_LE_ADV_PARAM(0, BT_GAP_PER_ADV_SLOW_INT_MIN, \
		BT_GAP_PER_ADV_SLOW_INT_MAX, NULL)
static void bt_ready(int err)
{


	err = bt_le_adv_start(BT_LE_ADV_NCONN_CP, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		printk("Advertising failed to start (err %d)\n", err);
		return;
	}

}

int main(void)
{
	int err;

	printk("Starting Beacon Demo\n");
	/* Put the flash into deep power down mode
	* Note. The init will fail if the system is reset without power loss.
	* I believe the init fails because the chip is still in dpd mode. I also
	* think this is why Zephyr is having issues.
	* The chip seems to stay in dpd and is OK after another reset or full power
	* cycle.
	* Since the errors are ignored I removed the checks.
	*/
	da_flash_init();
	da_flash_command(0xB9);
	da_flash_uninit();
	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
	do {
		k_msleep(1000);

	} while (1);
	return 0;
}
