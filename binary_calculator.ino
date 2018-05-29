// This is a simple binary calculator that takes two, 4-bit numbers
// and adds them together to produce a 5-bit result.
// I've been wanting to do a simple, binary calculator ever since
// seeing a binary clock program running on the micro:bit.
// The debounce code is adapted directly from the example included
// with the Arduino IDE at "File", "Examples", "Digital", "Debounce".
// I learned how to display pixels on the micro:bit's display from
// adafruit's matrix demo included with their micro:bit library.
// If you haven't already, check out their tutorial at
// https://learn.adafruit.com/use-micro-bit-with-arduino
// Other than that, the bulk of the logic is my own design, which
// undoubtedly means that it could be improved, so please feel free
// to tinker with the code ;)
// Good luck, and happy coding!

#include <Adafruit_Microbit.h>

Adafruit_Microbit_Matrix microbit;

int buttonState_A; // we'll use this to get the current state of button A.
int buttonState_B; // we'll use this to get the current state of button B.
int lastState_A = HIGH; // we'll use this to store the previous state of button A.
int lastState_B = HIGH; // we'll use this to store the previous state of button B.
int a = 0; // we'll store our first input as A.
int b = 0; // we'll store our second input as B.
int c; // we'll store the sum as C. notice a pattern?

unsigned long lastDebounceTime = 0; // this will represent the last time a button was pressed.
unsigned long debounceDelay = 50; // this is the debounce time.

void setup() {
  // let's go ahead and get the sum.
  c = a + b;
  
  // next we'll set up button A and button B as inputs.
  pinMode(PIN_BUTTON_A, INPUT);
  pinMode(PIN_BUTTON_B, INPUT);

  // finally we'll start up the micro:bit display.
  microbit.begin();
}

void loop() {
  // we need to get the current state of each button.
  int buttonRead_A = digitalRead(PIN_BUTTON_A);
  int buttonRead_B = digitalRead(PIN_BUTTON_B);

  // then we need to check to see if a button has been pressed
  // (i.e. the input went from HIGH to LOW), and we've waited long enough
  // since the last button press to ignore any noise.
  // note that the micro:bit's buttons default to HIGH
  // and pressing a button pulls it to LOW

  // if there has been a change:
  if ((buttonRead_A != lastState_A) or (buttonRead_B != lastState_B)) {
    // reset the debouncing timer.
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is, it's been there for longer than the debounce
    // delay, so we can treat is as the current state:

    // if the state of button A has changed:
    if (buttonRead_A != buttonState_A) {
      buttonState_A = buttonRead_A;

      // increment the counter for button A only if the button state is LOW.
      if (buttonState_A == LOW) {
        a++;
        // if counter for button A is greater than 15 (i.e. larger than 4 bits):
        if (a > 15) {
          // reset it.
          a = 0;
        }

        // let's calculate the new sum,
        calculate();

        // and update the display.
        displayBinary();
      }
    }

    // if the state of button B has changed:
    if (buttonRead_B != buttonState_B) {
      buttonState_B = buttonRead_B;

      // increment the counter for button B only if the button state is LOW.
      if (buttonState_B == LOW) {
        b++;
        // if counter for button B is greater than 15 (i.e. larger than 4 bits):
        if (b > 15) {
          // reset it
          b = 0;
        }

        // let's calculate the new sum,
        calculate();

        // and update the display.
        displayBinary();
      }
    }
  }

  // we'll save the readings as the lastState for the next iteration.
  lastState_A = buttonRead_A;
  lastState_B = buttonRead_B;
}

void calculate() {
  // some fancy math to calculate the new sum!
  c = a + b;
}

void displayBinary() {
  // first let's clear the display.
  microbit.clear();

  // then we'll convert A to binary.
  int d = a; // we'll use a temporary, local variable to store A.
  int i = 4; // we'll also use a local variable as a counter for our loops.

  // we'll use a while loop to convert A into binary and display it one pixel at a time.
  // note that this loop will run 4 times, which is important because.
  // the input could be a 4-bit number.
  while (i > 0) {
    // check to see if the remainder of d / 2 is a 1 or a 0.
    if ((d % 2) == 1) {
      // turn on an LED if the remainder is a 1.
      microbit.drawPixel(i, 0, LED_ON);
    }
    else {
      // turn off the LED if the remainder is a 0.
      microbit.drawPixel(i, 0, LED_OFF);
    }
    // now divide by 2, and decrease the counter by 1.
    d = d / 2;
    i--;
  }

  // we'll convert B to binary next.
  d = b; // we'll reuse our temporary, local variable to store B.
  i = 4; // we'll also reuse our local, loop counter variable.

  // we'll use another while loop to convert B into binary.
  // note that this loop will run 4 times, which is important because
  // the input could be a 4-bit number.
  while (i > 0) {
    // check to see if the remainder of d / 2 is a 1 or a 0.
    if ((d % 2) == 1) {
      // turn on an LED if the remainder is a 1.
      microbit.drawPixel(i, 2, LED_ON);
    }
    else {
      // turn off the LED if the remainder is a 0.
      microbit.drawPixel(i, 2, LED_OFF);
    }
    // now divide by 2, and decrease the counter by 1.
    d = d / 2;
    i--;
  }

  // finally let's convert the sum into binary.
  d = c; // we'll reuse our temporary, local variable to store C.
  i = 4; // we'll also reuse out local, loop counter variable.

  // we'll use one last loop to convert the sum into binary.
  // note that this loop is okay with i being equal to 0.
  // unlike the previous loops, this loop will run 5 times instead of 4,
  // which is important because the sum could be a 5-bit number.
  while (i >= 0) {
    // check to see if the remainder of d / 2 is a 1 or a 0.
    if ((d % 2) == 1) {
      // turn on an LED if the remainder is a 1.
      microbit.drawPixel(i, 4, LED_ON);
    }
    else {
      // turn off the LED if the remainder is a 0.
      microbit.drawPixel(i, 4, LED_OFF);
    }
    // now divide by 2, and decrease the counter by 1.
    d = d / 2;
    i--;
  }
}

