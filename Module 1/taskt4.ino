#include "PinChangeInt.h"

//Buttons: 1, 2 and 3 pins are defined. Furthermode, led pin 13 is also defined
const byte SLIDER = 2;
const byte BUTTON_2 = 3;
const byte BUTTON_3 = 4;
const int led = PB5;

//timer compare values are setup

const uint16_t t1_load = 0;
const uint16_t t1_compare = 15625; // A second long timer with 1024 scaler value

void setup() {
    //Serial communication value
    Serial.begin(9600);
    //Pin modes are defined
    pinMode(SLIDER, INPUT);
    pinMode(BUTTON_2, INPUT);
    pinMode(BUTTON_3, INPUT);

    //setting the led pinmode as output
    DDRB |= (1 << led);

    //Three external interrupts, each for every button
    attachInterrupt(digitalPinToInterrupt(PIR), Interrupt1, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ROTARTY), Interrupt2, RISING);
    attachPinChangeInterrupt(BUTTON, Interrupt3, RISING);

    reset timer1 control register A to its default value 
    TCCR1A = 0;

    // Clear on compare
    TCCR1B &= ~(1 << WGM13);
    TCCR1B |= (1 << WGM12);

    // setting the prescaler value to 1024
    TCCR1B |= (1 << CS12);  // 1
    TCCR1B &= ~(1 << CS11); // 0
    TCCR1B |= (1 << CS10);  // 1

    //setting timer and compare values
    TCNT1 = t1_load;
    OCR1A = t1_compare;

    //enable timer1 interrupt
    TIMSK1 = (1 << OCIE1A);

    //enable all global interrupts
    sei();
}

void loop() {
    delay(100);

}

// Interrupt service routine --- handles the timer internal interrupt

ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << led);
}

void Interrupt1() {

    Serial.println("");
    Serial.println("Detected: Interrupt 1");
}

void Interrupt2() {
    Serial.println("");
    delay(100);
    Serial.println("Detected: Interrupt 2");
}

void Interrupt3() {
    Serial.println("");
    Serial.println("Detected: Interrupt 3");
}