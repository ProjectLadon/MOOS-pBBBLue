/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: Outputs.cpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <streambuf>
#include <memory>
#include <cstdlib>
#include "BBBlue_FunctionBlock.hpp"
#include "moosbool.h"
#include "schema/motor_input_schema.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"

extern "C" {
    #include "roboticscape.h"
}

using namespace std;
using namespace rapidjson;
using namespace BBBL;

LEDBlock* LEDBlock::instance() {
    if (!s_instance) s_instance = new LEDBlock();
    return s_instance;
}

bool LEDBlock::configure(rapidjson::Value &v) {
    if (v.HasMember("RED") && v["RED"].IsString()) REDname = v["RED"].GetString();
    if (v.HasMember("GREEN") && v["GREEN"].IsString()) GRNname = v["GREEN"].GetString();
    return isConfigured();
}

bool LEDBlock::procMail(CMOOSMsg &msg) {
    string key = msg.GetKey();
    if ((key != "") && (key == GRNname)) {
        led = GREEN;
    } else if ((key != "") && (key == REDname)) {
        led = RED;
    } else return false;

    // Check the incoming message... return false if the rc_set_led() call fails
    if (moosbool::instance()->isTrue(msg)) {
        if (rc_set_led(led, 1)) return false;
        return true;
    } else {
        if (rc_set_led(led, 0)) return false;
        return true;
    }
    return false;
}

bool LEDBlock::subscribe(BBBlue *b) {
    return (b->registerVar(REDname) && b->registerVar(GRNname));
}

bool LEDBlock::isConfigured() {
    return ((REDname != "") || (GRNname != ""));
}

ACTable LEDBlock::buildReport() {
    ACTable actab(2);
    actab << "Green | Red";
    actab.addHeaderLines();
    actab << rc_get_led(GREEN);
    actab << rc_get_led(RED);
    return actab;
}

MotorBlock* MotorBlock::instance() {
    if (!s_instance) s_instance = new MotorBlock();
    return s_instance;
}

MotorBlock::MotorBlock () {
    rc_disable_motors();
    rapidjson Document d;
    if (d.Parse(reinterpret_cast<char*>(motor_input_schema_json), motor_input_schema_json_len).HasParseError()) {
        cerr << "JSON parse error " << GetParseError_En(d.GetParseError());
        cerr << " in motor message schema at offset " << d.GetErrorOffset() << endl;
        std::abort();
    }
    sd = new SchemaDocument(d);
    validator = new SchemaValidator(sd);
}

MotorBlock::~MotorBlock () {
    delete sd;
    delete validator;
}

bool MotorBlock::configure(rapidjson::Value &v) {
    for (auto &m: v.GetArray()) {
        if (m.IsString()) {
            motors.push_back(m.GetString());
            configured = true;
        } else {
            motors.push_back("");
        }
    }
    if (configured) {rc_enable_motors();}
    return configured;
}

bool MotorBlock::procMail(CMOOSMsg &msg) {
    std::string key = msg.GetKey();
    if (msg.IsString()) {
        int cnt = 0;
        for (auto &m: motors) {
            cnt++;  // note that the motor control ranges from 0-3, so we increment first so we get the right motor
            if ((m != "") && (m == key)) return setMotor(cnt, msg.GetString());
        }
    }
    if (key == "BBBL_MOTORS_ACTIVE") {
        if (moosbool::instance()->isTrue(msg)) {
            rc_enable_motors();
            enabled = true;
        } else {
            rc_disable_motors();
            disabled = true;
        }
        return true;
    } else if (msg.IsDouble() && (key == "BBBL_MOTORS0_FREQUENCY")) {
        if (rc_pwm_init(1, static_cast<int>(msg.GetDouble()))) return false; // rc_pwm_init() returns non-zero on failure
        return true;
    } else if (msg.IsDouble() && (key == "BBBL_MOTORS1_FREQUENCY")) {
        if (rc_pwm_init(2, static_cast<int>(msg.GetDouble()))) return false; // rc_pwm_init() returns non-zero on failure
        return true;
    }
    return false;
}

bool MotorBlock::subscribe(BBBlue *b) {
    for (auto &m: motors) {
        if (m != "") b->registerVar(m);
    }
    return true;
}

ACTable MotorBlock::buildReport() {
    ACTable actab(5);
    actab << "Enabled|Motor 1|Motor 2|Motor 3|Motor 4";
    for (int i = 0; i < 4; i++) {
        if (isBrake[i]) {
            actab << "BRAKE";
        } else if (isFree[i]) {
            actab << "FREE";
        } else actab << to_string(throttle[i]);
    }
    return actab;
}

bool MotorBlock::setMotor(int motor, std::string val) {
    rapidjson::Document d;
    if (d.Parse(val).HasParseError()) {
        cerr << "JSON parse error " << GetParseError_En(d.GetParseError());
        cerr << " in incoming motor message at offset " << d.GetErrorOffset() << endl;
        return false;
    }
    if (!d.Accept(*validator)) {
        StringBuffer buf;
        Writer<StringBuffer> writer(buf);
        d.Accept(writer);
        cerr << "Motor JSON failed validation :" << endl;
        cerr << "\t" << buf.GetString() << endl;
        return false;
    }
    if (d.IsDouble()) {
        if (rc_set_motor(motor, d.GetDouble())) return false;   // rc_set_motor() returns non-zero on failure
        throttle[(motor - 1)] = d.GetDouble();
        isFree[(motor - 1)] = false;
        isBrake[(motor - 1)] = false;
        return true;
    } else if (d.IsString()) {
        if (d.GetString() == "FREE") {
            if (rc_set_motor_free_spin(motor)) return false; // rc_set_motor_free_spin() returns non-zero on failure
            isFree[(motor - 1)] = true;
            isBrake[(motor - 1)] = false;
        } else if (d.GetString() == "BRAKE") {
            if(rc_set_motor_brake(motor)) return false; // rc_set_motor_brake() returns non-zero on failure
            isFree[(motor - 1)] = false;
            isBrake[(motor - 1)] = true;
        } else return false;
        return true;
    }
    return false;
}

LEDBlock* LEDBlock::s_instance = nullptr
MotorBlock* MotorBlock::s_instance = nullptr
