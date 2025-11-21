//=========================================================================
//  MyApp.h                                                                
//  Application class combining OLED display and LDR sensor.               
//=========================================================================

#ifndef MYAPP_H
#define MYAPP_H

//-------------------------------------------------------------------------
//  SELECT MODE: uncomment FUNCTION_OVERLOAD to use function overloading.  
//-------------------------------------------------------------------------
//#define FUNCTION_OVERLOAD                                                  // Comment this line to use templates.
//-------------------------------------------------------------------------

#include "OLEDDisplay.h"
#include "LightSensorLDR.h"
#include "Pot.h"
#include "KeyPress.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include "Userlogin.h"

//-------------------------------------------------------------------------
//  MyApp class: handles sensor reading and display output.                
//-------------------------------------------------------------------------
class MyApp {
public:
    MyApp();                                                               // Constructor
    void run();                                                            // Main loop

#ifdef FUNCTION_OVERLOAD
    std::string formatLux(float value);                                    // Overload for float
    std::string formatLux(uint16_t value);                                 // Overload for uint16_t
#else
    template <typename T>
    std::string formatLux(T value) {                                       // Template for both float & int
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << value;
        std::string str = oss.str();
        str += " LUX";
        return str;
    }

#endif
private:
    int selectedIndex = 0;   // selected username index
    KeyPress keyPress;       // PicoBricks button
    LightSensorLDR ldr;                                                    // Light sensor instance
    OLEDDisplay display;                                                   // OLED display instance
    Pot pot;
    enum AppState {
        MENU_MAIN,
        MENU_LOGIN,
        MENU_SESSION,
        MENU_FETCH,
    };
    AppState currentState = MENU_MAIN;
    UserLogin userLogin;
};

#endif
