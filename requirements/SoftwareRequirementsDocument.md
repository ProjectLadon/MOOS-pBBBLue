# 1.0 General Requirements {#SRD001 }

## 1.1 SRD002 {#SRD002 }

This shall be implemented as a MOOS-IvP AppCasting app.

## 1.2 SRD019 {#SRD019 }

Only one instance of this module **may** run on a particular Beaglebone Blue. Attempts to run multiple instances **shall** result in the second and later instances exiting with an error.

## 1.3 SRD133 {#SRD133 }

All failures **shall** result in an error message that accurately describes the error and makes the source of the failure clear.

## 1.4 SRD149 {#SRD149 }

This software **shall** pass valgrind memcheck with no memory leaks or corrupted memory.

# 2.0 Startup {#SRD003 }

## 2.1 SRD014 {#SRD014 }

This module **shall** read in the contents of the .moos file key conf as a JSON object, if present.

## 2.2 SRD015 {#SRD015 }

This module **shall** read in the contents of the file specified by the .moos file key ```confFile``` as JSON, if present.

## 2.3 SRD016 {#SRD016 }

If both ```conf``` and ```confFile``` are present, this module shall exit with an error.

## 2.4 SRD017 {#SRD017 }

The configuration JSON **must** conform to the schema defined in schema/configuration_schema.json.

## 2.5 SRD018 {#SRD018 }

If the configuration JSON is invalid, this module **shall** exit with an error.

# 3.0 LEDs {#SRD004 }

The Beaglebone Blue has two user-controllable LEDs, one red and one green.

## 3.1 SRD020 {#SRD020 }

This module **shall** subscribe to a BINARY community variable named by the ```RED``` key.

## 3.2 SRD021 {#SRD021 }

This module **shall** subscribe to a BINARY community variable named by the ```GREEN``` key.

## 3.3 SRD022 {#SRD022 }

If one of the named variables is true, the corresponding LED **shall** be turned on, and off if the variable is false.

## 3.4 SRD023 {#SRD023 }

LED switching **shall** happen when the OnNewMail() function is executed.

# 4.0 SRD005 {#SRD005 }

Buttons

The Beaglebone Blue has two user controllable button, mode and pause.

## 4.1 SRD024 {#SRD024 }

If the ```PAUSE``` key is present and the value is not null, this module **shall** publish a variable with the name given whose value is true if the pause button is pressed and false otherwise.

## 4.2 SRD025 {#SRD025 }

If the ```MODE``` key is present and the value is not null, this module **shall** publish a variable with the name given whose value is true if the mode button is pressed and false otherwise.

## 4.3 SRD026 {#SRD026 }

The buttons **shall** be sampled on every tick.

# 5.0 SRD006 {#SRD006 }

DC Motors

The Beaglebone Blue has four DC motor driver channels. Each can drive 1.2A at the main battery voltage. The driving values are +/- 1.0f.

## 5.1 Configuration JSON {#SRD027 }

### 5.1.1 SRD028 {#SRD028 }

Each array element in the configuration JSON **shall** correspond to the motor channel with the same index. For example, the first element in the array (index 0) configures motor channel 0.

### 5.1.2 SRD029 {#SRD029 }

If a particular array index is not present or has a value of null, then the corresponding motor channel **shall** be deactivated.

### 5.1.3 SRD030 {#SRD030 }

If a particular array element contains a string, this module **shall** subscribe to that variable.

### 5.1.4 SRD031 {#SRD031 }

The subscribed variable **must** be a STRING containing a JSON object conforming to schema/motor_input_schema.json.

#### 5.1.4.1 SRD041 {#SRD041 }

If the JSON object is a number with a value between -1.0f and 1.0f, that number **shall** be written to the corresponding motor channel.

#### 5.1.4.2 If the JSON object is a string, it **must** have a value of either FREE or BRAKE. {#SRD042 }

##### 5.1.4.2.1 SRD043 {#SRD043 }

If the value is FREE, the motor channel **shall** be set to free spinning mode.

##### 5.1.4.2.2 SRD044 {#SRD044 }

If the value is BRAKE, the motor channel **shall** be set to brake mode.

## 5.2 Subscribed Variables {#SRD032 }

### 5.2.1 SRD034 {#SRD034 }

Actions taken in response to subscribed variables **shall** be taken upon receipt.

### 5.2.2 SRD033 {#SRD033 }

