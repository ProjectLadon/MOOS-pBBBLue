# MOOS-pBBBlue
An interface from MOOS to the Beaglebone Blue roboticscape library. Since the roboticscape will only run in a single process on the entire machine, it is necessarily a single process per Beaglebone Blue

## Dependencies
* MOOS-IvP
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse
* http://www.strawsondesign.com/#!manual-install -- roboticscape library, provides Beaglebone Blue interface

## Functions
Only these listed sections are planned for implementation:
* [LEDS](http://www.strawsondesign.com/#!manual-leds) -- only LED on/off shall be implemented
* [Buttons](http://www.strawsondesign.com/#!manual-buttons) -- button state shall be published, but shall have no further effect.
* [DC Motors](http://www.strawsondesign.com/#!manual-dc-motors)
* [Encoders](http://www.strawsondesign.com/#!manual-encoders) -- values shall be published only and may not be set except at startup. 
* [ADC](http://www.strawsondesign.com/#!manual-adc) -- both raw and scaled values shall be available
* [Servos & ESC](http://www.strawsondesign.com/#!manual-servos)
* [IMU](http://www.strawsondesign.com/#!manual-imu) -- both random and DMP functions shall be available, configurable at startup.
* [Barometer](http://www.strawsondesign.com/#!manual-barometer)
* [GPIO](http://www.strawsondesign.com/#!manual-gpio)
* [PWM](http://www.strawsondesign.com/#!manual-pwm)

Configuration is set by a single json object provided either in the moos file configuration block specified by ```conf``` or as a separate file specified by ```confFile```. In either case, it must conform to schema/configuration_schema.json. The contents of each sub-section of this schema and related I/O schema are described in the section of this document for each roboticscape section listed above.

In general, the configuration json defines variable names that the pBBBlue will subscribe to or publish. The types of those variables can be deduced as follows:
* If the documentation names a JSON schema for published or received data, then the MOOS type of that data is STRING. In general, all STRING data will be json encoded in order to make parsing easier.
* If the documentation indicates that the variable has two and only two possible states, then the MOOS type will be BINARY.
* Otherwise, the type will be DOUBLE. 

## [LEDS](http://www.strawsondesign.com/#!manual-leds)
In this block, you can assign named variable to each LED (RED & GREEN). If the names are present, the module subscribes to the specified BINARY variable(s) and uses them to control the named LEDs. The configuration sub-schema is as follows:
```
"LED": {
	"type": "object",
	"properties": {
		"RED" : {"oneOf": [{"type": "string"}, null]},
		"GREEN" : {"oneOf": [{"type": "string"}, null]}
	}
}
```

Setting the subscribed variable to true turns the LED on and setting it false turns the LED off. 

## [Buttons](http://www.strawsondesign.com/#!manual-buttons)
In this block, you can assign named variables to the two buttons (PAUSE and MODE). If a name is provided, pBBBlue will publish a BINARY variable with the specified name and ties it to the state of the button. The configuration sub-schema is as follows:
```
"Button": {
	"type": "object",
	"properties": {
		"PAUSE": {"oneOf": [{"type": "string"}, null]},
		"MODE": {"oneOf": [{"type": "string"}, null]}
	}
}
```

## [DC Motors](http://www.strawsondesign.com/#!manual-dc-motors)
The four motor driver channels are configured together in a single array. The array index for each element corresponds to the motor channel it controls. If the element is a null value (or missing), the channel is disabled. If it is a string, it names a STRING variable that the module will subscribe to. pBBLue expects the messages to be json conforming to the input schema described below. The configuration schema is as follows:
```
"Motor": {
	"type": "object",
	"properties": {
		"motors": {
			"type": "array",
			"items": {"oneOf" :[null, {"type": "string"}]},	
			"minItems":0,
			"maxItems":4,
		}
	},
	"required": ["motors"] 
}
```

### Subscribed Variables
* BBBL_MOTORS_ACTIVE (BINARY) -- If any of the motor channels are active, this process shall subscribe to this variable. If it true or missing, it will enable the motors; if it is false, it will disable all motors and leave them free spinning.
* BBBL_MOTORS0_FREQUENCY (DOUBLE) -- If present, sets the frequency for the lower pair of motor channels. Values in Hertz.
* BBBL_MOTORS1_FREQUENCY (DOUBLE) -- If present, sets the frequency for the upper pair of motor channels. Values in Hertz.

### Command Schema
Each motor has three possible states -- a direction/duty cycle command ranging from -1.0 to 1.0, free spinning, or braking. The command must conform to the following schema:
```
{
	"$schema": "http://json-schema.org/schema#",
	"id": "BBBlue_motor_input",
	"oneOf": [
		{
			"type": "number", 
			"minimum": "-1.0",
			"maximum": "1.0"
		},
		{"type": "string", "enum": ["FREE", "BRAKE"]}
	]
}
```

## [Encoders](http://www.strawsondesign.com/#!manual-encoders)
Encoders may be set to a given value on startup. In the absence of a given value, each channel will initialize to zero. The array index of each element in the configuration array corresponds to the channel. A null value in any entry turns off that encoder channel.
```
"Encoders": {
	"type": "array",
	"items": {
		"oneOf": [
			null,
			{"type": "string"},
			{
				"type": "object",
				"properties": {
					"name": {"type": "string"},
					"init": {"type": "number", "$comment": "initial value"}
				},
				"required":["name"]
			}
		]
	},
	"minItems": 0,
	"maxItems": 4
}
```

## [ADC](http://www.strawsondesign.com/#!manual-adc)
While the Sitara at the core of the Beaglebone has eight channels, only seven are connected. Each one is 12 bits and measures 0-1.8V. They are connected as follows:
* AIN0 -- pin 3 of ADC header
* AIN1 -- pin 4 of ADC header
* AIN2 -- pin 5 of ADC header
* AIN3 -- pin 6 of ADC header
* AIN4 -- not connected
* AIN5 -- DC jack voltage (through voltage divider)
* AIN6 -- Battery voltage (through a voltage divider)
* AIN7 -- 3.3V rail (through a voltage divider)

The battery and DC jack voltages are available as processed voltages that take into account the voltage dividers. They can be used directly, as for vehicle health monitoring. If you only want to include them, include a blank ADC object. 

```
"ADC" : {
	"type": "object",
	"properties": {
		"rawChannels": {
			"type": "array",
			"items": {"oneOf": [null, {"type": "string"}]},
			"minItems":0,
			"maxItems":8
		},
		"voltChannels": {
			"type": "array",
			"items": {"oneOf": [null, {"type": "string"}]},
			"minItems":0,
			"maxItems":4
		}
	}
}
```

### Published Variables
* BBBL_BATTERY_VOLTAGE (DOUBLE) -- Current battery voltage, in volts
* BBBL_JACK_VOLTAGE (DOUBLE) -- Voltage at the DC input jack, in volts.

## [Servos & ESC](http://www.strawsondesign.com/#!manual-servos)
The Beaglebone Blue has eight servo/ESC outputs powered by the PRU. They are configured as follows:
```
"Servos": {
	"type" : "array",
	"items": {
		"oneOf": [
			null,
			{
				"type": "object",
				"properties": {
					"name": {"type": "string"},
					"action": {
						"type": "string",
						"enum": ["servo90", "servo180", "servo360", "micros", "ESC"],
						"$comment": "servo90 designates a servo with a zero to 90 degree range",
						"$comment": "servo180 designates a servo with a zero to 180 degree range",
						"$comment": "servo360 designates a servo with a zero to 360 degree range",
						"$comment": "micros designates a servo commanded in microseconds",
						"$comment": "ESC designates an electronic speed control which accepts inputs from 0.0 to 100.0 (percent throttle)"
					},
					"period": {"type": "integer", "$comment": "Period in microseconds, default to 20000 (50 Hz)"},
					"minServo": {"type": "number", "$comment" : "Minimum bound on comand in microseconds; smaller commands will be set to this value"},
					"maxServo": {"type": "number", "$comment" : "Maximum bound on comand in microseconds; larger commands will be set to this value"},
					"center": {"type": "integer", "$comment": "Location of center position, in microseconds. Defaults to 1500"},
					"invert": {"type": "boolean", "$comment": "Reverses the direction of travel if true"}
				},
				"required": ["name", "action"]
			}
		]
	},
	"minItems":0,
	"maxItems":8
}
```

### Subscribed variables
* BBBL_SERVO_PWR (BINARY) -- If this variable is present and true, pBBBlue will turn on the 6V servo power rail. 

## [IMU](http://www.strawsondesign.com/#!manual-imu)
The IMU has two modes -- Random and DMP. 

In Random mode, the accelerometer, gyroscopes, magnetometer, and temperature can all be read at any time. Additionally, the pBBBlue process calculates the tilt-compensated compass heading of the X axis in random mode. 

In DMP mode, the sensor fusion algorithms in the IMU run at a fixed rate (set by *dmpSampleRate*) and generate both a normalized quaternion output and a set of Tait-Bryan (Euler) angles. The heading (yaw) axis of the Tait-Bryan output converges to the raw compass heading on a time constant set by *dmpCompassTimeConstant*. Minimum value is 0.1.

In both modes, it is possible to set the range of the accelerometer and gyros. Picking the smallest value that encompasses the behavior of your robot will result in more precise measurements. 

There are also low pass input filters for the gyro and accelerometer, which can be set with the *accelFilter* and *gyroFilter* properties. The setting correspond roughly to the cutoff frequency.

```
"IMU": {
	"type" : "object",
	"properties" : {
		"mode": {"type": "string", "enum": ["Random", "DMP"]},
		"magnetometer": {
			"type": "boolean", 
			"$comment": "If this is true, magnetometer is on",
			"$comment": "default is false"
		},
		"accelRange": {
			"type": "string", 
			"enum": ["2G", "4G", "8G", "16G"],
			"$comment": "default is 4G"
		},
		"gyroRange": {
			"type": "string", 
			"enum": ["250DPS", "500DPS", "1000DPS", "2000DPS"],
			"$comment": "default is 1000DPS"
		},
		"accelFilter": {
			"type": "string",
			"enum": ["OFF", "184", "92", "41", "20", "10", "5"],
			"$comment": "default is 92"
		},
		"gyroFilter": {
			"type": "string",
			"enum": ["OFF", "184", "92", "41", "20", "10", "5"],
			"$comment": "default is 92"
		},
		"orientation": {
			"type": "string",
			"enum": [
				"Z_UP", "Z_DN", 
				"X_UP", "X_DN", 
				"Y_UP", "Y_DN",
				"X_FWD", "X_BACK"
			],
			"$comment": "defaults to Z_UP"
		},
		"dmpSampleRate": {
			"type": "integer",
			"enum": [200,100,50,40,25,20,10,8,5,4],
			"$comment": "Sample rate in Hz. Must be an integer divisor of 200",
			"$comment": "default is 100"
		},
		"dmpCompassTimeConstant": {
			"type": "number",
			"minimum": 0.1,
			"$comment": "Units of seconds, defaults to 5.0"
		}
	},
	"required":["mode"]
}
```

### Published Variables, Random Mode
* BBBL_IMU_TEMP (DOUBLE) -- temperature of the sensor in degrees celsius
* BBBL_IMU_ACCEL (STRING) -- Acceleration in units of m/s^2, packed according to BBBlue_IMU_tuple
* BBBL_IMU_MAG (STRING) -- Magnetic field in units of micro Tesla, packed according to BBBlue_IMU_tuple
* BBBL_IMU_GYRO (STRING) -- Angular rates in units of degree/s, packed according to BBBlue_IMU_tuple
* BBBL_IMU_COMPASS (DOUBLE) -- Compass heading in degrees. Calculated with a simple tilt compensated compass algorithm. 

### Published Variables, DMP Mode
* BBBL_DMP_TBA (STRING) -- The Tait-Bryan angles, units of degrees, packed according to BBBlue_IMU_tuple
* BBBL_DMP_QT (STRING) -- The normalized quaternion, packed according to BBBlue_IMU_quaternion
* BBBL_DMP_HEADING (DOUBLE) -- Filtered heading in degrees
* BBBL_DMP_HEADING_RAW (DOUBLE) -- Magnetometer heading in radians

### AppCast Reports
The AppCast reports contain all of the variables published in the current mode, but the JSON tuples are broken out into their components. 

## [Barometer](http://www.strawsondesign.com/#!manual-barometer)
The barometer measures current atmospheric pressure. This can be used either for measuring the weather (if altitude is known approximately) or altitude, if the current sea level barometric pressure is known. The only parameters to configure are the oversampling rate and filter constant. Configuration of these properties is not required, but an empty Barometer parameter is required in order to activate the barometer. 

```
"Barometer": {
	"type": "object",
	"properties": {
		"oversample": {
			"type": "integer",
			"enum": [1, 2, 4, 8, 16]
		},
		"filter": {
			"type": "integer",
			"enum": [0, 2, 4, 8, 16]
		}
	}
}
```

### Published Variables
* BBBL_BARO_TEMP (DOUBLE) -- Temperature of the sensor, in degrees Celsius
* BBBL_BARO_PRES (DOUBLE) -- Current atmospheric pressure, in pascals. 
* BBBL_BARO_ALT (DOUBLE) -- Current altitude in meters, based on sea level pressure

### Subscribed Variables
* BBBL_BARO_SEA_LEVEL (DOUBLE) -- Sea level pressure, in pascals. If this is not present, it defaults to standard sea level pressure (101325 Pa). 

## [GPIO](http://www.strawsondesign.com/#!manual-gpio)
There are eight accessible GPIO pins, including the two used for the red and green LEDs. If you try to configured the LEDs with an *LED* block and then again with a *GPIO* block, you will get an error on startup and pBBBlue will exit. 

Each GPIO pin can be attached to a MOOS variable, either published or subscribed based on the direction. These variables will be BINARY. In addition to being set to input or output, the *function* property for each pin can be set to an input with a pull-up or a pull-down. 

Attempting to set a single GPIO twice in this black will cause pBBBlue to throw an error and exit on startup.

The configuration JSON is defined as follows:
```
"GPIO": {
	"type" : "array",
	"items" : {
		"type": "object",
		"properties": {
			"gpioName": {
				"type":"string",
				"enum": [
					"GPIO1_25",
					"GPIO1_17",
					"GPIO3_20",
					"GPIO3_17",
					"GPIO3_2",
					"GPIO3_1"
				]
			},
			"gpioVar": {"type": "string"},
			"function": {
				"type": "string",
				"enum": ["OUTPUT", "INPUT", "INPUT_PU", "INPUT_PD"]
			}
		},
		"required": ["gpioName", "gpioVar", "function"]
	},
	"minItems": 0,
	"maxItems": 8
},
```

## [PWM](http://www.strawsondesign.com/#!manual-pwm)
There are three PWM interfaces (0, 1, and 2) each with two channels, A & B. Channels 1 and 2 are dedicated to the motor drivers; if you attempt to both configure both the motor drivers and the PWM interface for one of these two channels, you will get an error and pBBBLue will exit during the startup phase. 

For each of the three PWM subsystems, there are five variables that can be configured.
* frequency -- this can be either a fixed numeric value or the name of a DOUBLE variable to subscribe to.
* dutyCycle{A|B} -- this is the name of a DOUBLE variable to subscribe to for the duty cycle of each of the two channels. It must have a value between 0.0f and 1.0f.
* dutycycle{A|B}ns -- this is like the regular duty cycle variable but takes a value in nanoseconds rather than a percentage. If you attempt to define both for a given interface and channel, pBBBlue will throw and error and exit. 

```
"PWM": {
	"type": "array",
	"items": {
		"oneOf": [
			null,
			{
				"type": "object",
				"properties": {
					"frequency": {
						"oneOf": [
							{"type": "string"},
							{"type": "integer"}
						]
					},
					"dutyCycleA": {"type": "string"},
					"dutyCycleB": {"type": "string"},
					"dutyCycleAns": {"type": "string"},
					"dutyCycleBns": {"type": "string"}
				},
				"required": ["frequency"]
			}
		]
	},
	"minItems": 0,
	"maxItems": 3
}
```
