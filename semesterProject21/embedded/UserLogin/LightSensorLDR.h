//=========================================================================
//  LightSensorLDR.h                                                       
//  Derived class for light intensity measurement using LDR sensor.        
//=========================================================================

#ifndef LIGHT_SENSOR_LDR_H
#define LIGHT_SENSOR_LDR_H

#include "ADC.h"
#include <cstdint>
#include <cmath>                                                            // For powf()


//-------------------------------------------------------------------------
//  LightSensorLDR class: inherits from ADC                                
//-------------------------------------------------------------------------
class LightSensorLDR : public ADC {
public:
    LightSensorLDR();                                                      // Constructor (uses GPIO27)
    float readLux();                                                       // Convert ADC reading to Lux
};

#endif
