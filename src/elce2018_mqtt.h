/*
 * Copyright (c) 2018 Foundries.io
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ELCE 2018 Cloud Connected Hackathon MQTT public header
 */

#ifndef ELCE2018_MQTT_H__
#define ELCE2018_MQTT_H__

/**
 * @brief Start the background MQTT thread
 *
 * This thread will periodically attempt to publish temperature data
 * to an MQTT broker.
 *
 * @return 0 if the thread is started successfully, and a negative
 *         errno on error.
 */
int elce2018_mqtt_start(void);

#endif	/* ELCE2018_MQTT_H__ */
