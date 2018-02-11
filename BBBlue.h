/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: BBBlue.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef BBBlue_HEADER
#define BBBlue_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class BBBlue : public AppCastingMOOSApp
{
 public:
   BBBlue();
   ~BBBlue();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
