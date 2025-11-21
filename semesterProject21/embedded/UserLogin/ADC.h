//=========================================================================
//  ADC.h                                                                 
//  Base class for Analog-to-Digital Conversion on Raspberry Pi Pico.     
//=========================================================================

#ifndef ADC_H
#define ADC_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <cstdint>

//-------------------------------------------------------------------------
//  ADC base class                                                        
//-------------------------------------------------------------------------
class ADC {
public:
    explicit ADC(uint pin);                                                // Constructor, initializes ADC pin
    uint16_t read();                                                       // Single ADC read
    uint16_t readAvg(uint8_t samples = 8);                                 // Average over N samples

protected:
    uint _pin;                                                             // GPIO pin used for ADC
};

#endif
