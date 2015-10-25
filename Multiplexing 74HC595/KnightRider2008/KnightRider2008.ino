/*****************************************************************************/
/*  Name    : Multiplexing 74HC595, Knight Rider 2008                        */
/*  Author  : Nelson Pestana, https://github.com/npestana/                   */
/*  Date    : 25 October, 2015                                               */
/*  Version : 1.0                                                            */
/*  Notes   : Code for using a 74HC595 Shift Register                        */
/*          : to create KITT front panel effect                              */
/*****************************************************************************/

/* Pins used */
int clockPin = 12;
int dataPin = 11;
int latchPin = 8;

byte ledsState[9];
int ledsStateSpeed = 100;

void setup() {
  /* 
   *  Pins used to talk to the chip configured
   *  as output so we can write to them
   */
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  /* Leds states (8 bit) */
  ledsState[0] = 0xFF; /* 11111111 */
  ledsState[1] = 0xFE; /* 11111110 */
  ledsState[2] = 0xFC; /* 11111100 */
  ledsState[3] = 0xF8; /* 11111000 */
  ledsState[4] = 0xF0; /* 11110000 */
  ledsState[5] = 0xE0; /* 11100000 */
  ledsState[6] = 0xC0; /* 11000000 */
  ledsState[7] = 0x80; /* 10000000 */
  ledsState[8] = 0x00; /* 00000000 */
}

void loop() {
  int ledsStatePos;

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 9; i++) {
      if(j == 1 || j == 3)
        ledsStatePos = i;
      else
        ledsStatePos = 8 - i;

       if (j == 0 || j == 3)
        updateLedsState(LSBFIRST, MSBFIRST, ledsStatePos);
       else
        updateLedsState(MSBFIRST, LSBFIRST, ledsStatePos);

      delay(ledsStateSpeed);
    }
  }
}

/*
 *  NOTE: We need to use shiftOut from last to first member of the
 *  registers cascade.
 */
void updateLedsState(byte bitOrderA, byte bitOrderB, int ledsStatePos) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, bitOrderB, ledsState[ledsStatePos]);
  shiftOut(dataPin, clockPin, bitOrderA, ledsState[ledsStatePos]);
  digitalWrite(latchPin, HIGH);
}
