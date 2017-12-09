# 1.0 Dependencies & Build Configuration {#SDD001 }

## 1.1 SDD002 {#SDD002 }

This software **shall** be implemented as a MOOS Appcasting App as describe in the [MOOS-IvP wiki](http://oceanai.mit.edu/moos-ivp/pmwiki/pmwiki.php).

## 1.2 SDD003 {#SDD003 }

This software **shall** use [rapidjson](http://www.rapidjson.org) as the JSON parse/unparse library.

## 1.3 SDD004 {#SDD004 }

This software **shall** wrap the [roboticscape](http://www.strawsondesign.com/#!manual-install) library

## 1.4 SDD005 {#SDD005 }

Unit tests **shall** be written using the gtest suite.

## 1.5 SDD006 {#SDD006 }

This software **shall** be compiled using cmake.

## 1.6 SDD007 {#SDD007 }

The cmake list **shall** generate a MOOS executable target.

## 1.7 SDD008 {#SDD008 }

The cmake list **shall** generate a gtest target called ```unit_tests```.

## 1.8 SDD009 {#SDD009 }

The cmake list **shall** generate a target that compiles and runs the tests called ```test```.

# 2.0 Data Types {#SDD010 }

## 2.1 SDD011 {#SDD011 }

```enum class GPIOmode``` **shall** have a member for each of the four possible input modes: ```OUTPUT```, ```INPUT```, ```INPUT_PU```, and ```INPUT_PD```.

## 2.2 SDD012 {#SDD012 }

```enum class ServoMode``` **shall** have a member for each of the five possible servo modes: ```servo90```,  ```servo180```, ```servo360```, ```micros```, and ```ESC```.

## 2.3 ```GPIOpin``` class {#SDD013 }

### 2.3.1 SDD014 {#SDD014 }

This class defines the state of a single GPIO pin

### 2.3.2 Private Members {#SDD015 }

#### 2.3.2.1 SDD016 {#SDD016 }

```GPIOmode mode``` **shall** store the current mode of the pin.

#### 2.3.2.2 SDD017 {#SDD017 }

```bool state``` **shall** store the current value of the pin.

#### 2.3.2.3 SDD018 {#SDD018 }

```int pin``` **shall** store the pin number.

#### 2.3.2.4 SDD019 {#SDD019 }

```std::string var``` **shall** store the name of the attached variable.

### 2.3.3 Public Methods {#SDD020 }

#### 2.3.3.1 SDD021 {#SDD021 }

```GPIOmode getmode()``` **shall** return the value of ```mode```.

#### 2.3.3.2 SDD022 {#SDD022 }

```std::string getVar()``` **shall** return the value of ```var```.

#### 2.3.3.3 SDD023 {#SDD023 }

```void set(bool n)``` **shall** set the ```state``` equal to ```n``` if and only if ```state = GPIOmode::OUTPUT```. Otherwise, it **shall** do nothing.

#### 2.3.3.4 SDD024 {#SDD024 }

```bool get()``` **shall** poll the GPIO pin, set ```value``` to the result, and return it.

#### 2.3.3.5 SDD025 {#SDD025 }

```bool subscribe()``` This method **shall** return ```true``` if and only if ```mode``` is ```GPIOmode::OUTPUT```.

### 2.3.4 Constructor {#SDD026 }

#### 2.3.4.1 SDD027 {#SDD027 }

```GPIOpin(int pin, GPIOmode mode, std::string var, bool state = false)``` **shall** set up the named pin with the named mode. If it is an output, it **shall** set it to the provided state.

### 2.3.5 SDD028 {#SDD028 }

The destructor for this class shall set the pinmuxes back to default.

## 2.4 ```PWMChannel``` class {#SDD029 }

### 2.4.1 SDD030 {#SDD030 }

This class defines the state of a single PWM interface and its two pins.

### 2.4.2 Private Members {#SDD031 }

#### 2.4.2.1 SDD032 {#SDD032 }

```int iface``` **shall** store the number of the interface this points to.

#### 2.4.2.2 SDD033 {#SDD033 }

```int freq``` **shall** store the frequency, measured in Hz and **shall** default to 1000.

#### 2.4.2.3 SDD034 {#SDD034 }

```std::string freqVar``` **shall** store the name of the variable that sets the frequency.

#### 2.4.2.4 SDD035 {#SDD035 }

```std::string varA``` **shall** store the name of the variable that sets the duty cycle of channel A.

#### 2.4.2.5 SDD036 {#SDD036 }

```std::string varB``` **shall** store the name of the variable that sets the duty cycle of channel B.

#### 2.4.2.6 SDD037 {#SDD037 }

```bool isnsA``` **shall** be ```true``` if channel A expects a nanosecond value.

#### 2.4.2.7 SDD038 {#SDD038 }

```bool isnsB``` **shall** be ```true``` if channel B expects a nanosecond value.

### 2.4.3 Public Methods {#SDD039 }

#### 2.4.3.1 SDD040 {#SDD040 }

```bool setFrequency (int f)``` **shall** set ```freq``` to the provided value and call ```rc_pwm_init()``` to change the frequency of the interface. It **shall** not change the duty cycle.

#### 2.4.3.2 SDD041 {#SDD041 }

```std::string getVarA``` **shall** return ```varA```.

#### 2.4.3.3 SDD042 {#SDD042 }

```std::string getVarB``` **shall** return ```varB```.

#### 2.4.3.4 SDD043 {#SDD043 }

```std::string getFreqVar``` **shall** return ```freqVar```

#### 2.4.3.5 ```bool setDutyA (float d)``` **shall** set the duty cycle of channel A on the configured interface. {#SDD044 }

##### 2.4.3.5.1 SDD045 {#SDD045 }

The argument **shall** be interpreted as a fraction if ```isnsA``` equals ```false``` and as a number of nanoseconds otherwise.

##### 2.4.3.5.2 SDD046 {#SDD046 }

If ```isnsA``` is ```false```, this method **shall** check that the argument is between 0.0f and 1.0f and return false if it is not.

#### 2.4.3.6 ```bool setDutyB (float d)``` **shall** set the duty cycle of channel B on the configured interface. {#SDD047 }

##### 2.4.3.6.1 SDD048 {#SDD048 }

The argument **shall** be interpreted as a fraction if ```isnsB``` equals ```false``` and as a number of nanoseconds otherwise.

##### 2.4.3.6.2 SDD049 {#SDD049 }

If ```isnsB``` is ```false```, this method **shall** check that the argument is between 0.0f and 1.0f and return false if it is not.

#### 2.4.3.7 SDD050 {#SDD050 }

```bool subscribe()``` **shall** subscribe to the variables named by ```varA```, ```varB```, and ```freqVar```, if they are non-empty.

### 2.4.4 Constructor {#SDD051 }

#### 2.4.4.1 ```PWMChannel(int iface, float freq, std::string varA, std::string varB, bool isnsA = false, bool isnsB = false)``` {#SDD052 }

##### 2.4.4.1.1 SDD053 {#SDD053 }

This constructor **shall** set ```iface``` to the given value and other members to their defaults.

##### 2.4.4.1.2 SDD054 {#SDD054 }

```freq``` **shall** be set to the given value and ```freqVar``` **shall** be set to the empty string.

##### 2.4.4.1.3 SDD055 {#SDD055 }

```varA``` and ```varB``` **shall** be set to the given values.

##### 2.4.4.1.4 SDD056 {#SDD056 }

```isnsA``` and ```isnsB``` **shall** be set to the given values.

##### 2.4.4.1.5 SDD057 {#SDD057 }

If ```iface``` is 0, this constructor **shall** set the pinmux to enable the output pins.

#### 2.4.4.2 ```PWMChannel(int iface, std::string freqVar, std::string varA, std::string varB, bool isnsA = false, bool isnsB = false)``` {#SDD058 }

##### 2.4.4.2.1 SDD059 {#SDD059 }

This constructor **shall** set ```iface``` to the given value and other members to their defaults.

##### 2.4.4.2.2 SDD060 {#SDD060 }

```freqVar``` **shall** be set to the given value and ```freq``` **shall** be set to 1000.

##### 2.4.4.2.3 SDD061 {#SDD061 }

```varA``` and ```varB``` **shall** be set to the given values.

##### 2.4.4.2.4 SDD062 {#SDD062 }

```isnsA``` and ```isnsB``` **shall** be set to the given values.

##### 2.4.4.2.5 SDD063 {#SDD063 }

If ```iface``` is 0, this constructor **shall** set the pinmux to enable the output pins.

### 2.4.5 SDD064 {#SDD064 }

The destructor for this class shall set the pinmux back to default if ```iface``` is 0.

## 2.5 ```ServoChannel``` class {#SDD065 }

### 2.5.1 SDD066 {#SDD066 }

This class **shall** store the configuration of a single servo channel.

### 2.5.2 Private Members {#SDD067 }

#### 2.5.2.1 SDD068 {#SDD068 }

```const ServoMode mode``` **shall** store the mode of this servo channel.

#### 2.5.2.2 SDD069 {#SDD069 }

```const int channel``` **shall** store the number of this channel.

#### 2.5.2.3 SDD070 {#SDD070 }

```const std::string var``` **shall** store the name of the variable that controls this channel.

#### 2.5.2.4 SDD071 {#SDD071 }

```const std::chrono::duration period``` **shall** store the duration of each repetition.

#### 2.5.2.5 SDD072 {#SDD072 }

```const long min``` **shall** store the minimum length of the servo pulse in microseconds.

#### 2.5.2.6 SDD073 {#SDD073 }

```const long max``` **shall** store the maximum length of the servo pulse in microseconds.

#### 2.5.2.7 SDD074 {#SDD074 }

```const long center``` **shall** store the length of the servo pulse in microseconds at the center position.

#### 2.5.2.8 SDD075 {#SDD075 }

```long current``` **shall** store the current length of the servo pulse in microseconds.

#### 2.5.2.9 SDD076 {#SDD076 }

```const bool invert``` if this is ```true```, larger input value **shall** result in smaller values of ```current```.

#### 2.5.2.10 SDD077 {#SDD077 }

```bool killThread``` **shall** trigger the shutdown of ```servoThread``` when it is ```true```.

#### 2.5.2.11 SDD078 {#SDD078 }

```thread *servoThread``` **shall** store a pointer to the current thread.

### 2.5.3 Private Methods {#SDD079 }

#### 2.5.3.1 ```bool setServo90(double s)``` {#SDD080 }

##### 2.5.3.1.1 SDD081 {#SDD081 }

This method **shall** take a servo angle in degrees.

##### 2.5.3.1.2 SDD082 {#SDD082 }

Values outside the range of 0-90 **shall** be bounded to that range.

##### 2.5.3.1.3 SDD083 {#SDD083 }

Values between 0 and 45 **shall** be mapped to the range between ```min``` and ```center```.

##### 2.5.3.1.4 SDD084 {#SDD084 }

Values between 45 and 90 **shall** be mapped to the range between ```center``` and ```max```.

##### 2.5.3.1.5 SDD085 {#SDD085 }

If ```invert``` is ```true```, the direction of action **shall** be reversed.

#### 2.5.3.2 ```bool setServo180(double s)``` {#SDD086 }

##### 2.5.3.2.1 SDD087 {#SDD087 }

This method **shall** take a servo angle in degrees.

##### 2.5.3.2.2 SDD088 {#SDD088 }

Values outside the range of 0-180 **shall** be bounded to that range.

##### 2.5.3.2.3 SDD089 {#SDD089 }

Values between 0 and 90 **shall** be mapped to the range between ```min``` and ```center```.

##### 2.5.3.2.4 SDD090 {#SDD090 }

Values between 90 and 180 **shall** be mapped to the range between ```center``` and ```max```.

##### 2.5.3.2.5 SDD091 {#SDD091 }

If ```invert``` is ```true```, the direction of action **shall** be reversed.

#### 2.5.3.3 ```bool setServo360(double s)``` {#SDD092 }

##### 2.5.3.3.1 SDD093 {#SDD093 }

This method **shall** take a servo angle in degrees.

##### 2.5.3.3.2 SDD094 {#SDD094 }

Values outside the range of 0-360 **shall** be bounded to that range.

##### 2.5.3.3.3 SDD095 {#SDD095 }

Values between 0 and 180 **shall** be mapped to the range between ```min``` and ```center```.

##### 2.5.3.3.4 SDD096 {#SDD096 }

Values between 180 and 360 **shall** be mapped to the range between ```center``` and ```max```.

##### 2.5.3.3.5 SDD097 {#SDD097 }

If ```invert``` is ```true```, the direction of action **shall** be reversed.

#### 2.5.3.4 ```bool setServoESC(double s)``` {#SDD098 }

##### 2.5.3.4.1 SDD099 {#SDD099 }

This method **shall** take a throttle setting per percent.

##### 2.5.3.4.2 SDD100 {#SDD100 }

Values outside the range of 0-100 **shall** be bounded to that range.

##### 2.5.3.4.3 SDD101 {#SDD101 }

Values between 0 and 50 **shall** be mapped to the range between ```min``` and ```center```.

##### 2.5.3.4.4 SDD102 {#SDD102 }

Values between 50 and 100 **shall** be mapped to the range between ```center``` and ```max```.

##### 2.5.3.4.5 SDD103 {#SDD103 }

If ```invert``` is ```true```, the direction of action **shall** be reversed.

#### 2.5.3.5 SDD104 {#SDD104 }

```bool setServoMicros(double s)``` **shall** take a pulse length in microseconds and write it directly to ```current```.

#### 2.5.3.6 SDD105 {#SDD105 }

```void pulse()``` **shall** send one servo pulse of ```current``` microseconds every ```period``` until ```killThread``` is ```true```. This is intended to be the execution

### 2.5.4 Static Public Members {#SDD106 }

#### 2.5.4.1 SDD107 {#SDD107 }

```static const std::chronon::duration defaultPeriod``` **shall** be set to 20000 microseconds.

#### 2.5.4.2 SDD108 {#SDD108 }

```static const long defaultMax``` **shall** be set to 2000

#### 2.5.4.3 SDD109 {#SDD109 }

```static const long defaultMin``` **shall** be set to 1000

#### 2.5.4.4 SDD110 {#SDD110 }

```static const long defaultCenter``` **shall** be set to 1500

#### 2.5.4.5 SDD111 {#SDD111 }

```static const bool defaultInvert``` **shall** be set to ```false```.

### 2.5.5 Public Methods {#SDD112 }

#### 2.5.5.1 SDD113 {#SDD113 }

```ServoMode getMode()``` **shall** return the value of ```mode```.

#### 2.5.5.2 SDD114 {#SDD114 }

```std::string getVar()``` **shall** return the value of ```var```.

#### 2.5.5.3 SDD115 {#SDD115 }

```long getPeriod()``` **shall** return the value of ```period```.

#### 2.5.5.4 SDD116 {#SDD116 }

```bool setServo(double s)`` **shall** call the appropriate private method based on the value of ```mode``` to set the value of ```current```.

#### 2.5.5.5 SDD117 {#SDD117 }

```void startThread()``` **shall** set ```killThread``` to ```false```, spawn a new thread with the ```pulse()``` method, and store a pointer to it in ```servoThread```.

#### 2.5.5.6 SDD118 {#SDD118 }

```void stopThread()``` **shall** set ```killThread``` to ```true```.

#### 2.5.5.7 SDD119 {#SDD119 }

```bool matchVar(std::string v)``` **shall** return ```true``` if and only if ```v``` is equal to ```var```. This comparison **shall** be case insensitive.

### 2.5.6 Constructor {#SDD120 }

#### 2.5.6.1 SDD121 {#SDD121 }

```ServoChannel(ServoMode action, int chan, std::chrono::duration period = defaultPeriod, long maxServo = defaultMax, long minServo = defaultMin, long center = defaultCenter, invert = defaultInvert)``` **shall** set the members of this class to the given values.

## 2.6 ```FunctionBlock``` virtual base class {#SDD122 }

### 2.6.1 SDD123 {#SDD123 }

This class **shall** be a virtual base class.

### 2.6.2 Public Methods {#SDD124 }

#### 2.6.2.1 SDD125 {#SDD125 }

```virtual bool configure(rapidjson::Document d)``` **shall** configure the class from an appropriate JSON object.

#### 2.6.2.2 SDD126 {#SDD126 }

```virtual bool procMail(CMOOSMsg &msg)``` **shall** process incoming mail.

#### 2.6.2.3 SDD127 {#SDD127 }

```virtual bool tick()``` **shall** execute periodic actions, such as data reads.

#### 2.6.2.4 SDD128 {#SDD128 }

```virtual bool subscribe()``` **shall** subscribe to any configured variables.

#### 2.6.2.5 SDD129 {#SDD129 }

```virtual bool isConfigured()``` **shall** return true if any part of the block is configured.

## 2.7 ```LEDBlock``` class {#SDD130 }

### 2.7.1 SDD131 {#SDD131 }

This class **shall** be a singleton

### 2.7.2 SDD132 {#SDD132 }

This class is a sub-class of ```FunctionBlock```

### 2.7.3 Private Members {#SDD133 }

#### 2.7.3.1 SDD134 {#SDD134 }

```static LEDBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.7.3.2 SDD135 {#SDD135 }

```std::string REDname``` **shall** store the name of the MOOS variable that controls the red LED.

#### 2.7.3.3 SDD136 {#SDD136 }

```bool GRNstate``` **shall** store the current state of the red LED.

#### 2.7.3.4 SDD137 {#SDD137 }

```std::string GRNname``` **shall** store the name of the MOOS variable that controls the red LED.

#### 2.7.3.5 SDD138 {#SDD138 }

```bool GRNstate``` **shall** store the current state of the red LED.

### 2.7.4 Public Methods {#SDD139 }

#### 2.7.4.1 SDD140 {#SDD140 }

```static LEDBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.7.4.2 ```bool configure(rapidjson::Document d)``` {#SDD141 }

##### 2.7.4.2.1 SDD142 {#SDD142 }

This method **shall** consume an ```LED``` object as defined in ```schema/configuration_schema.json```.

##### 2.7.4.2.2 SDD143 {#SDD143 }

The ```REDname``` and ```GRNname``` members **shall** be populated from the ```RED``` and ```GREEN``` elements of the configuration JSON, if they are present.

##### 2.7.4.2.3 SDD144 {#SDD144 }

JSON nulls **shall** be treated as empty strings.

##### 2.7.4.2.4 SDD145 {#SDD145 }

This method **shall** always return true.

#### 2.7.4.3 ```bool procMail(CMOOSMsg &msg)``` {#SDD146 }

##### 2.7.4.3.1 SDD147 {#SDD147 }

If ```msg.GetKey()``` is not equal to either ```REDname``` or ```GRNname```, return ```false```.

##### 2.7.4.3.2 SDD148 {#SDD148 }

If ```msg``` matches a subscribed variable, this method **shall** check whether it is BINARY and throw a ```std::invalid_argument``` exception if it is not.

##### 2.7.4.3.3 SDD149 {#SDD149 }

This method **shall** set the corresponding state variable equal to the incoming value and call ```rc_set_led()``` against the corresponding LED, returning the result.

#### 2.7.4.4 SDD150 {#SDD150 }

```bool tick()``` **shall** do nothing and return ```true```.

#### 2.7.4.5 SDD151 {#SDD151 }

```bool subscribe()``` **shall** subscribe the variables named in ```REDname``` and ```GRNname``` if they are not empty and return the result.

#### 2.7.4.6 SDD152 {#SDD152 }

```bool isConfigured()``` **shall** return ```true``` if either ```REDname``` or ```GRNname``` is non-empty.

## 2.8 ```ButtonBlock``` class {#SDD153 }

### 2.8.1 SDD154 {#SDD154 }

This class **shall** be a singleton

### 2.8.2 SDD155 {#SDD155 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.8.3 Private Members {#SDD156 }

#### 2.8.3.1 SDD157 {#SDD157 }

```static ButtonBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.8.3.2 SDD158 {#SDD158 }

```std::string pause``` **shall** store the name of the variable the PAUSE button **shall** be published to.

#### 2.8.3.3 SDD159 {#SDD159 }

```std::string mode``` **shall** store the name of the variable the MODE button **shall** be published to.

### 2.8.4 Public Methods {#SDD160 }

#### 2.8.4.1 SDD161 {#SDD161 }

```static ButtonBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.8.4.2 ```bool configure(rapidjson::Document d)``` {#SDD162 }

##### 2.8.4.2.1 SDD163 {#SDD163 }

This method **shall** consume a ```Button``` object as defined in ```schema/configuration_schema.json```.

##### 2.8.4.2.2 SDD164 {#SDD164 }

The ```mode``` and ```pause``` members **shall** be populated from the ```MODE``` and ```PAUSE``` elements of the configuration JSON, if they are present.

##### 2.8.4.2.3 SDD165 {#SDD165 }

JSON nulls **shall** be treated as empty strings.

##### 2.8.4.2.4 SDD166 {#SDD166 }

This method **shall** always return true.

#### 2.8.4.3 SDD167 {#SDD167 }

```bool procMail(CMOOSMsg &msg)``` **shall** do nothing and return ```false```.

#### 2.8.4.4 ```bool tick()``` {#SDD168 }

##### 2.8.4.4.1 SDD169 {#SDD169 }

If ```pause``` is not an empty string, this method **shall** read the value of the PAUSE button and publish it to the variable named in ```pause```.

##### 2.8.4.4.2 SDD170 {#SDD170 }

If ```mode``` is not an empty string, this method **shall** read the value of the MODE button and publish it to the variable named in ```mode```.

##### 2.8.4.4.3 SDD171 {#SDD171 }

This method **shall** return true.

#### 2.8.4.5 SDD172 {#SDD172 }

```bool subscribe()``` **shall** do nothing and return ```true```.

#### 2.8.4.6 SDD173 {#SDD173 }

```bool isConfigured()``` **shall** return ```true``` if either ```pause``` or ```mode``` is non-empty.

## 2.9 ```MotorBlock``` class {#SDD174 }

### 2.9.1 SDD175 {#SDD175 }

This class **shall** be a singleton

### 2.9.2 SDD176 {#SDD176 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.9.3 Private Members {#SDD177 }

#### 2.9.3.1 SDD178 {#SDD178 }

```static MotorsBlock* s_instance``` **shall** store a pointer to the only instance of this sub-class.

#### 2.9.3.2 SDD179 {#SDD179 }

```std::list<std::string> motors``` **shall** store the names of the motor variables.

### 2.9.4 Private Methods {#SDD180 }

#### 2.9.4.1 ```bool setMotor(int motor, std::string val)``` {#SDD181 }

##### 2.9.4.1.1 SDD182 {#SDD182 }

This method **shall** parse ```val``` into a JSON object and validate it against schema/motor_input_schema.json. If it fails, this method **shall** return ```false```.

##### 2.9.4.1.2 SDD183 {#SDD183 }

This method **shall** increment the ```motor``` number by one, in order to transform the 0-3 of the list indices into the 1-4 of the motor numbers.

##### 2.9.4.1.3 SDD184 {#SDD184 }

This method **shall** check that ```motor``` is between 1 and 4, inclusive. If it is not, this method **shall** return false.

##### 2.9.4.1.4 SDD185 {#SDD185 }

If the value of the JSON is a number, it **must** be between -1.0f and 1.0f. If it is out of range, this method **shall** return ```false```.

##### 2.9.4.1.5 SDD186 {#SDD186 }

If it is a number and in range, this method **shall** pass it to ```rc_set_motor()``` and return the result.

##### 2.9.4.1.6 SDD187 {#SDD187 }

If the incoming value is a a string and is equal to "FREE", this method **shall** call ```rc_set_motor_free_spin()``` and return the result.

##### 2.9.4.1.7 SDD188 {#SDD188 }

If the incoming value is a a string and is equal to "BRAKE", this method **shall** call ```rc_set_motor_brake()``` and return the result.

### 2.9.5 Public Methods {#SDD189 }

#### 2.9.5.1 SDD190 {#SDD190 }

```static MotorsBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.9.5.2 ```bool configure(rapidjson::Document d)``` {#SDD191 }

##### 2.9.5.2.1 SDD192 {#SDD192 }

This method **shall** consume a ```Motor``` object from the configuration JSON and populate ```motors``` with its contents.

##### 2.9.5.2.2 SDD193 {#SDD193 }

If any of the elements are non-empty, this method **shall** call ```rc_enable_motors()```.

#### 2.9.5.3 ```bool procMail(CMOOSMsg &msg)``` {#SDD194 }

##### 2.9.5.3.1 SDD195 {#SDD195 }

If all members of ```motors``` are empty strings, this method **shall** return ```false```.

##### 2.9.5.3.2 SDD196 {#SDD196 }

If ```msg.GetKey()``` matches one of the non-empty members of ```motors```, this method **shall** pass the value and the matching index to ```setMotor()```, then return the result.

##### 2.9.5.3.3 SDD197 {#SDD197 }

If ```msg.GetKey()``` is equal to ```BBBL_MOTORS_ACTIVE``` and the value is both BINARY and ```false```, call ```rc_disable_motors()``` and return ```true```. If it is BINARY and ```true```, this method **shall** call ```rc_enable_motors()```.

##### 2.9.5.3.4 SDD198 {#SDD198 }

If ```msg.GetKey()``` is equal to ```BBBL_MOTORS0_FREQUENCY``` and the value is a DOUBLE, this method **shall** call ```rc_pwm_init()``` against PWM subsystem 1 with the given frequency value and return the result.

##### 2.9.5.3.5 SDD199 {#SDD199 }

If ```msg.GetKey()``` is equal to ```BBBL_MOTORS1_FREQUENCY``` and the value is a DOUBLE, this method **shall** call ```rc_pwm_init()``` against PWM subsystem 2 with the given frequency value and return the result.

#### 2.9.5.4 SDD200 {#SDD200 }

```bool tick()``` **shall** do nothing and return true.

#### 2.9.5.5 SDD201 {#SDD201 }

```bool subscribe()``` **shall** subscribe to each non-empty string in ```motors```, as well as ```BBBL_MOTORS_ACTIVE```, ```BBBL_MOTORS0_FREQUENCY```, and ```BBBL_MOTORS0_FREQUENCY```. It **shall** then return the logical and of the results.

#### 2.9.5.6 SDD202 {#SDD202 }

```bool isConfigured()``` **shall** return ```true``` if any member of ```motors``` is non-empty.

## 2.10 ```EncodersBlock``` class {#SDD203 }

### 2.10.1 SDD204 {#SDD204 }

This class **shall** be a singleton

### 2.10.2 SDD205 {#SDD205 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.10.3 Private Members {#SDD206 }

#### 2.10.3.1 SDD207 {#SDD207 }

```static EncodersBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.10.3.2 SDD208 {#SDD208 }

```std::list<std::string> encoders``` **shall** store the names of the encoder variables.

### 2.10.4 Public Methods {#SDD209 }

#### 2.10.4.1 SDD210 {#SDD210 }

```static EncodersBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.10.4.2 ```bool configure(rapidjson::Document d)``` {#SDD211 }

##### 2.10.4.2.1 SDD212 {#SDD212 }

This method **shall** consume an ```Encoders``` object from the configuration JSON and populate the ```encoders``` list with its contents.

##### 2.10.4.2.2 SDD213 {#SDD213 }

For each configured channel, this method **shall** either set the initial value to zero or to the value provided in the attached ```init``` element of the configuration JSON.

#### 2.10.4.3 SDD214 {#SDD214 }

```bool procMail(CMOOSMsg &msg)``` **shall** do nothing and return ```false```.

#### 2.10.4.4 SDD215 {#SDD215 }

```bool tick()``` **shall** read the corresponding encoder for each member of ```encoders``` that is not empty and publish the resulting value on the variable named by that element.

#### 2.10.4.5 SDD216 {#SDD216 }

```bool subscribe()``` **shall** do nothing and return ```true```.

#### 2.10.4.6 SDD217 {#SDD217 }

```bool isConfigured()``` **shall** return ```true``` if and only if ```encoders``` has at least one non-empty string.

### 2.10.5 SDD218 {#SDD218 }

Private Constructor

## 2.11 ```ADCBlock``` class {#SDD219 }

### 2.11.1 SDD220 {#SDD220 }

This class **shall** be a singleton

### 2.11.2 SDD221 {#SDD221 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.11.3 Private Members {#SDD222 }

#### 2.11.3.1 SDD223 {#SDD223 }

```static ADCBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.11.3.2 SDD224 {#SDD224 }

```std::list<std::string> rawChannels``` **shall** store the names of the raw output variables.

#### 2.11.3.3 SDD225 {#SDD225 }

```std::list<std::string> voltChannels``` **shall** store the names of the voltage scaled output variables.

### 2.11.4 Public Methods {#SDD226 }

#### 2.11.4.1 SDD227 {#SDD227 }

```static ADCBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.11.4.2 SDD228 {#SDD228 }

```bool configure(rapidjson::Document d)``` **shall** read the contents of the ```rawChannels``` and ```voltChannels``` elements into the corresponding members.

#### 2.11.4.3 SDD229 {#SDD229 }

```bool procMail(CMOOSMsg &msg)``` **shall** do nothing and return ```false```.

#### 2.11.4.4 ```bool tick()``` {#SDD230 }

##### 2.11.4.4.1 SDD231 {#SDD231 }

This method **shall** publish the battery voltage returned by ```rc_get_battery_voltage()``` to ```BBBL_BATTERY_VOLTAGE```.

##### 2.11.4.4.2 SDD232 {#SDD232 }

This method **shall** publish the DC jack voltage returned by ```rc_get_dc_jack_voltage()``` to ```BBBL_JACK_VOLTAGE```.

##### 2.11.4.4.3 SDD233 {#SDD233 }

For each member of ```rawChannels``` that is not empty, this method **shall** publish the result of calling ```rc_adc_raw()``` against that channel.

##### 2.11.4.4.4 SDD234 {#SDD234 }

For each member of ```voltChannels``` that is not empty, this method **shall** publish the result of calling ```rc_adc_volt()``` against that channel.

#### 2.11.4.5 SDD235 {#SDD235 }

```bool subscribe()``` **shall** do nothing and return ```true```.

#### 2.11.4.6 SDD236 {#SDD236 }

```bool isConfigured()``` **shall** return ```true```.

## 2.12 ```ServoBlock``` class {#SDD237 }

### 2.12.1 SDD238 {#SDD238 }

This class **shall** be a singleton

### 2.12.2 SDD239 {#SDD239 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.12.3 Private Members {#SDD240 }

#### 2.12.3.1 SDD241 {#SDD241 }

```static ServoBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.12.3.2 SDD242 {#SDD242 }

```std:list<ServoChannel> servos``` **shall** store the servo channels specified by the configuration JSON.

### 2.12.4 Public Methods {#SDD243 }

#### 2.12.4.1 SDD244 {#SDD244 }

```static ServoBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.12.4.2 ```bool configure(rapidjson::Document d)``` {#SDD245 }

##### 2.12.4.2.1 SDD246 {#SDD246 }

This method **shall** consume a ```Servos``` object from the configuration JSON.

##### 2.12.4.2.2 SDD247 {#SDD247 }

It **shall** iterate over the contained JSON array.

##### 2.12.4.2.3 SDD248 {#SDD248 }

For each non-null element, this method **shall** construct a ```ServoChannel``` object and emplace it at the end of the ```servos``` list.

##### 2.12.4.2.4 SDD249 {#SDD249 }

The elements **shall** be written to the constructor parameters with matching names.

##### 2.12.4.2.5 SDD250 {#SDD250 }

The channel number of any emplaced ```ServoChannel``` objects **shall** be their JSON array index.

##### 2.12.4.2.6 SDD251 {#SDD251 }

After iterating through the contained JSON array, this method **shall** iterate through ```servos``` calling each element's ```startThread()``` method.

#### 2.12.4.3 ```bool procMail(CMOOSMsg &msg)``` {#SDD252 }

##### 2.12.4.3.1 SDD253 {#SDD253 }

If ```msg.GetKey()``` matches ```BBBL_SERVO_PWR``` and the value is a ```BINARY```, this method **shall** call either ```rc_enable_servo_power_rail()``` or ```rc_disable_servo_power_rail()``` depending on whether the value is ```true``` or ```false```. This method **shall** return the success or failure of the called function.

##### 2.12.4.3.2 SDD254 {#SDD254 }

This method **shall** pass ```msg.GetKey()``` to each member of ```servos```'s ```matchVar()``` method.

##### 2.12.4.3.3 SDD255 {#SDD255 }

For each call that returns ```true```, this method **shall** check that the value of the named variable is DOUBLE and then pass it to that member's ```setServo()``` method.

##### 2.12.4.3.4 SDD256 {#SDD256 }

This method **shall** return the logical AND of all called ```setServo()``` methods.

##### 2.12.4.3.5 SDD257 {#SDD257 }

If there is no match to ```msg.GetKey()```, this method **shall** return ```false```.

##### 2.12.4.3.6 SDD258 {#SDD258 }

```bool tick()``` **shall** do nothing and return ```true```.

##### 2.12.4.3.7 SDD259 {#SDD259 }

```bool subscribe()``` **shall** iterate through the members of ```servos```, if any, subscribing to the variable names returned by calling ```getVar()``` on each one. It **shall** then return ```true```.

##### 2.12.4.3.8 SDD260 {#SDD260 }

```bool isConfigured()``` **shall** return ```true``` if and only if ```servos``` has at least one member.

## 2.13 ```IMURandomBlock``` class {#SDD261 }

### 2.13.1 SDD262 {#SDD262 }

This class **shall** be a singleton

### 2.13.2 SDD263 {#SDD263 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.13.3 Private Members {#SDD264 }

#### 2.13.3.1 SDD265 {#SDD265 }

```static IMURandomBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.13.3.2 SDD266 {#SDD266 }

```rc_imu_data_t data``` **shall** store the IMU data

#### 2.13.3.3 SDD267 {#SDD267 }

```rc_imu_config_t conf``` **shall** store the IMU configuration

#### 2.13.3.4 SDD268 {#SDD268 }

```bool configured``` **shall** be set true if and only if this block has been configured.

### 2.13.4 Public Methods {#SDD269 }

#### 2.13.4.1 SDD270 {#SDD270 }

```static IMURandomBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.13.4.2 ```bool configure(rapidjson::Document d)``` {#SDD271 }

##### 2.13.4.2.1 SDD272 {#SDD272 }

This method **shall** check if ```IMUDMPBlock``` has been configured, and return ```false``` if it has.

##### 2.13.4.2.2 SDD273 {#SDD273 }

This method **shall** consume an ```IMU``` object from the configuration JSON.

##### 2.13.4.2.3 SDD274 {#SDD274 }

This method **shall** check the value of the ```mode``` element in the configuration JSON and return ```false``` if it is not present and equal to ```Random```.

##### 2.13.4.2.4 SDD275 {#SDD275 }

This method **shall** call ```rc_set_imu_config_to_defaults()``` to set the configuration to defaults. If this fails, this method **shall** return false.

##### 2.13.4.2.5 SDD276 {#SDD276 }

If a ```magnetometer``` entry is present and ```true```, this method **shall** set ```conf.enable_magnetometer``` to 1.

##### 2.13.4.2.6 SDD277 {#SDD277 }

If an ```accelRange``` entry is present, this method **shall** set ```conf.accel_fsr``` to the given value.

##### 2.13.4.2.7 SDD278 {#SDD278 }

If a ```gyroRange``` entry is present, this method **shall** set ```conf.gyro_fsr``` to the given value.

##### 2.13.4.2.8 SDD279 {#SDD279 }

If an ```accelFilter``` entry is present, this method **shall** set ```conf.accel_dlpf``` to the given value.

##### 2.13.4.2.9 SDD280 {#SDD280 }

If a ```gyroFilter``` entry is present, this method **shall** set ```conf.gyro_dlpf``` to the given value.

##### 2.13.4.2.10 SDD281 {#SDD281 }

This method **shall** configure the IMU by calling ```rc_initialize_imu()``` and returning **false** if it fails.

##### 2.13.4.2.11 SDD282 {#SDD282 }

Once all other steps have been executed, this method **shall** set ```configured``` to true and return ```true```.

#### 2.13.4.3 SDD283 {#SDD283 }

```bool procMail(CMOOSMsg &msg)``` **shall** do nothing and return ```false```.

#### 2.13.4.4 ```bool tick()``` {#SDD284 }

##### 2.13.4.4.1 SDD285 {#SDD285 }

This method **shall** sample the accelerometer by calling ```rc_read_accel_data()``` and return ```false``` if it fails.

##### 2.13.4.4.2 SDD286 {#SDD286 }

This method **shall** sample the gyros by calling ```rc_read_gyro_data()``` and return ```false``` if it fails.

##### 2.13.4.4.3 SDD287 {#SDD287 }

This method **shall** sample the magnetometer by calling ```rc_read_mag_data()``` and return ```false``` if it fails.

##### 2.13.4.4.4 SDD288 {#SDD288 }

This method **shall** sample the temperature by calling ```rc_read_temp_data()``` and return ```false``` if it fails.

##### 2.13.4.4.5 SDD289 {#SDD289 }

This method **shall** calculate the compass bearing of the positive X axis with the tilt compensation algorithm described in http://www.cypress.com/file/130456/download and publish it to ```BBBL_IMU_COMPASS``` as a ```DOUBLE```.

##### 2.13.4.4.6 SDD290 {#SDD290 }

This method **shall** publish the temperature to ```BBBL_IMU_TEMP``` as a ```DOUBLE```.

##### 2.13.4.4.7 SDD291 {#SDD291 }

This method **shall** pack the accelerometer data into a JSON string as described in schema/imu_tuple_schema.json and publish it to the variable ```BBBL_IMU_ACCEL```.

##### 2.13.4.4.8 SDD292 {#SDD292 }

This method **shall** pack the gyro data into a JSON string as described in schema/imu_tuple_schema.json and publish it to the variable ```BBBL_IMU_GYRO```.

##### 2.13.4.4.9 SDD293 {#SDD293 }

This method **shall** pack the magnetometer data into a JSON string as described in schema/imu_tuple_schema.json and publish it to the variable ```BBBL_IMU_MAG```.

##### 2.13.4.4.10 SDD294 {#SDD294 }

Once all other steps have been executed, this method **shall** return ```true```.

#### 2.13.4.5 SDD295 {#SDD295 }

```bool subscribe()``` **shall** do nothing and return ```true```.

#### 2.13.4.6 SDD296 {#SDD296 }

```bool isConfigured()``` **shall** return the value of ```configured````.

## 2.14 ```IMUDMPBlock``` class {#SDD297 }

### 2.14.1 SDD298 {#SDD298 }

This class **shall** be a singleton

### 2.14.2 SDD299 {#SDD299 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.14.3 Private Members {#SDD300 }

#### 2.14.3.1 SDD301 {#SDD301 }

```static IMUDMPBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.14.3.2 SDD302 {#SDD302 }

```rc_imu_data_t data``` **shall** store the IMU data

#### 2.14.3.3 SDD303 {#SDD303 }

```rc_imu_config_t conf``` **shall** store the IMU configuration

#### 2.14.3.4 SDD304 {#SDD304 }

```bool configured``` **shall** be set true if and only if this block has been configured.

### 2.14.4 Public Methods {#SDD305 }

#### 2.14.4.1 SDD306 {#SDD306 }

```static IMUDMPBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.14.4.2 ```bool configure(rapidjson::Document d)``` {#SDD307 }

##### 2.14.4.2.1 SDD308 {#SDD308 }

This method **shall** check if ```IMURandomBlock``` has been configured, and return ```false``` if it has.

##### 2.14.4.2.2 SDD309 {#SDD309 }

This method **shall** consume an ```IMU``` object from the configuration JSON.

##### 2.14.4.2.3 SDD310 {#SDD310 }

This method **shall** check the value of the ```mode``` element in the configuration JSON and return ```false``` if it is not present and equal to ```DMP```.

##### 2.14.4.2.4 SDD311 {#SDD311 }

This method **shall** call ```rc_set_imu_config_to_defaults()``` to set the configuration to defaults. If this fails, this method **shall** return false.

##### 2.14.4.2.5 SDD312 {#SDD312 }

If a ```magnetometer``` entry is present and ```true```, this method **shall** set ```conf.enable_magnetometer``` to 1.

##### 2.14.4.2.6 SDD313 {#SDD313 }

If an ```accelRange``` entry is present, this method **shall** set ```conf.accel_fsr``` to the given value.

##### 2.14.4.2.7 SDD314 {#SDD314 }

If a ```gyroRange``` entry is present, this method **shall** set ```conf.gyro_fsr``` to the given value.

##### 2.14.4.2.8 SDD315 {#SDD315 }

If an ```accelFilter``` entry is present, this method **shall** set ```conf.accel_dlpf``` to the given value.

##### 2.14.4.2.9 SDD316 {#SDD316 }

If a ```gyroFilter``` entry is present, this method **shall** set ```conf.gyro_dlpf``` to the given value.

##### 2.14.4.2.10 SDD317 {#SDD317 }

If an ```orientation``` entry is present, this method **shall** set ```conf.orientation``` to the given value.

##### 2.14.4.2.11 SDD318 {#SDD318 }

If a ```dmpSampleRate``` entry is present, this method **shall** set ```conf.dmp_sample_rate``` to the given value.

##### 2.14.4.2.12 SDD319 {#SDD319 }

If a ```dmpCompassTimeConstant``` entry is present, this method **shall** set ```conf.compass_time_constant``` to the given value.

##### 2.14.4.2.13 SDD320 {#SDD320 }

This method **shall** configure the IMU by calling ```rc_initialize_imu_dmp()``` and returning **false** if it fails.

##### 2.14.4.2.14 SDD321 {#SDD321 }

Once all other steps have been executed, this method **shall** set ```configured``` to true and return ```true```.

#### 2.14.4.3 SDD322 {#SDD322 }

```bool procMail(CMOOSMsg &msg)``` **shall** do nothing and return ```false```.

#### 2.14.4.4 ```bool tick()``` {#SDD323 }

##### 2.14.4.4.1 SDD324 {#SDD324 }

This method **shall** pack the contents of ```data.fused_TaitBryan``` into a JSON string conforming to schema/imu_tuple_schema.json and publish it to ```BBBL_DMP_TBA```.

##### 2.14.4.4.2 SDD325 {#SDD325 }

This method **shall** pack the contents of ```data.fused_quat``` into a JSON string conforming to schema/imu_quaternion_schema.json and publish it to ```BBBL_DMP_QT```.

##### 2.14.4.4.3 SDD326 {#SDD326 }

This method **shall** publish the contents of ```data.compass_heading``` to ```BBBL_DMP_HEADING```.

##### 2.14.4.4.4 SDD327 {#SDD327 }

This method **shall** publish the contents of ```data.compass_heading_raw``` to ```BBBL_DMP_HEADING_RAW```.

#### 2.14.4.5 SDD328 {#SDD328 }

```bool subscribe()``` **shall** do nothing and return ```true```.

#### 2.14.4.6 SDD329 {#SDD329 }

```bool isConfigured()``` **shall** return the value of ```configured````.

## 2.15 ```BaroBlock``` class {#SDD330 }

### 2.15.1 SDD331 {#SDD331 }

This class **shall** be a singleton

### 2.15.2 SDD332 {#SDD332 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.15.3 Private Members {#SDD333 }

#### 2.15.3.1 SDD334 {#SDD334 }

```static BaroBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.15.3.2 SDD335 {#SDD335 }

```double seaLevelPressure``` **shall** store the sea level pressure in Pascals for calculation of altitude. Default value **shall** be 101325.

#### 2.15.3.3 SDD336 {#SDD336 }

```bool configured``` **shall** be set true if and only if this block has been configured.

### 2.15.4 Public Methods {#SDD337 }

#### 2.15.4.1 SDD338 {#SDD338 }

```static BaroBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.15.4.2 ```bool configure(rapidjson::Document d)``` {#SDD339 }

##### 2.15.4.2.1 SDD340 {#SDD340 }

This method **shall** consume a ```Barometer``` object from the configuration JSON.

##### 2.15.4.2.2 SDD341 {#SDD341 }

This method **shall** call ```rc_initialize_barometer()``` with th values of the ```oversample``` and ```filter``` elements.

##### 2.15.4.2.3 SDD342 {#SDD342 }

If this call succeeds, this method **shall** set ```configured``` to ```true```.

##### 2.15.4.2.4 SDD343 {#SDD343 }

This method **shall** return the value of ```configured```.

#### 2.15.4.3 SDD344 {#SDD344 }

```bool procMail(CMOOSMsg &msg)``` **shall** check if the incoming ```msg.GetKey()``` is equal to ```BBBL_BARO_SEA_LEVEL```. If it is and the type is ```DOUBLE```, it shall assign that value to ```seaLevelPressure``` and return ```true```. Otherwise, it **shall** return ```false```.

#### 2.15.4.4 ```bool tick()``` {#SDD345 }

##### 2.15.4.4.1 SDD346 {#SDD346 }

This method **shall** publish the return value of ```rc_bmp_get_temperature_c()``` to ```BBBL_BARO_TEMP```.

##### 2.15.4.4.2 SDD347 {#SDD347 }

This method **shall** publish the return value of ```rc_bmp_get_pressure_pa()``` to ```BBBL_BARO_PRES```.

##### 2.15.4.4.3 SDD348 {#SDD348 }

This method **shall** publish the return value of ```rc_bmp_get_altitude_m()``` to ```BBBL_BARO_ALT```.

#### 2.15.4.5 SDD349 {#SDD349 }

```bool subscribe()``` **shall** subscribe to the ```BBBL_BARO_SEA_LEVEL``` variable and return ```true```.

#### 2.15.4.6 SDD350 {#SDD350 }

```bool isConfigured()``` **shall** return the value of ```configured````.

## 2.16 ```GPIOblock``` class {#SDD351 }

### 2.16.1 SDD352 {#SDD352 }

This class **shall** be a singleton

### 2.16.2 SDD353 {#SDD353 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.16.3 Private Members {#SDD354 }

#### 2.16.3.1 SDD355 {#SDD355 }

```static GPIOblock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.16.3.2 SDD356 {#SDD356 }

```std::list<GPIOpin> pins``` **shall** store a list of the configured pins.

### 2.16.4 Public Methods {#SDD357 }

#### 2.16.4.1 SDD358 {#SDD358 }

```static GPIOblock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.16.4.2 ```bool configure(rapidjson::Document d)``` {#SDD359 }

##### 2.16.4.2.1 SDD360 {#SDD360 }

This method **shall** iterate over the members of the JSON array contained in the ```GPIO``` object.

##### 2.16.4.2.2 SDD361 {#SDD361 }

For each entry in the array, this method **shall** construct and emplace an entry to ```pins```.

#### 2.16.4.3 ```bool procMail(CMOOSMsg &msg)``` {#SDD362 }

##### 2.16.4.3.1 SDD363 {#SDD363 }

On each tick, this method **shall** iterate through each element of the ```pins``` list calling ```getMode()``` for each one.

##### 2.16.4.3.2 SDD364 {#SDD364 }

If the mode is ```GPIOmode::OUTPUT``` and the value of ```msg.GetKey()``` is equal to ```getVar()```, this method **shall** check that the incoming value is a BINARY and, if so, pass it to ```set()```.

##### 2.16.4.3.3 SDD365 {#SDD365 }

If it fails the check, this method **shall** return ```false```.

#### 2.16.4.4 ```bool tick()``` {#SDD366 }

##### 2.16.4.4.1 SDD367 {#SDD367 }

On each tick, this method **shall** iterate through each element of the ```pins``` list calling ```getMode()``` for each one.

##### 2.16.4.4.2 SDD368 {#SDD368 }

If the returned value is any input mode, this method **shall** call ```get()``` against that element and publish that value to the variable named by ```getVar()```.

#### 2.16.4.5 SDD369 {#SDD369 }

```bool subscribe()``` **shall** iterate through ```pins``` calling ```subscribe``` on each object in turn, and returning the logical OR of all results.

#### 2.16.4.6 SDD370 {#SDD370 }

```bool isConfigured()``` **shall** return ```true``` if and only if ```pins``` has at least one member.

### 2.16.5 SDD371 {#SDD371 }

This class **shall** have an explicit destructor that destroys all members of ```pins```.

## 2.17 ```PWMBlock``` class {#SDD372 }

### 2.17.1 SDD373 {#SDD373 }

This class **shall** be a singleton

### 2.17.2 SDD374 {#SDD374 }

This class **shall** be a subclass of ```FunctionBlock```

### 2.17.3 Private Members {#SDD375 }

#### 2.17.3.1 SDD376 {#SDD376 }

```static PWMBlock* s_instance``` **shall** store a pointer to the only instance of each sub-class.

#### 2.17.3.2 SDD377 {#SDD377 }

```std::list<PWMChannel> interfaces``` **shall** store a list of the configured PWM interfaces.

### 2.17.4 Public Methods {#SDD378 }

#### 2.17.4.1 SDD379 {#SDD379 }

```static PWMBlock* instance()``` **shall** return ```s_instance```, and instantiate it if it has not yet been instantiated (i.e. lazy instantiation).

#### 2.17.4.2 ```bool configure(rapidjson::Document d)``` {#SDD380 }

##### 2.17.4.2.1 SDD381 {#SDD381 }

This method **shall** consume a ```PWM``` object from the configuration JSON.

##### 2.17.4.2.2 SDD382 {#SDD382 }

If ```MotorsBlock::isConfigured()``` returns ```true``` and any elements other than the first (0) are present and non-null, this method **shall** throw a ```std::invalid_argument``` exception.

##### 2.17.4.2.3 SDD383 {#SDD383 }

For each element of the configuration JSON array, this method **shall** construct and emplace a ```PWMChannel``` object on the end of the ```interfaces``` list.

#### 2.17.4.3 ```bool procMail(CMOOSMsg &msg)``` {#SDD384 }

##### 2.17.4.3.1 SDD385 {#SDD385 }

This method **shall** iterated through ```interfaces```, comparing the value of ```msg.GetKey()``` to the value of ```getVarA()```, ```getVarB()```, and ```getFreqVar``` for each element of ```interfaces``` in turn. This comparison **shall** be case insensitive.

##### 2.17.4.3.2 SDD386 {#SDD386 }

For any match found, this method **shall** check that the incoming type is ```DOUBLE```, pass it to the corresponding method, and return the result.

##### 2.17.4.3.3 SDD387 {#SDD387 }

Otherwise, this method **shall** return ```false```.

#### 2.17.4.4 SDD388 {#SDD388 }

```bool tick()``` **shall** do nothing and return ```true```.

#### 2.17.4.5 SDD389 {#SDD389 }

```bool subscribe()``` **shall** iterate through the members of ```interfaces``` and call the ```subscribe()``` method of each one in turn. It **shall** return the logical AND of the results.

#### 2.17.4.6 SDD390 {#SDD390 }

```bool isConfigured()``` **shall** return ```true``` if and only if ```interfaces``` has at least one member.

### 2.17.5 SDD391 {#SDD391 }

This class **shall** have an explicit destructor that destroys all members of ```interfaces```.

# 3.0 Free Functions {#SDD392 }

## 3.1 ```std::list<FunctionBlock*>* getActive(bool reset = false)``` **shall** return a pointer to a list of those blocks that are active and configured. {#SDD393 }

### 3.1.1 SDD394 {#SDD394 }

If the argument is ```true```, it **shall** clear its internal state and return a pointer to an empty list. This is provided to aid in unit testing and **must not** be used for any production purpose.

## 3.2 ```rapidjson::Document getConf(std::string conf)``` {#SDD395 }

### 3.2.1 SDD396 {#SDD396 }

This function **shall** take the supplied configuration JSON string and attempts to parse it.

### 3.2.2 SDD397 {#SDD397 }

This function **shall** throw a ```std::invalid_argument``` exception if the supplied JSON does not conform to schema/configuration_schema.json.

## 3.3 ```rapidjson::Document getConfFile(std::string confFile)``` {#SDD398 }

### 3.3.1 SDD399 {#SDD399 }

This function **shall** throw a ```std::filesystem::filesystem_error``` exception if the argument is a filename and the file cannot be found or cannot be read.

### 3.3.2 SDD400 {#SDD400 }

This function **shall** pass the JSON read from the file to ```getConf()``` and return the result.

## 3.4 ```bool configureBBBlue(rapidjson::Document conf)``` {#SDD401 }

### 3.4.1 SDD402 {#SDD402 }

This method **shall** return ```false``` if the list returned by ```getActive()``` is not empty.

### 3.4.2 SDD403 {#SDD403 }

For each configuration block that is present in the configuration JSON, this function **shall** add a pointer to the corresponding subclass of ```FunctionBlock``` and pass the configuration object to its ```configure()``` method.

### 3.4.3 SDD404 {#SDD404 }

This method **shall** return the logical and of the return values of the ```configure()``` methods called.

# 4.0 Startup Behavior {#SDD405 }

## 4.1 SDD406 {#SDD406 }

The ```OnStartup()``` method shall search the provided configuration for a ```conf``` or ```confFile``` entry and pass it to the appropriate function.

## 4.2 SDD407 {#SDD407 }

If both ```conf``` and ```confFile``` are present, this function **shall** log an error and exit the program.

# 5.0 MOOS Community Connection Behavior {#SDD408 }

## 5.1 SDD409 {#SDD409 }

The ```OnConnectToServer()``` method **shall** call ```getActive()``` to get the active list and iterate through it, calling the ```subscribe()``` method for each item.

# 6.0 New Mail Behavior {#SDD410 }

## 6.1 SDD411 {#SDD411 }

The ```OnNewMail()``` method **shall** pass each incoming message to the ```procMail()``` method of each item returned by ```getActive()```.

# 7.0 App Tick Behavior {#SDD412 }

## 7.1 SDD413 {#SDD413 }

The ```OnNewMail()``` method **shall** pass each incoming message to the ```tick()``` method of each item returned by ```getActive()```.

# 8 SDD414 {#SDD414 }

Appcast Reporting Behavior

