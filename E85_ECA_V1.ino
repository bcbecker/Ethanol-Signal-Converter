/*******************************************************
This program will sample a 50-150Hz signal from a GM ethanol
sensor (P/N 13577429) based on ethanol content, then output a
0.5(E0) to 4.5V(E100) signal via PWM. The output is fed to an 
open 5v signal on the ECU (inline 3.3kOhm resistor works).


NOTE: Oscilloscope data was sourced, not produced by me.
Timers: https://www.arduino.cc/en/pmwiki.php?n=Tutorial/SecretsOfArduinoPWM
Wiring Diagram: (link here)

Input pin 8 (PB0) ICP1 on Atmega328 (Nano)
Output pin 11
********************************************************/

int inpPin = 8;               // defines input pin to pin 8
int outPin = 11;              // defines PWM output pin to pin 11

//defines global variables

volatile uint16_t revTick;    // unsigned 16bit, stores ticks per revolution (read from RAM)
uint16_t pwmOutput  = 0;      // unsigned 16bit, stores PWM value (0-255)
int Hz;                       // stores Hz input (50-150)
int ethanol = 0;              // stores ethanol content percentage (0-100)
float expectedV;              // stores expected voltage (GM sensors: 0.5V-4.5V)
float averagedHz = 0;         // stores averaged Hz based on count (50-150)
bool HzAvail = 0;             // averagedHz is or is not able to be calculated

float supplyVoltage = 5.0;    // sets voltage on 5V pin. Corrects the PWM output for unexact 5V
                              // *CHANGED FROM 4.9 TO 5.0 BASED ON READINGS ON BOARD*

double sum = 0;               // used in frequency averaging
int count = 0;                // used in frequency averaging

void setupTimer()   // setup timer control/interupt
{           
  TCCR1A = 0;      // normal mode
  TCCR1B = 132;    // (10000100) falling edge trigger, Timer = CPU Clock/256, noise cancellation ON
  TCCR1C = 0;      // normal mode
  TIMSK1 = 33;     // (00100001) input capture and overflow interupts enabled
  TCNT1 = 0;       // start from 0
}

ISR(TIMER1_CAPT_vect)     // PULSE DETECTED!  (interrupt automatically triggered, not called by main program)
{
  if (ICR1 > 5) {
    revTick = ICR1 - 4;    // saves duration of last revolution (-4 for filtering)
    HzAvail = 1;
  } else {
    revTick = 0;
  }
  TCNT1 = 0;              // restarts timer for next revolution
}

ISR(TIMER1_OVF_vect)      // counter overflow/timeout
{ revTick = 0; }          // ticks per second = 0


void setup()
{
  Serial.begin(9600);         // initializes serial port
  pinMode(inpPin,INPUT);      // initializes input pin
  setPwmFrequency(outPin,1);  // modifies frequency on PWM output
  setupTimer();               // initializes timer
}
 
void loop()
{  
  if (HzAvail = 1) {
    
    // averages several reading together
    
    Hz = 62200 / revTick;         // 62200 determined to be optimal (oscope)
    sum = sum + Hz;
    count = count + 1;
    HzAvail = 0;
    
    if (count > 30) {
      averagedHz = (sum / count);
      Serial.println(averagedHz);
      sum = 0;
      count = 0;                  // resets sum and count for next loop
    }
  } else {
    Hz = 0;                       // if fuel is present, should never see this
    Serial.println("Hz Error");
  }

  // calculates ethanol percentage
  // zeroes out the ethanol percentage if the Hz measurement is not in range
  
  if ((averagedHz > 49) && (averagedHz < 151)) {
    ethanol = (averagedHz - 50);
  } else {
    ethanol = 0;                // if fuel is present, should never see this
    Serial.println("E Error");
  }

  expectedV = (((ethanol / 100) * 4) + 0.5);
  
  // PWM Output
  
  pwmOutput = (255 * (expectedV / supplyVoltage));   // calculates output PWM for ECU
  analogWrite(outPin, pwmOutput);                  // writes the PWM value to output pin

}

void setPwmFrequency(int pin, int divisor) { // raises the timers linked to the PWM outputs
  byte mode;                                 // so the PWM frequency can be raised or lowered.
                                             // Prescaler of 1 sets PWM output to 32KHz (pin 3, 11)
  if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
