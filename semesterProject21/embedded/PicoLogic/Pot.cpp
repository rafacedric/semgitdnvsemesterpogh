// =========================================================================
// File: Pot.cpp
// Purpose: Implementation of Pot class derived from ADC
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#include "Pot.h"                                                            // Class declaration

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
// Calls the ADC base constructor with fixed GPIO26, which corresponds to
// ADC channel 0 on the PicoBricks board.
// -----------------------------------------------------------------------------
Pot::Pot()
    : ADC(26) { }                                                           // GPIO26 corresponds to ADC0

// -----------------------------------------------------------------------------
// Function: readPercent()
// -----------------------------------------------------------------------------
// Reads the potentiometer value using an averaged ADC reading and converts it
// into a normalized percentage (0–100 %).
// -----------------------------------------------------------------------------
float Pot::readPercent() {
    int raw = readAvg(5);                                                   // Average of 5 samples
    _normVal = static_cast<float>(raw) * 100.0f / 4095.0f;                  // Convert to percentage
    return _normVal;                                                        // Return normalized value
}

