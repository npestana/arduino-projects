/*****************************************************************************/
/*  Name    : Multiplexing 74HC595, Knight Rider 1982 (Analog Input)         */
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

byte ledsState[11];
int ledsStateSpeed = 75;
int potentiometerPin = A0;

void setup() {
  /*
   *  Pins used to talk to the chip configured
   *  as output so we can write to them
   */
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  

  /* Leds states (8 bit) */
  ledsState[0] = 0x80;  /* 10000000 */
  ledsState[1] = 0xC0;  /* 11000000 */
  ledsState[2] = 0xE0;  /* 11100000 */
  ledsState[3] = 0x70;  /* 01110000 */
  ledsState[4] = 0x38;  /* 00111000 */
  ledsState[5] = 0x1C;  /* 00011100 */
  ledsState[6] = 0xE;   /* 00001110 */
  ledsState[7] = 0x7;   /* 00000111 */
  ledsState[8] = 0x3;   /* 00000011 */
  ledsState[9] = 0x1;   /* 00000001 */
  ledsState[10] = 0x00; /* 00000000 */
}

void loop() {
  /*
   *  i == 0 Left -> Right
   *  i == 1 Left <- Right
   */
  for (int i = 0; i < 2; i++) {
    byte bitOrder;

    if (i == 0)
      bitOrder = LSBFIRST;
    else
      bitOrder = MSBFIRST;

    /*
     *  Left -> Right
     *  j == 2:  11100000 | 00000000
     *  j == 3:  01110000 | 00000000
     *  j == 4:  00111000 | 00000000
     *  j == 5:  00011100 | 00000000
     *  j == 6:  00001110 | 00000000
     *  j == 7:  00000111 | 00000000
     *  j == 8:  00000011 | 10000000
     *  j == 9:  00000001 | 11000000
     *  j == 10: 00000000 | 11100000
     *  j == 11: 00000000 | 01110000
     *  j == 12: 00000000 | 00111000
     *  j == 13: 00000000 | 00011100
     *  j == 14: 00000000 | 00001110
     *  j == 15: 00000000 | 00000111
     */
    for (int j = 2; j < 16; j++) {
      int ledsStatePosA, ledsStatePosB;

      if (i == 0 && j < 10)
        ledsStatePosA = j;
      else if (i == 1 && j > 7)
        ledsStatePosA = j - 8;
      else
        ledsStatePosA = 10;

      if (i == 0 && j > 7)
        ledsStatePosB = j - 8;
      else if (i == 1 && j < 10)
        ledsStatePosB = j;
      else
        ledsStatePosB = 10;

      updateLedsState(bitOrder, ledsStatePosA, ledsStatePosB);

      ledsStateSpeed = 75 + 0.9265 * analogRead(potentiometerPin);

      delay(ledsStateSpeed);
    }
  }
}

/*
 *  NOTE: We need to use shiftOut from last to first member of the
 *  registers cascade.
 */
void updateLedsState(byte bitOrder, int ledsStatePosA, int ledsStatePosB) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, bitOrder, ledsState[ledsStatePosB]);
  shiftOut(dataPin, clockPin, bitOrder, ledsState[ledsStatePosA]);
  digitalWrite(latchPin, HIGH);
}