This module **shall** subscribe to the BINARY variable BBBL_MOTORS_ACTIVE.

#### 5.2.2.1 SRD035 {#SRD035 }

If this value is true or missing, the motor drivers that are configured **shall** be enabled and active.

#### 5.2.2.2 SRD036 {#SRD036 }

If this value is present and false, the motor drivers **shall** be deactivated, allowing any attached motors to freewheel.

### 5.2.3 SRD037 {#SRD037 }

This module **shall** subscribe to the DOUBLE variable BBBL_MOTORS0_FREQUENCY.

#### 5.2.3.1 SRD038 {#SRD038 }

If present, the frequency of the motor PWM, in Hz, for channels 0 and 1 **shall** be set to the value of this variable upon receipt.

### 5.2.4 SRD039 {#SRD039 }

This module **shall** subscribe to the DOUBLE variable BBBL_MOTORS1_FREQUENCY.

#### 5.2.4.1 SRD040 {#SRD040 }

If present, the frequency of the motor PWM, in Hz, for channels 2 and 3 **shall** be set to the value of this variable upon receipt.

# 6.0 SRD007 {#SRD007 }

Encoders

The Beaglebone Blue has four incremental encoder channels.

## 6.1 SRD045 {#SRD045 }

Each element in the configuration JSON array **shall** correspond to an encoder channel with the same number. For example, the first element in the array (index 0) configures encoder channel 0.

## 6.2 SRD046 {#SRD046 }

If a particular array index is not present or is set to null, that channel **shall** be ignored.

## 6.3 SRD047 {#SRD047 }

If the array object has a ```name``` key, this module **shall** publish the value of that encoder to the named variable.

## 6.4 SRD048 {#SRD048 }

If the array object has an ```init``` key, the encoder **shall** be initialized with that value.

# 7.0 SRD008 {#SRD008 }

ADC

The Beaglebone Blue has eight ADC channels measuring 0-1.8V. Of these, the lower four are available for external connections. The top three are connected through voltage dividers for system health monitoring. They measure the DC jack voltage, the battery voltage, and the level of the 3.3V rail.

## 7.1 SRD049 {#SRD049 }

Configuration JSON

### 7.1.1 SRD054 {#SRD054 }

Each element of the ```rawChannels``` array **shall** correspond to the matching AIN channel.

### 7.1.2 SRD055 {#SRD055 }

The ```rawChannels``` array **may** contain up to eight entries, covering all eight analog inputs.

### 7.1.3 SRD056 {#SRD056 }

Each element of the ```voltChannels``` array **shall** correspond to the matching AIN channel.

### 7.1.4 SRD057 {#SRD057 }

The ```voltChannels``` array **may** have up to four entries, covering only those channels that are available for outside connections.

### 7.1.5 SRD058 {#SRD058 }

If an array entry is not null, the output of the corresponding channel **shall** be published on a DOUBLE variable with the name defined in that array element.

### 7.1.6 SRD059 {#SRD059 }

```rawChannel``` values **shall** be published as a 12-bit integer, i.e. 0-4095.

### 7.1.7 SRD060 {#SRD060 }

```voltChannel``` values **shall** be scaled to 0-1.8 to cover the analog input voltage range.

### 7.1.8 SRD061 {#SRD061 }

All configured analog variables **shall** be published on every tick.

## 7.2 Published Variables {#SRD050 }

### 7.2.1 SRD051 {#SRD051 }

These variables **shall** only be published if the configuration JSON contains an ADC object.

### 7.2.2 SRD052 {#SRD052 }

If active, this module **shall** publish the battery voltage as a DOUBLE variable named BBBL_BATTERY_VOLTAGE.

### 7.2.3 SRD053 {#SRD053 }

If active, this module **shall** publish the DC input voltage as a DOUBLE variable named BBBL_JACK_VOLTAGE.

# 8.0 SRD009 {#SRD009 }

Servos & ESCs

The Beaglebone Blue has eight R/C servo/ESC channels generated by the PRU. The Beaglebone Blue provides a 6V servo power rail for all servo outputs.

## 8.1 Configuration JSON {#SRD062 }

### 8.1.1 SRD063 {#SRD063 }

Each element of the of the configuration JSON array **shall** correspond to the servo channel of the same number.

### 8.1.2 SRD064 {#SRD064 }

If a given array element is null, the corresponding channel **shall** be disabled.

