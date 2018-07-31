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

struct pindef {
    uint8_t port;
    uint8_t pin;
};

class GPIOpin {
    public:
        GPIOpin(pindef pin, GPIOmode mode, std::string var, bool state = false);
        const GPIOmode &getMode() {return mode;};
        const std::string &getVar() {return var;};
        const pindef &getPin() {return pin;};
        const bool &getState() {return state;};
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b);
        void set(bool n) {if (mode == GPIOmode::OUTPUT) state = n;};
        bool get() {return state;}
        ~GPIOpin() {};
        const static pindef GPIO1_25;
        const static pindef GPIO1_17;
        const static pindef GPIO3_20;
        const static pindef GPIO3_17;
        const static pindef GPIO3_2;
        const static pindef GPIO3_1;

    private:
        GPIOmode mode;
        bool state;
        pindef pin;
        std::string var;
};

}

#endif
