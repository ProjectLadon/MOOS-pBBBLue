/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                     */
/*    FILE: IMU.cpp                                   */
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
#include "BBBlue.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"

extern "C" {
    #include "roboticscape.h"
}

using namespace std;
using namespace rapidjson;
using namespace BBBL;

bool getAccelDLPF (std::string a, rc_mpu_config_t &c);
bool getGyroDLPF (std::string a, rc_mpu_config_t &c);
bool getAccelFSR (std::string a, rc_mpu_config_t &c);
bool getGyroFSR (std::string a, rc_mpu_config_t &c);
bool getOrientation (std::string a, rc_mpu_config_t &c);

IMURandomBlock* IMURandomBlock::instance() {
    if (!s_instance) s_instance = new IMURandomBlock();
    return s_instance;
}

bool IMURandomBlock::configure(rapidjson::Value &v) {
    if (IMUDMPBlock::instance()->isConfigured()) return false;
    if (!v.HasMember("mode") || !v["mode"].IsString() ||
        (v["mode"].GetString() != "Random")) return false;
    configured = true;
    rc_mpu_set_config_to_default(&conf);
    if (v.HasMember("magnetometer") && v["magnetometer"].IsBool() &&
        v["magnetometer"].GetBool()) {
            conf.enable_magnetometer = 1;
        } else conf.enable_magnetometer = 0;
    if (v.HasMember("accelRange") && v["accelRange"].IsString())
        configured &= getAccelFSR(v["accelRange"].GetString(), conf);
    if (v.HasMember("gyroRange") && v["gyroRange"].IsString())
        configured &= getGyroFSR(v["gyroRange"].GetString(), conf);
    if (v.HasMember("accelFilter") && v["accelFilter"].IsString())
        configured &= getAccelDLPF(v["accelFilter"].GetString(), conf);
    if (v.HasMember("gyroFilter") && v["gyroFilter"].IsString())
        configured &= getGyroDLPF(v["gyroFilter"].GetString(), conf);
    if (configured && rc_mpu_initialize(&data, conf)) configured = false;    // Checks if initilization failed
    return configured;
}

bool IMURandomBlock::tick(BBBlue *b) {
    if (rc_mpu_read_accel(&data) || rc_mpu_read_gyro(&data) ||
        rc_mpu_read_mag(&data) || rc_mpu_read_temp(&data)) return false;

    // magnetic heading code derived from http://www.cypress.com/file/130456/download
    double Atotal = sqrt(data.accel[0]*data.accel[0] +
                    data.accel[1]*data.accel[1] +
                    data.accel[2]*data.accel[2]);
	double Ax = data.accel[0]/Atotal;
	double Ay = data.accel[1]/Atotal;
	double B = 1 - (Ax*Ax);
	double C = Ax*Ay;
	double D = sqrt(1 - (Ax*Ax) - (Ay*Ay));
    double x = data.mag[0]*B - data.mag[1]*C - data.mag[2]*Ax*D;    // Equation 18
    double y = data.mag[1]*D - data.mag[2]*Ay;                      // Equation 19
    heading = atan2(y, x) * 180/PI;

    // pack and publish all the things
    string accdat = "{\"x\":" + to_string(data.accel[0]) +
                        ",\"y\":" + to_string(data.accel[1]) +
                        ",\"z\":" + to_string(data.accel[2]) + "}";
    string gyrodat = "{\"x\":" + to_string(data.gyro[0]) +
                        ",\"y\":" + to_string(data.gyro[1]) +
                        ",\"z\":" + to_string(data.gyro[2]) + "}";
    string magdat = "{\"x\":" + to_string(data.mag[0]) +
                        ",\"y\":" + to_string(data.mag[1]) +
                        ",\"z\":" + to_string(data.mag[2]) + "}";
    b->notify("BBBL_IMU_COMPASS", heading);
    b->notify("BBBL_IMU_TEMP", data.temp);
    b->notify("BBBL_IMU_ACCEL", accdat);
    b->notify("BBBL_IMU_GYRO", gyrodat);
    b->notify("BBBL_IMU_MAG", magdat);
    return true;
}