### 8.1.3 A non-null array element **must** have a ```name``` element and an ```action``` element. {#SRD065 }

#### 8.1.3.1 SRD066 {#SRD066 }

The ```name``` element **shall** name a DOUBLE variable that this module will subscribe to and attach to the corresponding servo channel.

#### 8.1.3.2 SRD067 {#SRD067 }

The ```action``` element **must** have one of the following values: ```servo90```, ```servo180```, ```servo360```, ```micros```, or ```ESC```.

#### 8.1.3.3 SRD068 {#SRD068 }

If the value of ```action``` is ```servo90```, the servo **shall** accept a value between 0.0 and 90.0, corresponding to the servo angle in degrees.

#### 8.1.3.4 SRD069 {#SRD069 }

If the value of ```action``` is ```servo180```, the servo **shall** accept a value between 0.0 and 180.0, corresponding to the servo angle in degrees.

#### 8.1.3.5 SRD070 {#SRD070 }

If the value of ```action``` is ```servo360```, the servo **shall** accept a value between 0.0 and 360.0, corresponding to the servo angle in degrees.

#### 8.1.3.6 SRD071 {#SRD071 }

If the value ```action``` is ```micros```, the servo signal in microseconds **shall** be set to this value.

#### 8.1.3.7 SRD072 {#SRD072 }

If the value ```action``` is ```ESC```, the servo **shall** accept a value between 0.0 and 100.0, corresponding to the throttle setting in degrees.

### 8.1.4 A non-null array element **may** have a ```period``` element. {#SRD073 }

#### 8.1.4.1 SRD074 {#SRD074 }

If it is present, it **shall** define the period of the servo signal in microseconds.

#### 8.1.4.2 SRD075 {#SRD075 }

This value **shall** default to 20,000 (50 Hz) if this value is missing or null.

### 8.1.5 A non-null array alement **may** have a ```minServo``` element. {#SRD076 }

#### 8.1.5.1 SRD077 {#SRD077 }

If present, this **shall** set the minimum servo command, in microseconds.

#### 8.1.5.2 SRD078 {#SRD078 }

If it is missing or null, this value **shall** default to 1000 microseconds.

### 8.1.6 A non-null array alement **may** have a ```maxServo``` element. {#SRD079 }

#### 8.1.6.1 SRD080 {#SRD080 }

If present, this **shall** set the maximum servo command, in microseconds.

#### 8.1.6.2 SRD081 {#SRD081 }

If it is missing or null, this value **shall** default to 2000 microseconds.

### 8.1.7 A non-null array alement **may** have a ```center``` element. {#SRD082 }

#### 8.1.7.1 SRD083 {#SRD083 }

If present, this **shall** set the center servo command, in microseconds.

#### 8.1.7.2 SRD084 {#SRD084 }

If it is missing or null, this value **shall** default to 1500 microseconds.

### 8.1.8 SRD085 {#SRD085 }

Commands between the center and maximum values of the input command **shall** be interpolated between the ```center``` and ```maxServo``` values.

### 8.1.9 SRD086 {#SRD086 }

Commands between the minimum and center values of the input command **shall** be interpolated between the ```minServo``` and ```center``` values.

## 8.2 Subscribed Variables {#SRD087 }

### 8.2.1 SRD088 {#SRD088 }

If active, this module **shall** subscribe to a BINARY variable, BBBL_SERVO_PWR.

### 8.2.2 SRD089 {#SRD089 }

If this variable is missing or true, the 6V servo power rail **shall** be turned on, and off if it is both present and false.

# 9.0 SRD010 {#SRD010 }

IMU

The Beaglebone Blue has an onboard nine-axis IMU. It can be used in two modes -- either random access mode or DMP (direct motion processor). When in random access mode, this module generates a tilt-compensated compass output. DMP generates quaternion and Tait-Bryan output angles.

## 9.1 Configuration JSON {#SRD090 }

### 9.1.1 The configuration JSON **must** have a ```mode``` key. {#SRD091 }

#### 9.1.1.1 SRD092 {#SRD092 }

The value of ```mode``` **must** be either "Random" or "DMP"

#### 9.1.1.2 SRD093 {#SRD093 }

If the value is "Random", the IMU **shall** be set to random mode.

#### 9.1.1.3 SRD094 {#SRD094 }

If the value is "DMP", the IMU **shall** be set to DMP mode.

### 9.1.2 Therer **may** be an ```accelRange``` key. {#SRD095 }

