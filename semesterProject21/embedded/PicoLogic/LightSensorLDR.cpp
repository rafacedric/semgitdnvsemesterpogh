//=========================================================================
//  LightSensorLDR.cpp                                                     
//  Implementation of light sensor class using an LDR and ADC.             
//=========================================================================

#include "LightSensorLDR.h"

//-------------------------------------------------------------------------
//  Constructor: binds LDR sensor to GPIO27 (ADC1).                        
//-------------------------------------------------------------------------
LightSensorLDR::LightSensorLDR() : ADC(27) {                               // GPIO27 → ADC1
}

//-------------------------------------------------------------------------
//  Converts averaged ADC reading to approximate Lux value.                
//-------------------------------------------------------------------------
float LightSensorLDR::readLux() {
    int raw = readAvg(5);                                                  // Average of 5 samples
    if (raw <= 0) raw = 1;                                                 // Avoid division by zero

    const float Rfixed = 10000.0f;                                         // Fixed resistor value (Ω)
    float rLDR = Rfixed * (static_cast<float>(raw) / (4095.0f - raw));     // Compute LDR resistance

    const float A = 2.0e6f;                                                // Empirical constant (sensor-specific)
    const float gamma = 1.4f;                                              // Exponent defining response curve

    float lux = powf((A / rLDR), 1.0f / gamma);                            // Compute light intensity (lux)
    if (lux < 0.01f) lux = 0.01f;                                          // Clamp to avoid negatives
    return lux;                                                            // Return computed lux
}
