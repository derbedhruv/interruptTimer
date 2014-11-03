// http://www.instructables.com/id/Arduino-Timer-Interrupts/
// will be doing DSP realtime to remove that ugly bloody 50Hz feature and also do some smoothing andshit.

int currentValue, filteredValue, last_filteredValue, last_value;

void setup(){
  Serial.begin(115200);    // superfast  
  
  cli();//stop interrupts

  // set timer1 interrupt at 100Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 155;// = (16*10^6) / (100*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts

  // next this is something I'm going to try: how PWM LED control shows up on the photodiode...
  // using pin 11 since it is controlled by Timer2 (we;re using Timer1 for the interrupt)
  pinMode(11, OUTPUT);
  analogWrite(11, 55);
}

ISR(TIMER1_COMPA_vect){   //  timer1 interrupt 100Hz
   // will simply read A0 and throw the value into serial
   Serial.println(analogRead(A0));
   
   /*
   last_value = currentValue;
   
   currentValue = (float) analogRead(A0);  // read a piece of data
   filteredValue = currentValue - last_value;  // filter it using a digital filter, this is the simplest lowpass filter
   
   Serial.println(filteredValue);  // print it to serial
   /**/
}

void loop(){
  // nothign much happens here
}
