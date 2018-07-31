/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: Configuration.cpp                                   */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <streambuf>
#include <memory>
#include <cstdlib>
#include <cmath>
#include "BBBlue_FunctionBlock.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include "schema/configuration_schema.h"

using namespace std;
using namespace rapidjson;
using namespace BBBL;

rapidjson::Document &ConfBlock::parseConf(std::string conf) {
    Document confSchema;
    if (confSchema.Parse(reinterpret_cast<char*>(configuration_schema_json, configuration_schema_json_len)).HasParseError()) {
        cerr << "JSON parse error " << GetParseError_En(confSchema.GetParseError());
        cerr << " in configuration schema at offset " << confSchema.GetErrorOffset() << endl;
        std::abort();
    }
    SchemaDocument confSchemaDoc(confSchema);
    SchemaValidator validator(confSchemaDoc);
    if (configuration.Parse(conf.c_str()).HasParseError()) {
        cerr << "JSON parse error " << GetParseError_En(configuration.GetParseError());
        cerr << " in configuration JSON at offset " << configuration.GetErrorOffset() << endl;
        std::abort();
    }
    if (!configuration.Accept(validator)) {
        StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        cerr << "Invalid configuration schema: " << sb.GetString() << endl;
        cerr << "Invalid keyword: " << validator.GetInvalidSchemaKeyword() << endl;
        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        cerr << "Invalid document: " << sb.GetString() << endl;
        std::abort();
    }
    return configuration;
}

rapidjson::Document &ConfBlock::loadConfFile(std::string confFile) {
    ifstream infile;
    infile.open(confFile);
    // Make sure the file opened correctly
    if (!infile.is_open()) {
        cerr << "Failed to open configuration file " << confFile << endl;
        return configuration;
    }
    // Vacuum up the conf file
    string json;
    infile.seekg(0, ios::end);
    json.reserve(infile.tellg());
    infile.seekg(0, ios::beg);
    json.assign((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
    return parseConf(json);
}

bool ConfBlock::configureBlocks (rapidjson::Document &conf) {
    bool result = true;
    if (!conf.IsObject()) return false;
    if (conf.HasMember("LED") && conf["LED"].IsObject()) {
        result &= LEDBlock::instance()->configure(conf["LED"]);
        blocks.insert(pair<string, FunctionBlock*>("LED", LEDBlock::instance()));
    }
    if (conf.HasMember("Button") && conf["Button"].IsObject()) {
        result &= ButtonBlock::instance()->configure(conf["Button"]);
        blocks.insert(pair<string, FunctionBlock*>("Button", ButtonBlock::instance()));
    }
    if (conf.HasMember("Motor") && conf["Motor"].IsObject()) {
        result &= MotorBlock::instance()->configure(conf["Motor"]);
        blocks.insert(pair<string, FunctionBlock*>("Motor", MotorBlock::instance()));
    }
    if (conf.HasMember("Encoders") && conf["Encoders"].IsObject()) {
        result &= EncodersBlock::instance()->configure(conf["Encoders"]);
        blocks.insert(pair<string, FunctionBlock*>("Encoders", EncodersBlock::instance()));
    }
    if (conf.HasMember("ADC") && conf["ADC"].IsObject()) {
        result &= ADCBlock::instance()->configure(conf["ADC"]);
        blocks.insert(pair<string, FunctionBlock*>("ADC", ADCBlock::instance()));
    }
    if (conf.HasMember("Servos") && conf["Servos"].IsObject()) {
        result &= ServoBlock::instance()->configure(conf["Servos"]);
        blocks.insert(pair<string, FunctionBlock*>("Servos", ServoBlock::instance()));
    }
    if (conf.HasMember("IMU") && conf["IMU"].IsObject()) {
        if (conf["IMU"].HasMember("mode") && (conf["IMU"]["mode"].GetString() == "Random")) {
            result &= IMURandomBlock::instance()->configure(conf["IMU"]);
            blocks.insert(pair<string, FunctionBlock*>("IMU", IMURandomBlock::instance()));
        } else if (conf["IMU"].HasMember("mode") && (conf["IMU"]["mode"].GetString() == "DMP")) {
            result &= IMUDMPBlock::instance()->configure(conf["IMU"]);
            blocks.insert(pair<string, FunctionBlock*>("IMU", IMUDMPBlock::instance()));
        } else result &= false;
    }
    if (conf.HasMember("Barometer") && conf["Barometer"].IsObject()) {
        result &= BaroBlock::instance()->configure(conf["Barometer"]);
        blocks.insert(pair<string, FunctionBlock*>("Barometer", BaroBlock::instance()));
    }
    if (conf.HasMember("GPIO") && conf["GPIO"].IsObject()) {
        result &= GPIOBlock::instance()->configure(conf["GPIO"]);
        blocks.insert(pair<string, FunctionBlock*>("GPIO", GPIOBlock::instance()));
    }
    if (conf.HasMember("PWM") && conf["PWM"].IsObject()) {
        result &= PWMBlock::instance()->configure(conf["PWM"]);
        blocks.insert(pair<string, FunctionBlock*>("PWM", PWMBlock::instance()));
    }
    return result;
}

std::map<std::string, FunctionBlock*> ConfBlock::blocks;
rapidjson::Document ConfBlock::configuration;
