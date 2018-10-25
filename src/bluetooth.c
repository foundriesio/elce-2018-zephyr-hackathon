/*
 * Copyright (c) 2016-2017 Linaro Limited
 * Copyright (c) 2018 Foundries.io Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_MODULE_NAME elce2018_bluetooth
#define LOG_LEVEL CONFIG_ELCE2018_LOG_LEVEL

#include <logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include <zephyr/types.h>
#include <stddef.h>
#include <errno.h>
#include <zephyr.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include "bluetooth.h"

#include <init.h>
#include <logging/log_ctrl.h>
#include <misc/reboot.h>
#include <net/bt.h>
#include <net/net_if.h>
#include <net/net_mgmt.h>

#include "product_id.h"

static void set_own_bt_addr(bt_addr_le_t *addr)
{
	int i;
	u8_t tmp;

	/*
	 * Generate a static BT addr using the unique product number.
	 */
	for (i = 0; i < 4; i++) {
		tmp = (product_id_get()->number >> i * 8) & 0xff;
		addr->a.val[i] = tmp;
	}

	addr->a.val[4] = 0xe7;
/*
 * For CONFIG_NET_L2_BT_ZEP1656, the U/L bit of the BT MAC is toggled by Zephyr.
 * Later that MAC is used to create the 6LOWPAN IPv6 address.  To account for
 * that, we set 0xd6 here which toggles to 0xd4 later (matching the Linux
 * gateway configuration for bt0).
 */
#if defined(CONFIG_NET_L2_BT_ZEP1656)
	addr->a.val[5] = 0xd6;
#else
	addr->a.val[5] = 0xd4;
#endif
}

/* BT LE Connect/Disconnect callbacks */
static void set_bluetooth_led(bool state)
{
#if defined(BT_GPIO_PIN) && defined(BT_GPIO_CONTROLLER)
	struct device *gpio;

	gpio = device_get_binding(BT_GPIO_CONTROLLER);
	gpio_pin_configure(gpio, BT_GPIO_PIN, GPIO_DIR_OUT);
	gpio_pin_write(gpio, BT_GPIO_PIN, state);
#endif
}

static void connected(struct bt_conn *conn, u8_t err)
{
	if (err) {
		LOG_ERR("BT LE Connection failed: %u", err);
	} else {
		LOG_INF("BT LE Connected");
		set_bluetooth_led(1);
	}
}

static void disconnected(struct bt_conn *conn, u8_t reason)
{
	LOG_ERR("BT LE Disconnected (reason %u), rebooting!", reason);
	set_bluetooth_led(0);
	LOG_PANIC();
	sys_reboot(0);
}

static struct bt_conn_cb conn_callbacks = {
	.connected = connected,
	.disconnected = disconnected,
};

static int network_init(struct device *dev)
{
	bt_addr_le_t bt_addr;
	int ret = 0;

	/* Storage used to provide a BT MAC based on the serial number */
	LOG_INF("Setting Bluetooth MAC\n");

	memset(&bt_addr, 0, sizeof(bt_addr_le_t));
	bt_addr.type = BT_ADDR_LE_RANDOM;
	set_own_bt_addr(&bt_addr);
	ret = bt_set_id_addr(&bt_addr);
	bt_conn_cb_register(&conn_callbacks);

	return ret;
}

int bt_network_disable(void)
{
	/* TODO: use a better way to select BT interface */
	struct net_if *iface = net_if_get_default();
	int ret;

	ret = net_mgmt(NET_REQUEST_BT_DISCONNECT, iface, NULL, 0);
	if (ret < 0) {
		LOG_ERR("Disconnect failed:%d", ret);
		return ret;
	}

	ret = net_mgmt(NET_REQUEST_BT_ADVERTISE, iface, "off", 0);
	if (ret < 0) {
		LOG_ERR("Error stopping advertise:%d", ret);
		return ret;
	}

	return 0;
}

/* last priority in the POST_KERNEL init levels */
SYS_INIT(network_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
