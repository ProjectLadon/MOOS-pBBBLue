/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: BBBlue_FunctionBlock.hpp                       */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include "BBBlue_Servo.hpp"
#include "BBBlue_GPIO.hpp"
#include "BBBlue_PWM.hpp"
#include "roboticscape.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "ACTable.h"

#ifndef FUNCTION_BLOCK_H
#define FUNCTION_BLOCK_H

namespace BBBL {

class FunctionBlock {
    public:
        FunctionBlock() {};
        virtual bool configure(rapidjson::Value &v) = 0;
        virtual bool procMail(CMOOSMsg &msg) = 0;
        virtual bool tick(BBBlue *b) = 0;
        virtual bool subscribe(BBBlue *b) = 0;
        virtual bool isConfigured() = 0;
        virtual ACTable buildReport() = 0;
        ~FunctionBlock() {};
};

class ConfBlock {
    public:
        const static std::map<std::string, FunctionBlock*> &getBlockMap () {return blocks;};
        static rapidjson::Document &parseConf(std::string conf);
        static rapidjson::Document &loadConfFile(std::string confFile);
        static bool configureBlocks (rapidjson::Document &conf);
    private:
        ConfBlock() {};
        ~ConfBlock() {};
        static rapidjson::Document configuration;
        static std::map<std::string, FunctionBlock*> blocks;
};

class LEDBlock : public FunctionBlock {
    public:
        static LEDBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b) {return true;};
        bool subscribe(BBBlue *b);
        bool isConfigured();
        ACTable buildReport();
        ~LEDBlock() {};
    private:
        LEDBlock() {};
        static LEDBlock* s_instance;
        std::string REDname;
        std::string GRNname;
};

class ButtonBlock : public FunctionBlock {
    public:
        static ButtonBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg) {return false;};
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b) {return true;};
        bool isConfigured() {return ((pause != "") || (mode != ""));};
        ACTable buildReport();
        ~ButtonBlock() {};
    private:
        ButtonBlock() {};
        static ButtonBlock* s_instance;
        std::string pause;
        std::string mode;
};

class MotorBlock : public FunctionBlock {
    public:
        static MotorBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b) {return true;};
        bool subscribe(BBBlue *b);
        bool isConfigured() {return configured;};
        ACTable buildReport();
        ~MotorBlock();
    private:
        MotorBlock();
        static MotorBlock* s_instance;
        std::vector<std::string> motors;
        std::vector<bool> isfree;
        std::vector<bool> isbrake;
        std::vector<double> throttle;
        bool enabled;
        bool setMotor(int motor, std::string val);
        bool configured = false;
        rapidjson::SchemaDocument *sd;
        rapidjson::SchemaValidator *validator;
};

class EncodersBlock : public FunctionBlock {
    public:
        static EncodersBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg) {return false;};
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b) {return true;};
        bool isConfigured();
        ACTable buildReport();
        ~EncodersBlock() {};
    private:
        EncodersBlock() {};
        static EncodersBlock* s_instance;
        std::vector<std::string> encoders;
};

class ADCBlock : public FunctionBlock {
    public:
        static ADCBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg) {return false;};
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b) {return true;};
        bool isConfigured();
        ACTable buildReport();
        ~ADCBlock() {};
    private:
        ADCBlock() {};
        static ADCBlock* s_instance;
        std::vector<std::string> rawChannels;
        std::vector<std::string> voltChannels;
};

class ServoBlock : public FunctionBlock {
    public:
        static ServoBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b) {return true;};
        bool subscribe(BBBlue *b);
        bool isConfigured();
        ACTable buildReport();
        ~ServoBlock() {rc_servo_cleanup();};
    private:
        ServoBlock() {};
        static ServoBlock* s_instance;
        std::vector<std::unique_ptr<ServoChannel>> servos;
};

class IMURandomBlock : public FunctionBlock {
    public:
        static IMURandomBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg) {return false;};
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b) {return true;};
        bool isConfigured() {return configured;};
        ACTable buildReport();
        ~IMURandomBlock();
    private:
        IMURandomBlock() {};
        static IMURandomBlock* s_instance;
        rc_mpu_data_t data;
        rc_mpu_config_t conf;
        double heading;
        double temp;
        bool configured = false;

};

class IMUDMPBlock : public FunctionBlock {
    public:
        static IMUDMPBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg) {return false;};
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b) {return true;};
        bool isConfigured() {return configured;};
        ACTable buildReport();
        ~IMUDMPBlock();
    private:
        IMUDMPBlock() {};
        static IMUDMPBlock* s_instance;
        rc_mpu_data_t data;
        rc_mpu_config_t conf;
        double temp;
        bool configured = false;
};

class BaroBlock : public FunctionBlock {
    public:
        static BaroBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b);
        bool isConfigured() {return configured;};
        ACTable buildReport();
        ~BaroBlock() {rc_bmp_power_off();};
    private:
        BaroBlock() {};
        static BaroBlock* s_instance;
        double temp;
        double pressure;
        double seaLevelPressure = 101325;
        bool configured = false;
};

class GPIOBlock : public FunctionBlock {
    public:
        static GPIOBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b);
        bool subscribe(BBBlue *b);
        bool isConfigured() {return configured;};
        ACTable buildReport();
        ~GPIOBlock() {};
    private:
        GPIOBlock() {};
        static GPIOBlock* s_instance;
        bool configured = false;
        std::vector<std::unique_ptr<GPIOpin>> pins;
};

class PWMBlock : public FunctionBlock {
    public:
        static PWMBlock* instance();
        bool configure(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool tick(BBBlue *b) {return true;};
        bool subscribe(BBBlue *b);
        bool isConfigured() {return configured;};
        ACTable buildReport();
        ~PWMBlock() {};
    private:
        PWMBlock() {};
        static PWMBlock* s_instance;
        std::vector<std::unique_ptr<PWMChannel>> interfaces;
        bool configured = false;
};


}

#endif