ACTable IMURandomBlock::buildReport() {
    ACTable actab(4);
    actab << "Data|X|Y|Z";
    actab.addHeaderLines();
    actab << "Accel";
    for (int i = 0; i < 3; i++) actab << to_string(data.accel[i]);
    actab << "Gyro";
    for (int i = 0; i < 3; i++) actab << to_string(data.gyro[i]);
    actab << "Mag";
    for (int i = 0; i < 3; i++) actab << to_string(data.mag[i]);
    actab << "Heading";
    actab << to_string(heading);
    actab << "Temp";
    actab << to_string(data.temp);
    return actab;
}

IMURandomBlock::~IMURandomBlock() {
    rc_mpu_power_off();
}

IMUDMPBlock* IMUDMPBlock::instance() {
    if (!s_instance) s_instance = new IMUDMPBlock();
    return s_instance;
}

bool IMUDMPBlock::configure(rapidjson::Value &v) {
    if (IMURandomBlock::instance()->isConfigured()) return false;
    if (!v.HasMember("mode") || !v["mode"].IsString() ||
        (v["mode"].GetString() != "DMP")) return false;
    configured = true;
    if (v.HasMember("magnetometer") && v["magnetometer"].IsBool() &&
        v["magnetometer"].GetBool()) {
            conf.enable_magnetometer = 1;
        } else conf.enable_magnetometer = 0;
    if (v.HasMember("accelRange") && v["accelRange"].IsString())
        configured &= getAccelFSR(v["accelRange"].GetString(), conf);
    if (v.HasMember("gyroRange") && v["gyroRange"].IsString())
        configured &= getGyroFSR(v["gyroRange"].GetString(), conf);
    if (v.HasMember("accelFilter") && v["accelFilter"].IsString())
        configured &= getAccelDLPF(v["accelFilter"].GetString(), conf);
    if (v.HasMember("gyroFilter") && v["gyroFilter"].IsString())
        configured &= getGyroDLPF(v["gyroFilter"].GetString(), conf);
    if (v.HasMember("orientation") && v["orientation"].IsString())
        configured &= getOrientation(v["orientation"].GetString(), conf);
    if (v.HasMember("dmpSampleRate") && v["dmpSampleRate"].IsInt())
        conf.dmp_sample_rate = v["dmpSampleRate"].GetInt();
    if (v.HasMember("dmpCompassTimeConstant") && v["dmpCompassTimeConstant"].IsFloat())
        conf.compass_time_constant = v["dmpCompassTimeConstant"].GetFloat();
    if (configured && rc_mpu_initialize_dmp(&data, conf)) configured = false;    // Checks if initilization failed
    return configured;
}

bool IMUDMPBlock::tick(BBBlue *b) {
    string taitBryan = "{\"x\":" + to_string(data.fused_TaitBryan[0]) +
                        ",\"y\":" + to_string(data.fused_TaitBryan[1]) +
                        ",\"z\":" + to_string(data.fused_TaitBryan[2]) + "}";
    string quaternion = "{\"x1\":" + to_string(data.fused_quat[0]) +
                        ",\"x2\":" + to_string(data.fused_quat[1]) +
                        ",\"x3\":" + to_string(data.fused_quat[2]) +
                        ",\"x4\":" + to_string(data.fused_quat[3]) + "}";
    b->notify("BBBL_DMP_HEADING", data.compass_heading);
    b->notify("BBBL_DMP_HEADING_RAW", data.compass_heading_raw);
    b->notify("BBBL_DMP_TBA", taitBryan);
    b->notify("BBBL_DMP_QT", quaternion);
    b->notify("BBBL_IMU_TEMP", data.temp);
    return true;
}

