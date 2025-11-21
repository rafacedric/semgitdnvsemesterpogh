// =========================================================================
// File: KeyPress.h
// Purpose: Declaration of KeyPress class derived from Button
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#ifndef KEYPRESS_H
#define KEYPRESS_H

#include "PushButton.h"                                                     // Base Button class

// -----------------------------------------------------------------------------
// Class: KeyPress
// -----------------------------------------------------------------------------
// Derived from the Button class.
// Represents the PicoBricks push button connected to GPIO10.
// The constructor automatically configures GPIO10 with a rising-edge interrupt.
// -----------------------------------------------------------------------------
class KeyPress : public Button {
public:
    KeyPress();                                                             // Constructor (auto-configures GPIO10)
    bool isPressed();                                                       // Checks if button is pressed
};

#endif // KEYPRESS_H

