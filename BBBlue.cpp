/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: BBBlue.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "BBBlue.h"
#include "BBBlue_FunctionBlock.hpp"

using namespace std;

//---------------------------------------------------------
// Procedure: OnNewMail

bool BBBlue::OnNewMail(MOOSMSG_LIST &NewMail) {
    AppCastingMOOSApp::OnNewMail(NewMail);

    for(auto &msg : NewMail) {
        string key    = msg.GetKey();
        bool handled = false;
        for (auto &b: FunctionBlock::getBlockMap) {
            handled |= b.second->procMail(msg);
        }

        else if(!handled || (key != "APPCAST_REQ"))  { // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
        }
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool BBBlue::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool BBBlue::Iterate() {
  AppCastingMOOSApp::Iterate();
  for (auto &b: FunctionBlock::getBlockMap()) {
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

    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(!m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
        reportConfigWarning("No config block found for " + GetAppName());
    }

    for(auto &p: sParams) {
        string orig  = p;
        string line  = p;
        string param = toupper(biteStringX(line, '='));
        string value = line;

        bool handled = false;
        if(param == "conf") {
            handled = ConfBlock::configureBlocks(parseConf(value));
        }
        else if(param == "confFile") {
            handled = ConfBlock::configureBlocks(loadConfFile(value));
        }

        if(!handled) reportUnhandledConfigWarning(orig);
    }

    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void BBBlue::registerVariables() {
    for (auto &b: FunctionBlock::getBlockMap) {
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
      m_msgs << (b.second->buildReport()).getFormattedString();
  }

  return(true);
}
