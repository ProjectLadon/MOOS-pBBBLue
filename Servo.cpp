/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: Servo.cpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include "BBBlue_Servo.hpp"
#include "BBBlue_FunctionBlock.hpp"

extern "C" {
    #include "roboticscape.h"
}
using namespace std;
using namespace rapidjson;
using namespace BBBL;

ServoChannel::ServoChannel(ServoMode action,
                            int chan,
                            std::chrono::duration _period = defaultPeriod,
                            std::string variable,
                            long maxServo = defaultMax,
                            long minServo = defaultMin,
                            long center = defaultCenter,
                            invert = defaultInvert) :
                            mode(action),
                            channel(chan),
                            period(_period),
                            var(variable),
                            max(maxServo),
                            min(minServo),
                            center(center),
                            invert(invert) {
    startThread();
}

bool ServoChannel::setServo(double s) {
    switch (mode) {
        case (ServoMode::servo90):
            return setServo90(s);
            break;
        case (ServoMode::servo180):
            return setServo180(s);
            break;
        case (ServoMode::servo360):
            return setServo360(s);
            break;
        case (ServoMode::ESC):
            return setServoESC(s);
            break;
        case (ServoMode::micros):
            return setServoMicros(s);
            break;
        default:
    }
}

void ServoChannel::startThread() {
    killThread = false;
    servoThread = new std::thread(threadRunner);
    servoThread->detach();
}

bool ServoChannel::procMail(CMOOSMsg &msg) {
    if (msg.GetKey() == var) {
        if (msg.IsDouble()) {
            return setServo(msg.GetDouble());
        }
    }
    return false;
}

bool ServoChannel::setServo90(double s) {return servoMap(s, 90.0, 45.0, 0.0);}

bool ServoChannel::setServo180(double s) {return servoMap(s, 180.0, 90.0, 0.0);}

bool ServoChannel::setServo360(double s) {return servoMap(s, 360.0, 180.0, 0.0);}

bool ServoChannel::setServoESC(double s) {return servoMap(s, 1.0, 0.5, 0.0);}

bool ServoChannel::servoMap(double s, double inmax, double incenter, double inmin) {
    if (s < inmin) s = inmin;
    if (s > inmax) s = inmax;
    if (invert) s = (inmax - s);
    if (s < incenter) {
        s = (s - inmin) * (center - min) / (incenter - inmin) + min;
    } else {
        s = (s - incenter) * (max - center) / (inmax - incenter) + center;
    }
    return setServoMicros(s);
}

bool ServoChannel::setServoMicros(double s) {
    if (s <  min) s = min;
    if (s > max) s = max;
    micros = s;
    return true;
}

void ServoChannel::threadRunner() {
    while (!killThread) {
        auto endtime = std::chrono::system_clock::now() + period;
        pulse();
        std::this_thread::sleep_until(endtime);
    }

}

void ServoChannel::pulse() {
    rc_send_servo_pulse_us(channel, micros);
}

ServoMode ServoChannel::getAction(std::string act) {
    if ("servo90" == act) {
        return ServoMode::servo90;
    } else if ("servo180" == act) {
        return ServoMode::servo180;
    } else if ("servo360" == act) {
        return ServoMode::servo360;
    } else if ("micros" == act) {
        return ServoMode::micros;
    } else if ("ESC" == act) {
        return ServoMode::ESC;
    } else return ServoMode::None;
}

static ServoBlock* ServoBlock::instance() {
    if (!s_instance) s_instance = new ServoBlock();
    return s_instance;
}

bool ServoBlock::configure(rapidjson::Value &v) {
    int chanctr = 0;
    for (auto &s: v.GetArray()) {
        if (s.IsNull()) {
            servos.push_back(std::unique_ptr<ServoChannel>(nullptr));
        } else {
            std::chrono::duration period = ServoChannel::defaultPeriod;
            long min = ServoChannel::defaultMin;
            long max = ServoChannel::defaultMax;
            long center = ServoChannel::defaultCenter;
            bool invert = ServoChannel::defaultInvert;
            ServoMode act = ServoChannel::getAction(s["action"].GetString());
            std::string var = s["name"].GetString();
            if (s.HasMember("period")) period = 1ms * s["period"].GetInt();
            if (s.HasMember("minServo")) min = s["minServo"].GetInt();
            if (s.HasMember("maxServo")) max = s["maxServo"].GetInt();
            if (s.HasMember("center")) center = s["center"].GetInt();
            if (s.HasMember("invert")) invert = s["invert"].GetBool();
            servos.push_back(std::unique_ptr<ServoChannel>(new ServoChannel(act, chanctr, period, var, max, min, center, invert)));
        }
        chanctr++;
    }
    return isConfigured();
}

bool ServoBlock::procMail(CMOOSMsg &msg) {
    bool result = false;
    for (auto &s: servos) {
        if (s) result |= s->procMail(msg);
    }
    return result;
}

bool ServoBlock::subscribe(BBBlue *b) {
    bool result = true;
    for (auto &s: servos) {
        if (s) result &= s->subscribe(b);
    }
    return result;
}

bool ServoBlock::isConfigured() {
    if (servos.size() > 0) return true;
    return false;
}

ACTable ServoBlock::buildReport() {
    ACTable actab(servos.size());
    int ctr = 0;
    for (auto &s: servos) {
        actab << ("Channel" + to_string(ctr));
        ctr++;
    }
    actab.addHeaderLines();
    for (auto &s: servos) actab << to_string((int)(s->getMode()));
    for (auto &s: servos) actab << to_string(s->getMicros());

    return actab;
}

ServoBlock* ServoBlock::s_instance = nullptr;
