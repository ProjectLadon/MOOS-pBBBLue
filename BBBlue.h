/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: BBBlue.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef BBBlue_HEADER
#define BBBlue_HEADER

#include <vector>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class BBBlue : public AppCastingMOOSApp {
    public:
        BBBlue() {};
        ~BBBlue() {};
        bool notify(const std::string &var, const std::string &val) {
     	   return Notify(var, val);
        };
        bool notify(const std::string &var, const double &val) {
     	   return Notify(var, val);
        };
        bool notify(const std::string &var, const std::vector<uint8_t> &val) {
     	   return Notify(var, val);
        };
        bool registerVar(const std::string &var) {return Register(var);};

    protected: // Standard MOOSApp functions to overload
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

    protected: // Standard AppCastingMOOSApp function to overload
        bool buildReport();
        void registerVariables();

};

#endif
