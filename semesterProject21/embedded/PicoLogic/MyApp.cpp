//=========================================================================
//  MyApp.cpp                                                              
//  Implementation of MyApp class using LDR sensor and OLED display.       
//=========================================================================

#include "MyApp.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#define USER_LOGIN

//-------------------------------------------------------------------------
//  Constructor: initializes I2C and OLED display.                         
//-------------------------------------------------------------------------
MyApp::MyApp()
    : ldr(),                                                               // Initialize LDR sensor (GPIO27)
      display(i2c_default, 0x3C, 128, 32),                                  // OLED: 128x32 @ I²C 0x3C
      userLogin()
{
    stdio_init_all();                                                      // Initialize stdio
    i2c_init(i2c_default, 400 * 1000);                                     // I²C at 400 kHz
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);            // SDA pin
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);            // SCL pin
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);                                // Enable pull-ups
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    display.init();                                                        // Initialize display
    display.clear();                                                       // Clear screen
}

//-------------------------------------------------------------------------
//  FUNCTION OVERLOADING IMPLEMENTATIONS                                   
//-------------------------------------------------------------------------
#ifdef FUNCTION_OVERLOAD

std::string MyApp::formatLux(float value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    std::string str = oss.str();
    str += " LUX";
    return str;
}

//-------------------------------------------------------------------------
//  TODO: Implement the uint16_t version of formatLux().                   
//  Use the float version above as inspiration.                            
//-------------------------------------------------------------------------
std::string MyApp::formatLux(uint16_t value) {                                                                           
    // Hints:                                                             
    // 1. Create an output stream (std::ostringstream).   
    std::ostringstream output;                
    // 2. Insert the integer 'value' into the stream.
    output << std::fixed << value;                     
    // 3. Convert the stream into a string using oss.str(). 
    std::string str = output.str();
    // 4. Append the unit " LUX" to the string.       
    str += " LUX";                    
    // 5. Return the completed string below.                                                                               
    return str;                                                            // Return the formatted string
}
#endif
//-------------------------------------------------------------------------
//  Main loop: reads LDR, converts, and shows Lux values.                  
//-------------------------------------------------------------------------
#ifdef USER_LOGIN
void MyApp::run() {
    // persistent state
    static int loginIndex = 0;
    static int sessionIndex = 0;
    static bool loginChanged = true;
    static bool sessionChanged = true;
    bool firstLoginEntry = true;
    bool firstSessionEntry = true;

    const std::vector<std::string> sessionChoices = {"Stand","Sit","Logout"};
    const int totalSessionChoices = static_cast<int>(sessionChoices.size());

    // pot debouncing / rate limit
    const uint32_t pot_min_delay_ms = 150;   // minimum ms between pot-driven index changes
    uint32_t pot_last_change_ms = to_ms_since_boot(get_absolute_time());
    float last_pot_percent = pot.readPercent();

    // button debounce
    const uint32_t button_debounce_ms = 200;

    while (true) {
        float lux = ldr.readLux();
        float potPercent = pot.readPercent();
        uint16_t potPercentInt = static_cast<uint16_t>(potPercent);

        // current time once per loop
        uint32_t now_ms = to_ms_since_boot(get_absolute_time());

        switch (currentState) {
            case MENU_MAIN: {
                // show main menu (Login / Fetch)
                display.clear();
                if (potPercentInt < 50) {
                    display.writeText(5, 0, "* Login");
                    display.writeText(5, 8, "Fetch");
                } else {
                    display.writeText(5, 0, "Login");
                    display.writeText(5, 8, "* Fetch");
                }
                display.render();

                if (keyPress.isPressed()) {
                    // debounce simple: wait a bit and wait for release
                    sleep_ms(button_debounce_ms);
                    while (keyPress.isPressed()) sleep_ms(10);
                    currentState = (potPercentInt < 50) ? MENU_LOGIN : MENU_FETCH;
                    firstLoginEntry = true;
                }
                break;
            }

            case MENU_FETCH: {
                // visual feedback
                display.clear();
                display.writeText(5, 0, "fetching ...");
                display.render();

                // Blocking network call - consider making this async later
                if (userLogin.fetchUsernames()) {
                    currentState = MENU_MAIN;
                } else {
                    // show failure briefly
                    display.clear();
                    display.writeText(5, 0, "fetch failed");
                    display.render();
                    sleep_ms(700);
                    currentState = MENU_MAIN;
                }
                break;
            }

            case MENU_LOGIN: {
                const auto& users = userLogin.getUsernames();
                int total = static_cast<int>(users.size());

                if (firstLoginEntry) {
                    loginIndex = 0;
                    loginChanged = true;
                    firstLoginEntry = false;
                    // Reset pot tracking so immediate wobble doesn't change index
                    last_pot_percent = potPercent;
                    pot_last_change_ms = now_ms;
                }

                if (total == 0) {
                    display.clear();
                    display.writeText(0, 0, "No users found");
                    display.render();
                    // let user press to go back
                    if (keyPress.isPressed()) {
                        sleep_ms(button_debounce_ms);
                        while (keyPress.isPressed()) sleep_ms(10);
                        currentState = MENU_MAIN;
                    }
                    break;
                }

                // POT handling with rate limit + hysteresis (deadzone)
                const float deadzone = 3.0f; // ignore tiny changes
                if (fabs(potPercent - last_pot_percent) > deadzone && (now_ms - pot_last_change_ms) >= pot_min_delay_ms) {
                    // move left / right only one step per rate interval
                    if (potPercent < 33.0f && loginIndex > 0) {
                        loginIndex--;
                        loginChanged = true;
                        pot_last_change_ms = now_ms;
                    } else if (potPercent > 66.0f && loginIndex < total - 1) {
                        loginIndex++;
                        loginChanged = true;
                        pot_last_change_ms = now_ms;
                    }
                    last_pot_percent = potPercent;
                }

                if (loginChanged) {
                    display.clear();
                    int start = std::max(0, std::min(loginIndex - 1, total - 3));
                    for (int i = 0; i < 3 && (start + i) < total; ++i) {
                        std::string name = users[start + i];
                        if (start + i == loginIndex) name = "* " + name;
                        else name = "  " + name;
                        display.writeText(0, i * 8, name.c_str());
                    }
                    display.render();
                    loginChanged = false;
                }

                if (keyPress.isPressed()) {
                    // debounce: wait for release
                    sleep_ms(button_debounce_ms);
                    while (keyPress.isPressed()) sleep_ms(10);

                    // call login (may block). Consider doing this async.
                    bool ok = userLogin.loginUser(users[loginIndex]);

                    // show result briefly
                    display.clear();
                    if (ok) display.writeText(0, 8, "Login OK");
                    else    display.writeText(0, 8, "Login FAIL");
                    display.render();
                    sleep_ms(500);

                    // move to session menu only if login succeeded
                    if (ok) {
                        currentState = MENU_SESSION;
                        firstSessionEntry = true;
                        // reset session pot tracking
                        last_pot_percent = pot.readPercent();
                        pot_last_change_ms = to_ms_since_boot(get_absolute_time());
                    } else {
                        // stay in login menu
                        loginChanged = true;
                    }
                }
                break;
            }

            case MENU_SESSION: {
                if (firstSessionEntry) {
                    sessionIndex = 0;
                    sessionChanged = true;
                    firstSessionEntry = false;
                }

                // POT handling (same approach)
                if (fabs(potPercent - last_pot_percent) > 3.0f && (now_ms - pot_last_change_ms) >= pot_min_delay_ms) {
                    if (potPercent < 33.0f && sessionIndex > 0) {
                        sessionIndex--;
                        sessionChanged = true;
                        pot_last_change_ms = now_ms;
                    } else if (potPercent > 66.0f && sessionIndex < totalSessionChoices - 1) {
                        sessionIndex++;
                        sessionChanged = true;
                        pot_last_change_ms = now_ms;
                    }
                    last_pot_percent = potPercent;
                }

                if (sessionChanged) {
                    display.clear();
                    for (int i = 0; i < totalSessionChoices; ++i) {
                        std::string choice = sessionChoices[i];
                        if (i == sessionIndex) choice = "* " + choice;
                        display.writeText(0, i * 8, choice.c_str());
                    }
                    display.render();
                    sessionChanged = false;
                }

                if (keyPress.isPressed()) {
                    // debounce
                    sleep_ms(button_debounce_ms);
                    while (keyPress.isPressed()) sleep_ms(10);

                    // perform action
                    switch (sessionIndex) {
                        case 0: // Stand
                            display.clear();
                            display.writeText(0,8, "sending to API ");
                            display.writeText(0,16, "stand request");
                            display.writeText(0,24, "...");
                            display.render();
                            // TODO: call API method e.g. userLogin.sendSessionAction("stand")
                            sleep_ms(500);
                            break;
                        case 1: // Sit
                            display.clear();
                            display.writeText(0,8, "sending to API ");
                            display.writeText(0,16, "sit request");
                            display.writeText(0,24, "...");
                            display.render();
                            sleep_ms(500);
                            // TODO: call API method e.g. userLogin.sendSessionAction("sit")
                            break;
                        case 2: // Logout
                            display.clear();
                            display.writeText(0,8, "sending to API");
                            display.writeText(0,16, "logout request");
                            display.writeText(0,24, "...");
                            display.render();
                            sleep_ms(500);
                            userLogin.logoutUser(); // consider non-blocking
                            currentState = MENU_MAIN;
                            firstLoginEntry = true;
                            break;
                    }

                    // reset selection after action
                    sessionIndex = 0;
                    sessionChanged = true;
                }
                break;
            }

            default:
                break;
        }

        // main loop delay (keeps UI responsive)
        sleep_ms(100);
    }
}


#else
void MyApp::run() {
    while (true) {
        float lux = ldr.readLux();                                         // Read light as float
        uint16_t luxInt = static_cast<uint16_t>(lux);                      // Integer version

        display.clear();                                                   // Clear OLED buffer
        display.writeText(5, 0, "LIGHT SENSOR");                           // Title
        display.writeText(5, 8, formatLux(lux).c_str());                   // Float format
        display.writeText(5, 16, formatLux(luxInt).c_str());               // Integer format
        display.render();                                                  // Render to OLED

        sleep_ms(1000);                                                    // Wait 1 second
    }
}
#endif