#### 9.1.2.1 SRD096 {#SRD096 }

If present, it **must** have a value of "2G", "4G", "8G", or "16G", which define the acceleration range in gravities.

#### 9.1.2.2 SRD097 {#SRD097 }

The IMU's acceleration range **shall** be set to this value, if present.

#### 9.1.2.3 SRD098 {#SRD098 }

The default value **shall** be "4G"

### 9.1.3 There **may** be a ```gyroRange``` key. {#SRD099 }

#### 9.1.3.1 SRD100 {#SRD100 }

If present, it **must** have a value of "250DPS", "500DPS", "1000DPS", or "2000DPS", which define the gyro range in degrees per second.

#### 9.1.3.2 SRD101 {#SRD101 }

The IMU's gyro range **shall** be set to this value, if present.

#### 9.1.3.3 SRD102 {#SRD102 }

The default value **shall** be "1000DPS"

### 9.1.4 There **may** be an ```accelFilter``` key. {#SRD103 }

#### 9.1.4.1 SRD104 {#SRD104 }

The value of this key **must** be "OFF", "184", "92", "41", "20", "10", or "5", which defines the cutoff frequency, in hertz, of the first order internal low pass filter.

#### 9.1.4.2 SRD105 {#SRD105 }

This value, if present, **shall** be written to the acceleration filter register.

#### 9.1.4.3 SRD106 {#SRD106 }

The default value **shall** be "92".

### 9.1.5 There **may** be a ```gyroFilter``` key. {#SRD107 }

#### 9.1.5.1 SRD108 {#SRD108 }

The value of this key **must** be "OFF", "184", "92", "41", "20", "10", or "5", which defines the cutoff frequency, in hertz, of the first order internal low pass filter.

#### 9.1.5.2 SRD109 {#SRD109 }

This value, if present, **shall** be written to the gyro filter register.

#### 9.1.5.3 SRD110 {#SRD110 }

The default value **shall** be "92".

### 9.1.6 There may be an ```orientation``` key. {#SRD111 }

#### 9.1.6.1 SRD112 {#SRD112 }

This key, if present, **must** have a value of "Z_UP", "Z_DN", "X_UP", "X_DN", "Y_UP", "Y_DN", "X_FWD", or "X_BACK".

#### 9.1.6.2 SRD113 {#SRD113 }

This value, if present, **shall** be written to the orientation register.

#### 9.1.6.3 SRD114 {#SRD114 }

The default value **shall** be "Z_UP"

## 9.2 SRD117 {#SRD117 }

Every app tick, this modules **shall** sample and publish all variables described below, depending on the current mode.

## 9.3 Published Variables, Random Mode {#SRD118 }

### 9.3.1 SRD119 {#SRD119 }

In random mode, this module **shall** publish BBBL_IMU_TEMP, a DOUBLE equal to the temperature of the sensor in degrees celsius

### 9.3.2 SRD120 {#SRD120 }

In random mode, this module **shall** publish BBBL_IMU_ACCEL, a JSON STRING containing acceleration in units of m/s^2, packed according to schema/imu_tuple_schema.json.

### 9.3.3 SRD121 {#SRD121 }

In random mode, this module **shall** publish BBBL_IMU_MAG (STRING), a JSON STRING containing the magnetic field in units of micro Tesla, packed according to schema/imu_tuple_schema.json.

### 9.3.4 SRD122 {#SRD122 }

In random mode, this module **shall** publish BBBL_IMU_GYRO (STRING), a JSON STRING containing the angular rates in units of degree/s, packed according to schema/imu_tuple_schema.json.

### 9.3.5 SRD123 {#SRD123 }

In random mode, this module **shall** publish BBBL_IMU_COMPASS, a DOUBLE containing the compass heading of the positive X axis degrees.

### 9.3.6 SRD124 {#SRD124 }

The random mode compass heading **shall** be calculated with the tilt-compensation algorithm described in http://www.cypress.com/file/130456/download

## 9.4 Published Variables, DMP Mode {#SRD125 }

### 9.4.1 SRD126 {#SRD126 }

In DMP mode, this module **shall** publish BBBL_DMP_TBA, a JSON STRING containing the Tait-Bryan angles in units of degrees, packed according to schema/imu_tuple_schema.json.

### 9.4.2 SRD127 {#SRD127 }

