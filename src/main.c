/*
 * Copyright (c) 2017 Linaro Limited
 * Copyright (c) 2018 Foundries.io
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_MODULE_NAME elce2018_main
#define LOG_LEVEL CONFIG_ELCE2018_LOG_LEVEL

#include <logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include "app_work_queue.h"
#include "product_id.h"
#include "elce2018_mqtt.h"

void main(void)
{
	app_wq_init();

	LOG_INF("ELCE 2018 Cloud Connected Hackathon application");
	LOG_INF("Board: %s, DeviceID: %x",
		product_id_get()->name, product_id_get()->number);

	LOG_INF("Running Built in Self Test (BIST)");

	LOG_INF("Initializing MQTT service\n");
	if (elce2018_mqtt_start()) {
		LOG_ERR("MQTT init failed; aborting!");
		return;
	}

	/*
	 * From this point on, just handle work.
	 */
	app_wq_run();
}
