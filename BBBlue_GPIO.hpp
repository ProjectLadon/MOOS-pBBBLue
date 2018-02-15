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

enum class GPIOmode { OUTPUT, INPUT, INPUT_PU, INPUT_PD };

class GPIOpin {
    public:
        GPIOpin(int pin, GPIOmode mode, std::string var, bool state = false)
        const GPIOmode &getMode() {return mode;};
        const std::string &getVar() {return var;};
        bool procMail(CMOOSMsg &msg);
        void set(bool n);
        bool get();
        ~GPIOpin();

    private:
        GPIOmode mode;
        bool state;
        int pin;
        std::string var;
};

}
