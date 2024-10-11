/**
 * @file MiniR4VernierLib.h
 * @brief Handling MiniR4.Vernier functions.
 * @author MATRIX Robotics
 */
#ifndef MiniR4VernierLib_h
#    define MiniR4VernierLib_h
#    include <math.h>
#    define VERNIERLIB_LIB_VERSION "1.0.6"   // displayed automatically

/**
 * @brief A library for interfacing with Vernier sensors.
 *
 * This class provides methods to read sensor data, control DCUs,
 * and manage calibration information for Vernier sensors.
 */
class MiniR4VernierLib
{
public:
    MiniR4VernierLib();

    void autoID();   // this is the function for the autoID code
    // it returns calibration information
    int   channel() { return _channel; };
    float voltageID() { return _voltageID; };
    int   sensorNumber() { return _sensorNumber; };
    char* sensorName() { return _sensorName; };
    char* shortName() { return _shortName; };
    char* sensorUnits() { return _sensorUnits; };
    float slope() { return _slope; };
    float intercept() { return _intercept; };
    float cFactor() { return _cFactor; };
    int   calEquationType() { return _calEquationType; };
    int   page() { return _page; };

    float readSensor();   // This function converts count to sensor reading
    float sensorReading() { return _sensorReading; };

    void DCUPWM(int PWMSetting);   // function to control PWM via the DCU, line 4 (Arduino line 9)

    void DCU(int DCUSetting);   // function for controlling a DCU

    void DCUStep(int stepCount, int stepDirection, int stepDelay);   // function for stepper motors

    float readMotionDetector();   // function for Motion Detectors
    float distance() { return _distance; };

protected:   //  also known as local
    float _voltageID;
    int   _channel;
    int   _sensorNumber;
    char  _sensorName[16];
    char  _shortName[12];   // 11 char + terminator
    char  _sensorUnits[7];
    float _slope;
    float _sum;
    float _intercept;
    float _cFactor;
    int   _calEquationType;
    int   _page;
    float _sensorReading;
    float _distance;
    // this misc. variable is used many places
    int _i;
};
#endif

// END OF FILE
