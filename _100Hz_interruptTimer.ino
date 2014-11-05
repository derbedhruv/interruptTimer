// http://www.instructables.com/id/Arduino-Timer-Interrupts/

void setup(){
  Serial.begin(115200);    // superfast serial communication
  
  // STEP 1: we create an interrupt timer at precisely 100Hz
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

  // STEP 2 : Set PWM frequency for pin 11, which will control LED brightness via RC smoothing of the pwm signal
  // using pin 11 since it is controlled by Timer2 (we;re using Timer1 for the interrupt)
  // first we'll change the frequency of the PWM to suit our needs...
  // ref: http://playground.arduino.cc/Main/TimerPWMCheatsheet
  TCCR2B = TCCR2B & 0b11111000 | 0x01;  // 31372.55 Hz

  // STEP 3 : Give the output pwm value in the range (0, 255) mapped to (0, 5)V
  pinMode(11, OUTPUT);
  float voltage = 2.4;  // voltage you wanna outputio
  analogWrite(11, int((voltage/5.0)*256) - 1);
}

ISR(TIMER1_COMPA_vect){   //  timer1 interrupt 100Hz
   // will simply read A0 and throw the value into serial
   Serial.println(analogRead(A0));

}

void loop(){
  // nothign happens here
}
