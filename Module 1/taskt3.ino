//Two buttons for each input device and led pin are defined
const byte button_1 = 2;
const byte button_2 = 3;
const byte led = 13;

//led_state boolean initialized with a baud rate of 9600
bool led_state = false;

void setup() {
    //serial communication is initiated with a baud rate of 9600
    Serial.begin(9600);
    //pinmodes are defined as input for the two buttons
    //rinmode of led is set as output
    pinMode(led,OUTPUT);
    pinMode(button_1,INPUT);
    pinMode(button_2,INPUT);

    //two interrupts are added for each of the external input pins of both the buttons
    attachInterrupt(digitalPinToInterrupt(button_1), Button1, HIGH);
    attachInterrupt(digitalPinToInterrupt(button_2), Button2, RISING);
}


void loop() {
  delay(10);
  //
}

//Button1 function for handling the interrupt occuring due to the first push button
void Button1()
{
  led_state = !led_state;
  //LED TOGGLE
  digitalWrite(led, led_state);
  Serial.println("");
  Serial.println("A button#1 interrupt has occured");
  
}

//This function handles the interrupt occuring due to the second button press
void Button2(){
  led_state = !led_state;
  //LED TOGGLE
  digitalWrite(led, led_state);
  Serial.println("");
  Serial.println("a button#2 interrupt has occured");
}