In DMP mode, this module **shall** publish BBBL_DMP_QT, a JSON STRING containing the normalized quaternion, packed according to schema/imu_quaternion_schema.json.

### 9.4.3 SRD128 {#SRD128 }

In DMP mode, this module **shall** publish BBBL_DMP_HEADING, a DOUBLE containing the filtered heading in degrees.

### 9.4.4 SRD129 {#SRD129 }

In DMP mode, this module **shall** publish BBBL_DMP_HEADING_RAW, a DOUBLE containing the unflitered magnetometer heading in radians.

## 9.5 AppCast Report {#SRD130 }

### 9.5.1 SRD131 {#SRD131 }

The AppCast report **shall** contain all published variables.

### 9.5.2 SRD132 {#SRD132 }

JSON string tuples **shall** be broken out into their components in the AppCast.

# 10 SRD011 {#SRD011 }

Barometer

The Beaglebone Blue has an onboard barometric pressure sensor. If the current sea level pressure is known, it can be used to measure altitude. It only has two parameters, the oversampling level and the filter constant, but these do not have to be configured.

## 10.1 SRD134 {#SRD134 }

This function **shall** be activated if and only if a ```Barometer``` key is present in the configuration JSON. Its value **may** be null.

## 10.2 The ```Barometer``` element **may** contain an ```oversample``` key. {#SRD135 }

### 10.2.1 SRD136 {#SRD136 }

If this key is present, it **must** have an integer value of 1, 2, 4, 8, or 16.

### 10.2.2 SRD137 {#SRD137 }

The oversample register **shall** be set to the value of this key if present.

## 10.3 The ```Barometer``` element **may** contain an ```filter``` key. {#SRD138 }

### 10.3.1 SRD139 {#SRD139 }

If this key is present, it **must** have an integer value of 0, 2, 4, 8, or 16.

### 10.3.2 SRD140 {#SRD140 }

The filter register **shall** be set to the value of this key if present.

## 10.4 This function **shall** subscribe to BBBL_BARO_SEA_LEVEL, a DOUBLE containing the current sea level pressure in pascals. {#SRD141 }

### 10.4.1 SRD142 {#SRD142 }

If this variable is not available, the value **shall** default to standard sea level pressure, 101325 Pa.

## 10.5 Published Variables {#SRD143 }

### 10.5.1 SRD144 {#SRD144 }

Variables **shall** be published if and only if this function is active.

### 10.5.2 SRD145 {#SRD145 }

This function **shall** publish BBBL_BARO_TEMP, a DOUBLE containing the temperature of the sensor, in degrees Celsius.

### 10.5.3 SRD146 {#SRD146 }

This function **shall** publish BBBL_BARO_PRES, a DOUBLE containing the current atmospheric pressure, in pascals.

### 10.5.4 SRD147 {#SRD147 }

This function **shall** publish BBBL_BARO_ALT, a DOUBLE containing the current altitude in meters, based on sea level pressure.

## 10.6 SRD148 {#SRD148 }

All published variables shall be published in an AppCast as well.

# 11 SRD012 {#SRD012 }

GPIO

The Beaglebone Blue has eight free GPIO lines, including the two used for the red and green LEDs. Each of these can be connected to a corresponding MOOS variable; whether it is published or subscribed depends on the pin direction. Additionally, there are pull ups and pull downs which can be set on inputs.

## 11.1 Configuration JSON {#SRD150 }

### 11.1.1 SRD151 {#SRD151 }

The configuration JSON **must** consist of a JSON array of not more than eight items.

### 11.1.2 Each element **must** contain a ```gpioName``` key. {#SRD152 }

#### 11.1.2.1 SRD153 {#SRD153 }

The value of this key **must** name one of the GPIO outputs, which are ```GPIO1_25```, ```GPIO1_17```, ```GPIO3_20```, ```GPIO3_17```, ```GPIO3_2```, ```GPIO3_1```, ```LED_RED```, and ```LED_GRN```

#### 11.1.2.2 SRD154 {#SRD154 }

If the LED block is configured, any attempt to configure ```LED_RED``` or ```LED_GRN``` **shall** cause pBBBlue to fail and exit during startup.

### 11.1.3 Each element **must** contain a ```function``` key. {#SRD155 }

#### 11.1.3.1 SRD156 {#SRD156 }

The value of this key **must** be ```OUTPUT```, ```INPUT```, ```INPUT_PU```, or ```INPUT_PD```.

