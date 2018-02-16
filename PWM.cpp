/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: PWM.cpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include "BBBlue_PWM.hpp"
#include "BBBlue_FunctionBlock.hpp"

extern "C" {
    #include "roboticscape.h"
}
using namespace std;
using namespace rapidjson;
using namespace BBBL;

PWMChannel::PWMChannel(int iface, int freq,
                        std::string varA, std::string varB,
                        bool isnsA = false, bool isnsB = false) :
                        freq(freq), varA(varA), varB(varB),
                        isnsA(isnsA), isnsB(isnsB)
                        {
                            period = (1e9/freq);
                            rc_pwm_init(iface, freq);
                        }

PWMChannel::PWMChannel(int iface, std::string freqVar,
                        std::string varA, std::string varB,
                        bool isnsA = false, bool isnsB = false) :
                        freqVar(freqVar), varA(varA), varB(varB),
                        isnsA(isnsA), isnsB(isnsB), freq(1000)
                        {
                            rc_pwm_init(iface, freq);
                        }

bool PWMChannel::setFrequency (int f) {
    freq = f;
    period = (1e9/freq);
    return !(rc_pwm_init(iface, freq));
}

bool PWMChannel::procMail (CMOOSMsg &msg) {
    string key = msg.GetKey();
    if (varA == key) {
        return setDutyA(msg.GetDouble());
    } else if (varB == key) {
        return setDutyB(msg.GetDouble());
    } else if ((freqVar != "") && (freqVar == key)) {
        freq = msg.GetDouble();
        return setFrequency(msg.GetDouble());
    }
    return false;
}

bool PWMChannel::setDutyA (double d) {
    if (isnsA) {
        if (d > period) d = period;
        if (d < 0.0f) d = 0.0f;
        A = d;
        return !(rc_pwm_set_duty_ns(iface, 'A', d));
    } else {
        if (d > 1.0f) d = 1.0f;
        if (d < 0.0f) d = 0.0f
        A = d;
        return !(rc_pwm_set_duty(iface, 'A', d));
    }
    return false;
}

bool PWMChannel::setDutyB (double d)  {
    if (isnsB) {
        if (d > period) d = period;
        if (d < 0) d = 0;
        B = d;
        return !(rc_pwm_set_duty_ns(iface, 'B', d));
    } else {
        if (d > 1.0f) d = 1.0f;
        if (d < 0.0f) d = 0.0f
        B = d;
        return !(rc_pwm_set_duty(iface, 'B', d));
    }
    return false;
}

bool PWMChannel::subscribe(BBBlue *b) {
    result = true;
    if (freqVar != "") result &= b->registerVar(freqVar);
    if (varA != "") result &= b->registerVar(varA);
    if (varB != "") result &= b->registerVar(varB);
}

PWMBlock* PWMBlock::instance() {
    if (!s_instance) s_instance = new PWMBlock();
    return s_instance;
}

bool PWMBlock::configure(rapidjson::Value &v) {
    if (v.IsArray()) {
        ctr = 0;
        for (auto &a: v.GetArray()) {
            if (ctr > 2) break;
            if (a.IsObject()) {
                string varA; string varB;
                bool Ans = false; Bns = false;
                if (a.HasMember("dutyCycleA")) {
                    varA = a["dutyCycleA"].GetString();
                } else if (a.HasMember("dutyCycleAns")) {
                    varA = a["dutyCycleAns"].GetString();
                    Ans = true;
                }
                if (a.HasMember("dutyCycleB")) {
                    varB = a["dutyCycleB"].GetString();
                } else if (a.HasMember("dutyCycleBns")) {
                    varB = a["dutyCycleBns"].GetString();
                    Bns = true;
                }
                if (a["frequency"].IsString()) {
                    servos.push_back(unique_ptr<PWMChannel>(new PWMChannel(ctr, a["frequency"].GetString(), varA, varB, Ans, Bns)));
                    configured = true;
                } else if (a["frequency"].IsNumber()) {
                    servos.push_back(unique_ptr<PWMChannel>(new PWMChannel(ctr, a["frequency"].GetDouble(), varA, varB, Ans, Bns)));
                    configured = true;
                } else {
                    servos.push_back(unique_ptr<PWMChannel>(nullptr));
                }
            } else {
                servos.push_back(unique_ptr<PWMChannel>(nullptr));
            }
            ctr++;
        }
    }
    return configured;
}

bool PWMBlock::procMail(CMOOSMsg &msg) {
        result = false;
        for (auto &s: interfaces) result |= s->procMail(msg);
        return result;
}

bool PWMBlock::subscribe(BBBlue *b) {
    result = true;
    for (auto &s: interfaces) result &= s->subscribe(b);
    return result;
}

ACTable PWMBlock::buildReport() {
    int ctr = 0;
    ACTable actab(interfaces.size());
    if (!configured) return actab;
    for (auto &p: interfaces) {
        if (p) {
            if (p->varA != "") {
                string hdr = "PWM " + to_string(ctr) + "-A";
                actab << hdr;
            }
            if (p->varB != "") {
                string hdr = "PWM " + to_string(ctr) + "-B";
                actab << hdr;
            }
        }
    }
    actab.addHeaderLines();
    for (auto &p: interfaces) {
        if (p) {
            if (p->varA != "") actab << to_string(p->getA());
            if (p->varB != "") actab << to_string(p->getB());
        }
    }
    return actab;
}

PWMBlock* PWMBlock::s_instance = nullptr;
