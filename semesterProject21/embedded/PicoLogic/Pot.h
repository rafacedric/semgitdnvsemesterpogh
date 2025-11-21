// =========================================================================
// File: Pot.h
// Purpose: Declaration of Pot class derived from ADC
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#ifndef POT_H
#define POT_H

#include "ADC.h"                                                           // Base ADC class

// -----------------------------------------------------------------------------
// Class: Pot
// -----------------------------------------------------------------------------
// Derived from the ADC base class.
// Represents the PicoBricks potentiometer connected to GPIO26 (ADC0).
// Provides a convenient method for reading the potentiometer value as a
// normalized percentage (0–100 %).
// -----------------------------------------------------------------------------
class Pot : public ADC {
public:
    Pot();                                                                 // Constructor (fixed to GPIO26)
    float readPercent();                                                   // Returns potentiometer value in percent (0–100 %)

private:
    float _normVal;                                                        // Stores last normalized reading
};

#endif // POT_H
