// C++ code
//

//constant variables 
const uint8_t BTN_PIN = 2; //defining button pin number
const uint8_t LED_PIN = 12; //defining LED pin number

//variables defined for button and LED state
uint8_t buttonPrevState = LOW; 
uint8_t ledState = LOW;

//setup function - this will run once
void setup()
{
  //initializing the button pin as an input 
  pinMode(BTN_PIN, INPUT_PULLUP);
  //initializing the LED pin as an output
  pinMode(LED_PIN, OUTPUT);
  
  //interrupt:
  
  attachInterrupt(0, button, CHANGE);
  //initializing serial communication with a baud rate of 9600
  Serial.begin(9600);
}

void loop()
{
 // 
  
}

void button(){
	buttonPrevState = digitalRead(2);
  	digitalWrite(12, buttonPrevState);
    Serial.println(buttonPrevState);
}
