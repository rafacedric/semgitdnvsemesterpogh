/*
@startuml
skinparam classAttributeIconSize 0

class Button {
  ..attributes..

  - pin : uint
  - edge : uint32_t
  - pressCount : volatile int
  - toggleStateValue : volatile bool
  - debounceAlarm : alarm_id_t
  - lastState : volatile bool
  - eventFlag : volatile bool
  - instances[MAX_GPIO] : static Button*
  
  ..methods..
  
  + Button(pinNumber : uint, edgeType : uint32_t)
  + getPressCount() : int
  + isPressed() : bool
  + toggleState() : bool
  + hasEvent() : bool

  - gpio_isr(gpio : uint, events : uint32_t) : static
  - startDebounce() : void
  - debounceTimerCallback(id : alarm_id_t, user_data : void*) : static int64_t
  - handleDebounce() : int64_t
}
@enduml
*/

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "dbop.h"   // giver COMMENT() og DEBUG_INFO-makro

#define MAX_GPIO     32   // max number of GPIOs we support
#define DEBOUNCE_MS  50   // debounce time in ms

class Button {
private:
    uint pin;                  // GPIO pin
    uint32_t edge;             // edge type for interrupt
    volatile int pressCount;   // antal validerede tryk
    volatile bool toggleStateValue;

    alarm_id_t debounceAlarm;  // alarm id til debounce
    volatile bool lastState;
    volatile bool eventFlag;   // event-flag til hasEvent()

    static Button* instances[MAX_GPIO];  // array over instanser pr. pin

    static void gpio_isr(uint gpio, uint32_t events);           // statisk ISR
    void startDebounce();                                       // start debounce timer
    static int64_t debounceTimerCallback(alarm_id_t id, void*); // statisk timer callback
    int64_t handleDebounce();                                   // tjek tilstand efter debounce

public:
    Button(uint pinNumber, uint32_t edgeType);  // constructor

    int getPressCount() const;    // returner antal tryk
    bool isPressed() const;       // returner nuværende knaptilstand
    bool toggleState() const;     // returner toggle-state
    bool hasEvent();              // true hvis nyt event siden sidste aflæsning
};

#endif // PUSHBUTTON_H