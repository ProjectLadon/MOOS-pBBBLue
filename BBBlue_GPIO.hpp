/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: BBBlue_GPIO.hpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

#ifndef BBBLUE_GPIO_H
#define BBBLUE_GPIO_H

class BBBlue;

namespace BBBL {

enum class GPIOmode { OUTPUT, INPUT };

class GPIOpin {
    public:
        GPIOpin(int pin, GPIOmode mode, std::string var, bool state = false)
        const GPIOmode &getMode() {return mode;};
        const std::string &getVar() {return var;};
        const int &getPin() {return pin;};
        const bool &getState() {return state;};
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b);
        void set(bool n) {if (mode == GPIOmode::OUTPUT) state = n};
        bool get() {return state;}
        ~GPIOpin() {};
        static int GPIO1_25 = (32 + 25);
        static int GPIO1_17 = (32 + 17);
        static int GPIO3_20 = ((32 * 3) + 20);
        static int GPIO3_17 = ((32 * 3) + 17);
        static int GPIO3_2 = ((32 * 3) + 2);
        static int GPIO3_1 = ((32 * 3) + 1);

    private:
        GPIOmode mode;
        bool state;
        int pin;
        std::string var;
};

}
