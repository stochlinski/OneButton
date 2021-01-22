/*
 InterruptOneButton.ino - Example for the OneButtonLibrary library.
 This is a sample sketch to show how to use the OneButtonLibrary
 to detect double-click events on a button by using interrupts. 
 The library internals are explained at
 http://www.mathertel.de/Arduino/OneButtonLibrary.aspx

 Good article on Arduino UNO interrupts:
 https://arduino.stackexchange.com/questions/30968/how-do-interrupts-work-on-the-arduino-uno-and-similar-boards
 ... and the processor datasheet.
  
 Setup a test circuit:
 * Connect a pushbutton to pin A1 (ButtonPin) and ground.
 * The pin 13 (StatusPin) is used for output attach a led and resistor to ground
   or see the built-in led on the standard arduino board.
   
 The Sketch shows how to setup the library and bind a special function to the doubleclick event.
 By using interrupts the doubleclick function will be called not from the main program.
*/

// 03.03.2011 created by Matthias Hertel
// 01.12.2011 extension changed to work with the Arduino 1.0 environment
// 04.11.2017 Interrupt version created.
// 04.11.2017 Interrupt version using attachInterrupt.

#include "OneButton.h"

#if defined(ARDUINO_AVR_UNO)
// Example for Arduino UNO with input button on pin 2 and builtin LED on pin 13
// attachInterrupt only supports pin 2 and 3 on UNO.
// See https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
#define PIN_INPUT 2
#define PIN_LED 13

#else if defined(ESP8266)
// #error This Example uses interrupts  NodeMCU with input button using FLASH button on D3 and using the led on -12 module (D4).
#define PIN_INPUT D3
#define PIN_LED D4

#endif


// Setup a new OneButton on pin A1.
OneButton button(PIN_INPUT, true);


// This function is called when the signal on the PIN_INPUT has changed.
void checkTicks()
{
  // include all buttons here to be checked
  button.tick(); // just call tick() to check the state.
}


// setup code here, to run once:
void setup()
{
  Serial.begin(115200);
  Serial.println("One Button Example with interrupts.");

  pinMode(PIN_LED, OUTPUT); // sets the digital pin for LED as output

  // setup interrupt routine
  pinMode(PIN_INPUT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT), checkTicks, CHANGE);

  // link the doubleclick function to be called on a doubleclick event.
  button.attachDoubleClick(doubleclick);

  // A1-Option:
  // it is possible to use e.g. A1 but then some direct register modifications and an ISR has to be used:
  // You may have to modify the next 2 lines if using another pin than A1
  // PCICR |= (1 << PCIE1);   // This enables Pin Change Interrupt 1 that covers the Analog input pins or Port C.
  // PCMSK1 |= (1 << PCINT9); // This enables the interrupt for pin 1 of Port C: This is A1.

} // setup

// A1-Option:
// The Interrupt Service Routine for Pin Change Interrupt 1
// This routine will only be called on any signal change on A1: exactly where we need to check.
// ISR(PCINT1_vect)
// {
//   // keep watching the push button:
//   button.tick(); // just call tick() to check the state.
// }


// main code here, to run repeatedly:
void loop()
{
  // You can implement other code in here or just wait a while
  delay(10);
} // loop


// this function will be called when the button was pressed 2 times in a short timeframe.
void doubleclick()
{
  Serial.println("x2");

  static int m = LOW;
  // reverse the LED
  m = !m;
  digitalWrite(13, m);
} // doubleclick

// End
