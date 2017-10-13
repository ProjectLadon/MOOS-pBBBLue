# MOOS-BBBLue
An interface from MOOS to the Beaglebone Blue roboticscape library. Since the roboticscape will only run in a single process on the entire machine, it is necessarily a single process running under MOOS.

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

Configuration is set by a single json object provided either in the moos file configuration block or as a separate file and described in the schema/configuration_schema.json. The contents of each sub-section of this schema and related I/O schema are described in the section of this document for each roboticscape section.

## LEDs
In configuration, you can assign named variable to each LED (RED & GREEN). If the names are present, the module subscribes to the specified BINARY variable(s) and uses them to control the named LEDs. The configuration sub-schema is as follows:
```
"LED": {
	"type": "object",
	"properties": {
		"RED" : {"oneOf": [{"type": "string"}, null]},
		"GREEN" : {"oneOf": [{"type": "string"}, null]}
	}
}
```

Setting the subscribed binary variable to true turns the LED on and setting it false turns the LED off. 

## Buttons
In configuration, you can ssign named variables to the two buttons (PAUSE and MODE). If a name is provided, they publish a BINARY variable with the specified name and ties it to pressing (or releasing) the button. The configuration sub-schema is as follows:
```
"Button": {
	"type": "object",
	"properties": {
		"PAUSE": {"oneOf": [{"type": "string"}, null]},
		"MODE": {"oneOf": [{"type": "string"}, null]}
	}
}
```

## DC Motors
The four motor channels are configured together in a single array. The array index for each element corresponds to the motor channel it controls. If the element is a null value, the channel is disabled. If it is a string, it names a STRING variable that the module will subscribe to. It expects the messages to be json conforming to the input schema described below. The configuration schema is as follows:
```
"Motor": {
	"type": "object",
	"properties": {
		"motors": {
			"type": "array",
			"items": {"oneOf" :[null, {"type": "string"}]},	
			"minItems":1,
			"maxItems":4,
		}
	},
	"required": ["motors"] 
}
```

### Subscribed Variables
If any of the motor channels are active, this process shall subscribe to a BINARY variable called BBBL_MOTORS_ACTIVE. If it true or missing, it will enable the motors; if it is false, it will disable all motors and leave them free spinning.

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

## Encoders