ACTable IMUDMPBlock::buildReport() {
    ACTable actab(5);
    actab << "Data|X/X1|Y/X2|Z/X3|X4";
    actab.addHeaderLines();
    actab << "Tait-Bryan";
    for (int i = 0; i < 3; i++) actab << to_string(data.fused_TaitBryan[i]);
    actab << " ";
    actab << "Quaternion";
    for (int i = 0; i < 3; i++) actab << to_string(data.fused_quat[i]);
    actab << "Heading";
    actab << to_string(data.compass_heading);
    actab << "Temp";
    actab << to_string(data.temp);
    return actab;
}

IMUDMPBlock::~IMUDMPBlock() {
    rc_mpu_power_off();
}

bool getAccelDLPF (std::string a, rc_mpu_config_t &c) {
    if ("OFF" == a) {c.accel_dlpf = ACCEL_DLPF_OFF; return true;}
    if ("184" == a) {c.accel_dlpf = ACCEL_DLPF_184; return true;}
    if ("92" == a) {c.accel_dlpf = ACCEL_DLPF_92; return true;}
    if ("41" == a) {c.accel_dlpf = ACCEL_DLPF_41; return true;}
    if ("20" == a) {c.accel_dlpf = ACCEL_DLPF_20; return true;}
    if ("10" == a) {c.accel_dlpf = ACCEL_DLPF_10; return true;}
    if ("5" == a) {c.accel_dlpf = ACCEL_DLPF_5; return true;}
    return false;
}

bool getGyroDLPF (std::string a, rc_mpu_config_t &c) {
    if ("OFF" == a) {c.gyro_dlpf = GYRO_DLPF_OFF; return true;}
    if ("184" == a) {c.gyro_dlpf = GYRO_DLPF_184; return true;}
    if ("92" == a) {c.gyro_dlpf = GYRO_DLPF_92; return true;}
    if ("41" == a) {c.gyro_dlpf = GYRO_DLPF_41; return true;}
    if ("20" == a) {c.gyro_dlpf = GYRO_DLPF_20; return true;}
    if ("10" == a) {c.gyro_dlpf = GYRO_DLPF_10; return true;}
    if ("5" == a) {c.gyro_dlpf = GYRO_DLPF_5; return true;}
    return false;
}

bool getAccelFSR (std::string a, rc_mpu_config_t &c) {
    if ("2G" == a) {c.accel_fsr = ACCEL_FSR_2G; return true;}
    if ("4G" == a) {c.accel_fsr = ACCEL_FSR_4G; return true;}
    if ("8G" == a) {c.accel_fsr = ACCEL_FSR_8G; return true;}
    if ("16G" == a) {c.accel_fsr = ACCEL_FSR_16G; return true;}
    return false;
}

bool getGyroFSR (std::string a, rc_mpu_config_t &c) {
    if ("250DPS" == a) {c.gyro_fsr = GYRO_FSR_250DPS; return true;}
    if ("500DPS" == a) {c.gyro_fsr = GYRO_FSR_500DPS; return true;}
    if ("1000DPS" == a) {c.gyro_fsr = GYRO_FSR_1000DPS; return true;}
    if ("2000DPS" == a) {c.gyro_fsr = GYRO_FSR_2000DPS; return true;}
    return false;
}

bool getOrientation (std::string a, rc_mpu_config_t &c) {
    if ("Z_UP"   == a) {c.orient = ORIENTATION_Z_UP; return true;}
    if ("Z_DN"   == a) {c.orient = ORIENTATION_Z_DOWN; return true;}
	if ("X_UP"   == a) {c.orient = ORIENTATION_X_UP; return true;}
    if ("X_DN"   == a) {c.orient = ORIENTATION_X_DOWN; return true;}
	if ("Y_UP"   == a) {c.orient = ORIENTATION_Y_UP; return true;}
    if ("Y_DN"   == a) {c.orient = ORIENTATION_Y_DOWN; return true;}
	if ("X_FWD"  == a) {c.orient = ORIENTATION_X_FORWARD; return true;}
    if ("X_BACK" == a) {c.orient = ORIENTATION_X_BACK; return true;}
    return false;
}

IMURandomBlock* IMURandomBlock::s_instance;
IMUDMPBlock* IMUDMPBlock::s_instance;
