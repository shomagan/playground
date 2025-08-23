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
#include <zephyr/drivers/sensor.h>
#include "battery/battery.h"


#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define BT_GAP_PER_ADV_SLOW_INT_MIN_CP 0x05dc /*1500*/
#define BT_GAP_PER_ADV_SLOW_INT_MAX_CP 0x06d6 /*1750*/
#define BT_LE_ADV_NCONN_CP BT_LE_ADV_PARAM(0, BT_GAP_PER_ADV_SLOW_INT_MIN_CP, \
		BT_GAP_PER_ADV_SLOW_INT_MAX_CP, NULL)
#define BATT_MEASUREMENT_INTERVAL_MS 30000

uint32_t heart_bit = 0;
/*
 * Set Advertisement data. Based on the Eddystone specification:
 * https://github.com/google/eddystone/blob/master/protocol-specification.md
 * https://github.com/google/eddystone/tree/master/eddystone-url
 */
uint8_t tlm_data[] = {
	0xaa, 0xfe, /* 0,1 Eddystone UUID */
	0x20, /* 2 tlm */
	0x00, /* 3 version */
	0x00, 0x00, /* 4,5 vbat */
	0x00, 0x00, /* 6,7 temp */
	0x00, 0x00, 0x00, 0x00, /* heart bit */
	0x00, 0x00, 0x00, 0x00  /* time sync */
};
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0xaa, 0xfe),
	//BT_DATA_BYTES(BT_DATA_SVC_DATA16,
	//	      0xaa, 0xfe, /* Eddystone UUID */
	//	      0x10, /* Eddystone-URL frame type */
	//	      0x00, /* Calibrated Tx power at 0m */
	//	      0x00, /* URL Scheme Prefix http://www. */
	//	      'z', 'e', 'p', 'h', 'y', 'r',
	//	      'p', 'r', 'o', 'j', 'e', 'c', 't',
	//	      0x08) /* .org */,
	BT_DATA(BT_DATA_SVC_DATA16, tlm_data, sizeof(tlm_data)),

};

/* Set Scan Response data */
static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

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
	struct sensor_value odr_attr;
	int ret;
	int64_t last_adc_measurement_time = -BATT_MEASUREMENT_INTERVAL_MS-1;
	uint16_t voltage;


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
	const struct device *accel = DEVICE_DT_GET_ONE(st_lsm6dsl);
	if (!device_is_ready(accel)) {
		printk("Sensor not ready");
	}else{
		odr_attr.val1 = 104;
		odr_attr.val2 = 0;

		if (sensor_attr_set(accel, SENSOR_CHAN_ACCEL_XYZ,
				SENSOR_ATTR_SAMPLING_FREQUENCY, &odr_attr) < 0) {
			printk("Cannot set sampling frequency for accelerometer.\n");
		}

	}

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(bt_ready);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
	}
	ret = battery_init(1/*adc_only*/);
	if (ret)	{
		printk("Failed to initialize battery management (error %d)", ret);
	}
	battery_suspend();

	do{
		k_msleep(1000);
		if (accel && device_is_ready(accel)) {
			struct sensor_value xyz[3];

			if (sensor_sample_fetch_chan(accel, SENSOR_CHAN_ACCEL_XYZ) == 0 &&
				sensor_channel_get(accel, SENSOR_CHAN_ACCEL_XYZ, xyz) == 0) {
				tlm_data[6] = (xyz[0].val1 >> 8) & 0xFF;
				tlm_data[7] = (xyz[2].val1 >> 8) & 0xFF;
			}
		}


		if(k_uptime_get() - last_adc_measurement_time > BATT_MEASUREMENT_INTERVAL_MS) {
			battery_resume();
			ret = battery_get_millivolt(&voltage);
			if (ret == 0) {
				// Use the voltage value here
				tlm_data[4] = (voltage >> 8) & 0xFF;
				tlm_data[5] = voltage & 0xFF;
			}
			last_adc_measurement_time = k_uptime_get();
			battery_suspend();
		}
		/*update advertisement tlm data*/
		tlm_data[8] = (heart_bit >> 24) & 0xFF;
		tlm_data[9] = (heart_bit >> 16) & 0xFF;
		tlm_data[10] = (heart_bit >> 8) & 0xFF;
		tlm_data[11] = heart_bit & 0xFF;
		/*update advertisement tlm data*/
		bt_le_adv_update_data(ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
		heart_bit++;
	} while (1);

	return 0;
}
