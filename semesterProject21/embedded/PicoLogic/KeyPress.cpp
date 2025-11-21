// =========================================================================
// File: KeyPress.cpp
// Purpose: Implementation of KeyPress class derived from Button
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#include "KeyPress.h"                                                       // Class declaration

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
// Calls the Button base-class constructor with fixed GPIO10 and a rising-edge
// interrupt configuration. This setup matches the PicoBricks hardware layout.
// -----------------------------------------------------------------------------
KeyPress::KeyPress()
    : Button(10, GPIO_IRQ_EDGE_RISE) { }                                    // PicoBricks button on GPIO10

// -----------------------------------------------------------------------------
// Function: isPressed()
// -----------------------------------------------------------------------------
// Provides a direct query for the pressed state of the PicoBricks button.
// Internally reuses the base class method hasEvent().
// -----------------------------------------------------------------------------
bool KeyPress::isPressed() {
    return hasEvent();                                                      // Reuse base class functionality
}

