/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: BBBlue_PWM.hpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

#ifndef BBBLUE_PWM_H
#define BBBLUE_PWM_H

class BBBlue;

namespace BBBL {

class PWMChannel {
    public:
        PWMChannel(int iface, int freq, std::string varA, std::string varB, bool isnsA = false, bool isnsB = false);
        PWMChannel(int iface, std::string freqVar, std::string varA, std::string varB, bool isnsA = false, bool isnsB = false);
        bool setFrequency (int f);
        const std::string &getVarA() {return varA;};
        const std::string &getVarB() {return varB;};
        const std::string &getFreqVar() {return freqVar;};
        const double &getA() {return A;};
        const double &getB() {return B;};
        bool procMail (CMOOSMsg &msg);
        bool setDutyA (double d);
        bool setDutyB (double d);
        bool subscribe(BBBlue *b);
        ~PWMChannel() {rc_pwm_close(iface);};
    private:
        int iface;
        int period;
        int freq;
        std::string freqVar;
        std::string varA;
        std::string varB;
        double A;
        double B;
        bool isnsA;
        bool isnsB
};

}
