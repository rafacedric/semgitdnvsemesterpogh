//=========================================================================
//  ADC.cpp                                                                
//  Implementation of ADC base class for Raspberry Pi Pico.                
//=========================================================================

#include "ADC.h"

//-------------------------------------------------------------------------
//  Constructor: initializes ADC hardware and selects input pin.           
//-------------------------------------------------------------------------
ADC::ADC(uint pin) : _pin(pin) {
    adc_init();                                                            // Initialize ADC subsystem
    adc_gpio_init(pin);                                                    // Configure GPIO as ADC input
    uint channel = pin - 26;                                               // GPIO26 → ADC0, 27 → ADC1, 28 → ADC2
    adc_select_input(channel);                                             // Select ADC channel
}

//-------------------------------------------------------------------------
//  Performs a single ADC conversion.                                      
//-------------------------------------------------------------------------
uint16_t ADC::read() {
    return adc_read();                                                     // Return 12-bit result (0–4095)
}

//-------------------------------------------------------------------------
//  Averages several ADC readings for noise reduction.                     
//-------------------------------------------------------------------------
uint16_t ADC::readAvg(uint8_t samples) {
    uint32_t sum = 0;                                                      // Accumulate samples
    for (uint8_t i = 0; i < samples; ++i) {
        sum += read();                                                     // Read ADC and add
        sleep_us(50);                                                      // Short delay for stability
    }
    return static_cast<uint16_t>(sum / samples);                           // Return average value
}
