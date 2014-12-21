/* ###########################################################
  Extracting DC from a signal and then throwing the DC component out via PWM.
  The PWM is smoothed using an RC filter and then put through a difference amplifier with the original signal
  THis difference is then sent through analog A3 

  This exercise is one part of building a pulse oximeter, with the LED brightness once again being controlled
  by PWM (on pin 11, one more LED will need to be added later).
  
  Interrupt timer used to implement digital DC filter, taken from
  http://www.instructables.com/id/Arduino-Timer-Interrupts/
########################################################### */

#define redLED 11
// #define irLED 
#define DCout 3

float filtered_value, last_filtered_value;
int value, last_value;

void setup(){
  Serial.begin(115200);    // superfast serial communication
  
  double sampling_freq = 200;  // Fsampl in Hz
  int match_register = 16000000/(1024*sampling_freq) - 1; // match register to match that sampling frequency
  
  // STEP 1: we create an interrupt timer at the sampling frequency
  cli();//stop interrupts

  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = match_register; // HERE'S WHERE IT'S SET
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

  // STEP 2 : Set PWM frequency for pin 11, which will control LED brightness via RC smoothing of the pwm signal
  // using pin 11 since it is controlled by Timer2 (we;re using Timer1 for the interrupt)
  // first we'll change the frequency of the PWM to suit our needs...
  // ref: http://playground.arduino.cc/Main/TimerPWMCheatsheet
  TCCR2B = TCCR2B & 0b11111000 | 0x01;  // 31372.55 Hz

  // STEP 3 : Give the output pwm value in the range (0, 255) mapped to (0, 5)V
  pinMode(redLED, OUTPUT);
  float voltage = 2.4;  // voltage you wanna outputio
  analogWrite(redLED, int((voltage/5.0)*256) - 1);
}

ISR(TIMER1_COMPA_vect){   //  timer1 interrupt 100Hz
   // update variables...
   last_filtered_value = filtered_value;

   // will simply read A0 and then filter it
   value = analogRead(A0);
   
   // filter out PPG frequencies and only have the DC part
   // DSP LPF Cutoff = 
   filtered_value = last_filtered_value + 0.004*(value - last_filtered_value);
   
   // then send the values over serial...
   // but send an int because it's faster
   // Serial.println(int(filtered_value));
   // Serial.println(value);
   
   // the filtered value is now send out through PWM pin D3, which is also controlled by Timer2
   // filtered_value is in the range (0,1023) and the analogWrite value needs to be in the range (0, 255).
   // Hence we convert..
   analogWrite(DCout, int(256*(filtered_value + 1)/1024) - 1);
   
   // Then we read the value in again from an isolated analog pin and print it out..
   Serial.println(analogRead(A3));
}

void loop(){
  // nothign happens here
}
