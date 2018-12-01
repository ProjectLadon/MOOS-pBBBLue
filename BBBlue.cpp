/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: BBBlue.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "ACTable.h"
#include "BBBlue.h"
#include "BBBlue_FunctionBlock.hpp"

extern "C" {
    #include "roboticscape.h"
}

using namespace std;
using namespace BBBL;

//---------------------------------------------------------
// Procedure: OnNewMail

bool BBBlue::OnNewMail(MOOSMSG_LIST &NewMail) {
    AppCastingMOOSApp::OnNewMail(NewMail);

    for(auto &msg : NewMail) {
        string key    = msg.GetKey();
        bool handled = false;
        for (auto &b: ConfBlock::getBlockMap()) {
            handled |= b.second->procMail(msg);
        }

        if(!handled || (key != "APPCAST_REQ"))  { // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
        }
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool BBBlue::OnConnectToServer() {
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool BBBlue::Iterate() {
  AppCastingMOOSApp::Iterate();
  for (auto &b: ConfBlock::getBlockMap()) {
      b.second->tick(this);
  }
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool BBBlue::OnStartUp() {
    AppCastingMOOSApp::OnStartUp();

<<<<<<< HEAD
    //if (rc_initialize()) {
    //    std::cerr << "Failed to initialize Beaglebone Blue hardware" << endl;
    //    std::abort();
    //}
=======
    // if (rc_initialize()) {
    //     std::cerr << "Failed to initialize Beaglebone Blue hardware" << endl;
    //     std::abort();
    // }
>>>>>>> fixes

    std::cerr << "App name is: " << GetAppName() << endl;
    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(!m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
        reportConfigWarning("No config block found for " + GetAppName());
    }

    for(auto &p: sParams) {
        cerr << "Parameter line: " << p << endl;
        string orig  = p;
        string line  = p;
        string param = toupper(biteStringX(line, '='));
        string value = line;

        cerr << "Parameter is: " << param << endl;
        cerr << "Value is: " << value << endl;

        bool handled = false;
        if(param == "CONF") {
            handled = ConfBlock::configureBlocks(ConfBlock::parseConf(value));
        }
        else if(param == "CONFFILE") {
            handled = ConfBlock::configureBlocks(ConfBlock::loadConfFile(value));
        }

        if(!handled) {
            reportUnhandledConfigWarning(orig);
            // exit(-1);
        }
    }

    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void BBBlue::registerVariables() {
    for (auto &b: ConfBlock::getBlockMap()) {
        b.second->subscribe(this);
    }
    AppCastingMOOSApp::RegisterVariables();
}

//------------------------------------------------------------
// Procedure: buildReport()

bool BBBlue::buildReport() {
  m_msgs << "============================================ \n";
  m_msgs << "Beaglebone Blue                              \n";
  m_msgs << "============================================ \n";

  for (auto &b: ConfBlock::getBlockMap()) {
      m_msgs << "++++++++++++++++++";
      m_msgs << b.first;
      m_msgs << "\n++++++++++++++++++\n";
      m_msgs << (b.second->buildReport()).getFormattedString();
  }

  return(true);
}
