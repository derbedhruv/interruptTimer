//http://www.instructables.com/id/Arduino-Timer-Interrupts/

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

}

ISR(TIMER1_COMPA_vect){   //  timer1 interrupt 100Hz
   // will simply read A0 and throw the value into serial
   Serial.println(analogRead(14));
}

void loop(){
  // nothign much happens here
}
