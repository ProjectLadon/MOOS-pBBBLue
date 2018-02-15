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

PWMBlock::PWMBlock* instance() {
    if (!s_instance) s_instance = new PWMBlock();
    return s_instance;
}

bool PWMBlock::configure(rapidjson::Value &v);

bool PWMBlock::procMail(CMOOSMsg &msg) {
        result = false;
        for (auto &s: servos) result |= s->procMail(msg);
        return result;
}

bool PWMBlock::subscribe(BBBlue *b) {
    result = true;
    for (auto &s: servos) result &= s->subscribe(b);
    return result;
}

ACTable PWMBlock::buildReport();

PWMBlock* PWMBlock::s_instance = nullptr;
