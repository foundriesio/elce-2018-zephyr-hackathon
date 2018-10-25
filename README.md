# Zephyr ELCE Europe 2018 Cloud Connected Hackathon Application

Example application that provides sensor updates using MQTT via BLE
IPSP. The example application is configured to publish the
reel_board's nRF52 die temperature data by default.

# Build and Use

During the hackathon, BLE gateways will be configured to forward the
data to an MQTT broker on the cloud at mgmt.foundries.io, port 18830.

To build and install the app:

```
mkdir build && cd build
cmake -DBOARD=reel_board -GNinja ..
ninja
ninja flash
```

Then open your device's serial console; you will see messages like this:

```
***** Booting Zephyr OS zephyr-v1.13.0-1271-g8df61cb616 *****
[00:00:00.339,630] <inf> elce2018_main.main: ELCE 2018 Cloud Connected Hackathon application
[00:00:00.339,630] <inf> elce2018_main.main: Board: reel_board, DeviceID: SOME_HEX_VALUE
[00:00:00.339,630] <inf> elce2018_main.main: Running Built in Self Test (BIST)
[00:00:00.339,660] <inf> elce2018_main.main: Initializing MQTT service

[00:00:00.339,721] <inf> elce2018_mqtt_temp.init_sensor_sources: Did not find ambient temperature sensor ambient-temp
[00:00:00.339,721] <inf> elce2018_mqtt_temp.init_sensor_sources: Found die temperature sensor die-temp
[00:03:24.261,749] <inf> elce2018_bluetooth.connected: BT LE Connected
[00:03:27.612,640] <dbg> elce2018_mqtt_temp.elce2018_mqtt_connect_cb: connected
[00:03:27.613,372] <dbg> elce2018_mqtt_temp.read_temperature: die-temp: read 22.750000 C
[00:03:27.613,464] <dbg> elce2018_mqtt_temp.elce2018_mqtt_publish: topic: id/reel_board-SOME_HEX_VALUE/sensor-data/json
[00:03:27.613,464] <dbg> elce2018_mqtt_temp.elce2018_mqtt_publish: message: {"die_temp":22}
```

To fetch this data, you can subscribe to the MQTT data in the given topic.

For example, using `mosquitto_sub` (https://mosquitto.org/man/mosquitto_sub-1.html):

```
# IMPORTANT: change the topic name to match your DeviceID printed on your serial console.
$ mosquitto_sub  -h mgmt.foundries.io -p 18830 -t 'id/reel_board-SOME_HEX_VALUE/sensor-data/json'
{"die_temp":22}
[...]
```

To view your data being graphed in Grafana, load:

https://mgmt.foundries.io/mqtt/reel_board-SOME_HEX_VALUE.html

You can use any other MQTT client you like as well!

Where "SOME_HEX_VALUE" is the "DeviceID" value printed by
your board at startup.

# Hackathon Challenges

Here are some ideas for what to do during the hackathon, but feel free
to invent your own!

## Integrate Additional Sensors

Hook up the sensors on the Reel board to the MQTT sensor handler,
extend the JSON descriptors and other data structures in
elce2018_mqtt.c to handle your sensor, and send it to the cloud!
Search for the word "hackathon" in that file for hints on what you
will need to change.

You can use the following JSON keys in your sensor data (like
"die_temp" in the above example) for each sensor type supported by
Zephyr. Not all of them are supported out of the box by the
`reel_board` -- use the Zephyr documentation to figure out which ones
are!

- "accel_x": Acceleration on the X axis, in m/s^2.
- "accel_y": Acceleration on the Y axis, in m/s^2.
- "accel_z": Acceleration on the Z axis, in m/s^2.
- "accel_xyz": Acceleration on the X, Y and Z axes, as a length 3 array of m/s^2.
- "gyro_x": Angular velocity around the X axis, in radians/s.
- "gyro_y": Angular velocity around the Y axis, in radians/s.
- "gyro_z": Angular velocity around the Z axis, in radians/s.
- "gyro_xyz": Angular velocity around the X, Y and Z axes, as a length 3 array of radians/s.
- "magn_x": Magnetic field on the X axis, in Gauss.
- "magn_y": Magnetic field on the Y axis, in Gauss.
- "magn_z": Magnetic field on the Z axis, in Gauss.
- "magn_xyz": Magnetic field on the X, Y and Z axes, as a length 3 array of Gauss.
- "die_temp": Device die temperature in degrees Celsius.
- "ambient_temp": Ambient temperature in degrees Celsius.
- "press": Pressure in kilopascal.
- "prox": Proximity.  Adimensional.  A value of 1 indicates that an object is close.
- "humidity": Humidity, in percent.
- "light": Illuminance in visible spectrum, in lux.
- "ir": Illuminance in infra-red spectrum, in lux.
- "red": Illuminance in red spectrum, in lux.
- "green": Illuminance in green spectrum, in lux.
- "blue": Illuminance in blue spectrum, in lux.
- "altitude": Altitude, in meters.
- "pm_1_0": 1.0 micro-meters Particulate Matter, in ug/m^3.
- "pm_2_5": 2.5 micro-meters Particulate Matter, in ug/m^3.
- "pm_10": 10 micro-meters Particulate Matter, in ug/m^3.
- "distance": Distance. From sensor to target, in meters.
- "co2": CO2 level, in parts per million (ppm).
- "voc": VOC level, in parts per billion (ppb).
- "voltage": Voltage, in volts.
- "current": Current, in amps.
- "rotation": Angular rotation, in degrees.

You may need to format integer and fractional parts as strings for
floating point numbers.

## Add an MQTT client in Python

Use the Eclipse Paho MQTT client to connect to mgmt.foundries.io (the
port is unusual! it's 18830, not 1883).

For details on the API, see:

https://www.eclipse.org/paho/clients/python/docs/

## Control the board using MQTT

Edit the code to subscribe to a topic instead of just publishing where
you can receive commands in any format you like, and react to them.

Some ideas:

1. Display some text on the e-Ink display
2. Change the color of the LED

# Information on the Gateway

The BLE IPSP gateway used in the hackathon uses the Foundries.io Linux
microPlatform, but you are free to set up your own.

To reproduce the gateway used during the hackathon, follow the "Set Up
the IoT Gateway" steps in the "hawkBit and MQTT demonstration system"
documentation (link requires a login):

https://app.foundries.io/docs/0.31/other/hawkbit-mqtt-system.html#set-up-the-iot-gateway
