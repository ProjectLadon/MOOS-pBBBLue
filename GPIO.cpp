/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: GPIO.cpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include "BBBlue_GPIO.hpp"
#include "BBBlue_FunctionBlock.hpp"

extern "C" {
    #include "roboticscape.h"
}
using namespace std;
using namespace rapidjson;
using namespace BBBL;

GPIOpin::GPIOpin(int pin, GPIOmode mode, std::string var, bool state = false) : pin(pin), var(var), state(state), mode(mode) {
        rc_gpio_export(pin);
        if (GPIOmode::OUTPUT) {
            rc_gpio_set_dir(pin, OUTPUT_PIN);
        } else {
            rc_gpio_set_dir(pin, INPUT_PIN);
        }
}

bool GPIOpin::procMail(CMOOSMsg &msg) {
    if (msg.GetKey() == var) {
        if (msg.IsBinary()) {
            if (msg.GetBinaryData(data)) {
                unsigned char *data = msg.GetBinaryData();
                set(data[0]);
                return true;
            }
        } else if (msg.IsString()) {
            string val = toupper(msg.GetString());
            if (("ON" == val) || ("TRUE" == val) || ("T" == val)) {
                set(true);
            } else set(false);
            return true;
        } else if (msg.IsDouble()) {
            double val = msg.GetDouble();
            if (val > 0.5) {
                set(true);
            } else set(false);
            return true;
        } else return false;
    }
    return false;
}

bool GPIOpin::tick(BBBlue *b) {
    if (mode == GPIOmode::OUTPUT) {
        return rc_gpio_set_value(pin, state);
    } else {
        int ret, result;
        result = rc_gpio_get_value(pin, &ret);
        state = ret;
        uint8_t val = 0;
        if (state) val = 0xff;
        b->notify(var, val);
        return result;
    }
    return true;
}

bool GPIOpin::subscribe(BBBlue *b) {
    if (var != "") {
        return b->registerVar(var);
    } else return false;
}

static GPIOBlock* GPIOBlock::instance() {
    if (!s_instance) s_instance = new GPIOBlock();
    return s_instance;
}

bool GPIOBlock::configure(rapidjson::Value &v) {
    if (v.IsArray()) {
        for (auto &g: v.GetArray()) {
            if (g.IsObject()) {
                GPIOmode mode;
                if ("OUTPUT" == g["function"].GetString()) {
                    mode = GPIOmode::OUTPUT;
                } else if ("INPUT" == g["function"].GetString()) {
                    mode = GPIOmode::INPUT;
                } else continue;
                if ("GPIO1_25" == g["gpioName"].GetString()) {
                    pins.push_back(unique_ptr<GPIOpin>(new GPIOpin(GPIOpin::GPIO1_25, mode, g["gpioVar"].GetString())));
                    configured = true;
                } else if ("GPIO1_17" == g["gpioName"].GetString()) {
                    pins.push_back(unique_ptr<GPIOpin>(new GPIOpin(GPIOpin::GPIO1_17, mode, g["gpioVar"].GetString())));
                    configured = true;
                } else if ("GPIO3_20" == g["gpioName"].GetString()) {
                    pins.push_back(unique_ptr<GPIOpin>(new GPIOpin(GPIOpin::GPIO3_20, mode, g["gpioVar"].GetString())));
                    configured = true;
                } else if ("GPIO3_17" == g["gpioName"].GetString()) {
                    pins.push_back(unique_ptr<GPIOpin>(new GPIOpin(GPIOpin::GPIO3_17, mode, g["gpioVar"].GetString())));
                    configured = true;
                } else if ("GPIO3_2" == g["gpioName"].GetString()) {
                    pins.push_back(unique_ptr<GPIOpin>(new GPIOpin(GPIOpin::GPIO3_2, mode, g["gpioVar"].GetString())));
                    configured = true;
                } else if ("GPIO3_1" == g["gpioName"].GetString()) {
                    pins.push_back(unique_ptr<GPIOpin>(new GPIOpin(GPIOpin::GPIO3_1, mode, g["gpioVar"].GetString())));
                    configured = true;
                } else continue;
            }
        }
    }
    if (pins.size() > 0) {return true;}
    return false;
}

bool GPIOBlock::procMail(CMOOSMsg &msg) {
    bool result = false;
    for (auto &g: pins) result |= g->procMail(msg);
    return result;
}

bool GPIOBlock::tick(BBBlue *b) {
    bool result = true;
    for (auto &g: pins) result &= g->tick(b);
    return result;
}

bool GPIOBlock::subscribe(BBBlue *b) {
    bool result = true;
    for (auto &g: pins) result &= g->subscribe(b);
    return result;
}

ACTable GPIOBlock::buildReport() {
    ACTable actab(pins.size());
    if (!configured) return actab;
    for (auto &p: pins) {
        string hdr = "GPIO" + to_string(g.getPin())
        actab << hdr;
    }
    actab.addHeaderLines();
    for (auto &p: pins) {
        string hdr = to_string(g.getState());
        actab << hdr;
    }
    return actab;
}

GPIOBlock* GPIOBlock::s_instance = nullptr;
