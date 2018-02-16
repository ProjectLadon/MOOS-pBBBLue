/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: BBBlue_Servo.hpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

#ifndef BBBLUE_SERVO_H
#define BBBLUE_SERVO_H

namespace BBBL {

enum class ServoMode { servo90, servo180, servo360, micros, ESC, None };

class ServoChannel {
    public:
        // constants
        static const std::chrono::duration defaultPeriod;
        static const long defaultMax;
        static const long defaultMin;
        static const long defaultCenter;
        static const bool defaultInvert;

        ServoChannel(ServoMode action,
                    int chan,
                    std::chrono::duration _period = defaultPeriod,
                    std::string variable,
                    long maxServo = defaultMax,
                    long minServo = defaultMin,
                    long center = defaultCenter,
                    invert = defaultInvert);
        const ServoMode &getMode() {return mode;};
        const std::string &getVar() {return var;};
        const std::chrono:duration &getPeriod() {return period};
        const double &getMicros() {return micros;};
        bool subscribe(BBBlue *b) {return b->registerVar(var);};
        bool setServo(double s);
        void startThread();
        void stopThread() {killThread = true;};
        bool procMail(CMOOSMsg &msg);
        static ServoMode getAction(std::string act);
        ~ServoChannel() {killThread = true;};

    private:
        bool setServo90(double s);
        bool setServo180(double s);
        bool setServo360(double s);
        bool setServoESC(double s);
        bool servoMap(double s, double inmax, double incenter, double inmin);
        bool setServoMicros(double s);
        void threadRunner();
        void pulse();

        ServoMode mode;
        int channel;
        std::string var;
        std::chrono::duration period;
        double micros;
        long min;
        long max;
        long center;
        bool invert;
        bool killThread;
        std::thread *servoThread;
};

}