#### 11.1.3.2 SRD157 {#SRD157 }

If the value is ```OUTPUT```, this function **shall** set that GPIO pin to an output.

#### 11.1.3.3 SRD158 {#SRD158 }

If the value is ```INPUT```, this function **shall** set that GPIO pin to a high-Z input.

#### 11.1.3.4 SRD159 {#SRD159 }

If the value is ```INPUT_PU```, this function **shall** set that GPIO pin to an input with a pull-up.

#### 11.1.3.5 SRD160 {#SRD160 }

If the value is ```INPUT_PD```, this function **shall** set that GPIO pin to an input with a pull-down.

### 11.1.4 Each element **must** contain a ```gpioVar``` key. {#SRD161 }

#### 11.1.4.1 SRD162 {#SRD162 }

If the element is configured as an output, this function **shall** subscribe to that variable.

#### 11.1.4.2 SRD163 {#SRD163 }

If the element is configured as any kind of input, this function **shall** publish a BINARY variable of that name.

## 11.2 SRD164 {#SRD164 }

This function **shall** propagate subscribed variables to the corresponding outputs upon receipt.

## 11.3 SRD165 {#SRD165 }

This function **shall** publish all inputs every tick.

## 11.4 SRD166 {#SRD166 }

This function **shall** add its published variables to this module's AppCast.

# 12 SRD013 {#SRD013 }

PWM

The Beaglebone Blue has three PWM interfaces (0, 1, and 2) with two channels each. Interfaces 0 and 1 are dedicated to the motor drivers and may not be configured here if the motors are active. Each interface can be set to a different PWM frequency.

## 12.1 Configuration JSON {#SRD167 }

### 12.1.1 SRD168 {#SRD168 }

The configuration JSON **must** consist of a JSON array with (optionally) an entry for each of three PWM interfaces.

### 12.1.2 SRD169 {#SRD169 }

Each element **shall** configure the corresponding PWM interface.

### 12.1.3 SRD170 {#SRD170 }

An element **may** be null.

### 12.1.4 SRD171 {#SRD171 }

If the DC motors are configured and either the 0 or 1 elements are not null, this function **shall** print an error and force pBBBlue to exit.

### 12.1.5 The ```frequency``` key **must** be present and **may** be either a fixed integer value or the name of a MOOS variable to of type DOUBLE. {#SRD172 }

#### 12.1.5.1 SRD173 {#SRD173 }

If the key is an integer, the frequency of that PWM interface **shall** be set to the given value.

#### 12.1.5.2 SRD174 {#SRD174 }

If the key is a string, this function **shall** subscribe to that variable.

#### 12.1.5.3 SRD175 {#SRD175 }

If the variable is missing or is not of type DOUBLE, this PWM interface **shall** be disabled.

### 12.1.6 Either the ```dutyCycleA``` or ```dutyCycleAns``` **must** be present. {#SRD176 }

#### 12.1.6.1 SRD177 {#SRD177 }

If neither or both are present, this function **shall** print and error and force pBBBlue to exit.

#### 12.1.6.2 SRD178 {#SRD178 }

If ```dutyCycleA``` is present, the named variable exists, is of type DOUBLE, and has a value between 0.0f and 1.0f, inclusive, then the duty cycle of channel A of the corresponding interface shall be set to that value.

#### 12.1.6.3 SRD179 {#SRD179 }

If ```dutyCycleAns``` is present, the named variable exists, is of type DOUBLE, and has a value between 0 and the PWM period in nanosecond, inclusive, then the on time of channel A of the corresponding interface shall be set to that value, in nanoseconds.

### 12.1.7 Either the ```dutyCycleB``` or ```dutyCycleBns``` **must** be present. {#SRD180 }

#### 12.1.7.1 SRD181 {#SRD181 }

If neither or both are present, this function **shall** print and error and force pBBBlue to exit.

#### 12.1.7.2 SRD182 {#SRD182 }

If ```dutyCycleB``` is present, the named variable exists, is of type DOUBLE, and has a value between 0.0f and 1.0f, inclusive, then the duty cycle of channel A of the corresponding interface shall be set to that value.

#### 12.1.7.3 SRD183 {#SRD183 }

If ```dutyCycleBns``` is present, the named variable exists, is of type DOUBLE, and has a value between 0 and the PWM period in nanosecond, inclusive, then the on time of channel A of the corresponding interface shall be set to that value, in nanoseconds.

