
#include "SevenSegmentTM1637.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
uint32_t   _pinClk;            // clock pin
uint32_t  _pinDIO;            // digital out pin
uint8_t         _numCols;           // number of columns
uint8_t         _numRows;           // number of rows
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_float(addr) (*(const float *)(addr))

#define pgm_read_byte_near(addr) pgm_read_byte(addr)
#define pgm_read_word_near(addr) pgm_read_word(addr)
#define pgm_read_dword_near(addr) pgm_read_dword(addr)
#define pgm_read_float_near(addr) pgm_read_float(addr)
#define pgm_read_byte_far(addr) pgm_read_byte(addr)
#define pgm_read_word_far(addr) pgm_read_word(addr)
#define pgm_read_dword_far(addr) pgm_read_dword(addr)
#define pgm_read_float_far(addr) pgm_read_float(addr)
uint8_t   _backLightValue;          // brightness of the display (0..100)
uint8_t   _cursorPos;               // current cursor position
uint16_t  _printDelay;              // print delay in ms (multiple chars)
uint8_t   _colonOn;                 // colon bit if set
uint8_t   _rawBuffer[TM1637_MAX_COLOM];// hold the last chars printed to display
/* Static version of low level function
* If using more than one display, this saves some space since these methods will be shared among all instances/objects of the class
*/
static bool    SevenSegmentTM1637_commandMultiLL(uint32_t pinClk, uint32_t pinDIO, uint8_t cmd);
static bool    SevenSegmentTM1637_commandBufferLL(uint32_t pinClk, uint32_t pinDIO, const uint8_t* command, uint8_t length);
static void    SevenSegmentTM1637_comStartLL(uint32_t pinClk, uint32_t pinDIO);
static void    SevenSegmentTM1637_comWriteByteLL( uint32_t pinClk, uint32_t pinDIO, uint8_t command);
static bool    SevenSegmentTM1637_comAckLL(uint32_t pinClk, uint32_t pinDIO);
static void    SevenSegmentTM1637_comStopLL(uint32_t pinClk, uint32_t pinDIO);
bool    SevenSegmentTM1637_command(uint32_t pinClk, uint32_t pinDIO, const uint8_t* commands, uint8_t length);


/**
 * ComACK API
 * @return returns true on success
 */
bool SevenSegmentTM1637_comAck();
void SevenSegmentTM1637(uint32_t pinClk, uint32_t pinDIO)  {


	_pinClk = pinClk;
	_pinDIO = pinDIO;

	SevenSegment_pinSet(_pinClk);
	SevenSegment_pinSet(_pinDIO);
  // setup defaults
	SevenSegmentTM1637_setCursor(0, TM1637_DEFAULT_CURSOR_POS);
	SevenSegmentTM1637_setPrintDelay(TM1637_DEFAULT_PRINT_DELAY);
	SevenSegmentTM1637_setColonOn(TM1637_DEFAULT_COLON_ON);
	SevenSegmentTM1637_setBacklight(TM1637_DEFAULT_BACKLIGHT);

	SevenSegmentTM1637_commandSingle(	    TM1637_COM_SET_DATA |
		    TM1637_SET_DATA_WRITE |
		    TM1637_SET_DATA_A_ADDR |
		    TM1637_SET_DATA_M_NORM);

};

// Print API ///////////////////////////////////////////////////////////////////
// single byte
size_t  SevenSegmentTM1637_writeByte(uint8_t byte) {
  TM1637_DEBUG_PRINT(F("write byte:\t")); TM1637_DEBUG_PRINTLN((char)byte);

  size_t n = 0;
  if ( _cursorPos == _numCols ) {
	  SevenSegmentTM1637_shiftLeft(_rawBuffer, _numCols);
    _rawBuffer[_cursorPos] = SevenSegmentTM1637_encodeChar( (char)byte );
    // buffer, length, position
    SevenSegmentTM1637_printRawBuffer( _rawBuffer, _numCols, 0);
    ++n;
  };

  if (_cursorPos < _numCols) {
    _rawBuffer[_cursorPos] = SevenSegmentTM1637_encodeInteger( byte );
    // buffer, length, position
    SevenSegmentTM1637_printRawBuffer( _rawBuffer, _cursorPos+1, 0);
    SevenSegmentTM1637_setCursor(1, _cursorPos + 1);
    ++n;
  }
  return n;
}

// null terminated char array
size_t  SevenSegmentTM1637_writeString(const char* str) {
  TM1637_DEBUG_PRINT(F("write char*:\t")); TM1637_DEBUG_PRINTLN(str);
  uint8_t encodedBytes[4];

  SevenSegmentTM1637_encodeBuffer(encodedBytes, str, 4);
  uint8_t i =4;
  while( str[i] != '\0' ) {
	  SevenSegmentTM1637_printRawBuffer(encodedBytes,4,0);
	  SevenSegmentTM1637_shiftLeft(encodedBytes, 4);
    encodedBytes[3] = SevenSegmentTM1637_encodeChar( str[i] );
    i++;
    if ( i == TM1637_MAX_CHARS) {
      break;
    }
  }
  return i;
};

// byte array with length
size_t  SevenSegmentTM1637_writeBuffer(const uint8_t* buffer, size_t size) {
  TM1637_DEBUG_PRINT(F("write uint8_t*:\t("));
  for(size_t i=0; i < size; i++) {
    TM1637_DEBUG_PRINT((char)buffer[i]);
    TM1637_DEBUG_PRINT(i == size -1?F(""):F(", "));
  }
  TM1637_DEBUG_PRINT(F(") "));
  TM1637_DEBUG_PRINT(size);

  uint8_t encodedBytes[TM1637_MAX_CHARS];

  if ( size > TM1637_MAX_CHARS) {
    size = TM1637_MAX_CHARS;
  }
  size_t length = SevenSegmentTM1637_encodeBuffer(encodedBytes, buffer, size);
  TM1637_DEBUG_PRINT(F(" (")); TM1637_DEBUG_PRINT(length); TM1637_DEBUG_PRINT(F(", "));
  TM1637_DEBUG_PRINT(_cursorPos); TM1637_DEBUG_PRINTLN(F(")"));
  SevenSegmentTM1637_printRawBuffer(encodedBytes, length, _cursorPos);
  return length;
};

// Liquid cristal API
void SevenSegmentTM1637_begin(uint8_t cols, uint8_t rows) {
  _numCols = cols;
  _numRows = rows;
  SevenSegmentTM1637_clear();
};



void SevenSegmentTM1637_clear(void) {
  uint8_t rawBytes[4] = {1,1,1,1};
  SevenSegmentTM1637_printRawBuffer(rawBytes,4,0);
  SevenSegmentTM1637_home();
};

void SevenSegmentTM1637_home(void) {
	SevenSegmentTM1637_setCursor(0, 0);
};

void SevenSegmentTM1637_setCursor(uint8_t row, uint8_t col) {
  col = (col > TM1637_MAX_COLOM -1)?TM1637_MAX_COLOM-1:col;
  _cursorPos = col;

};

void SevenSegmentTM1637_setBacklight(uint8_t value) {
  value = (value    > 100 )?100:value;  // 0..100 brightness
  // Store the backlight value
  _backLightValue = value;
  // scale backlight value to 0..8
  value /= 10;                          // 0..10
  value = (value > 8   )?  8:value;     // only 8 levels and off
  uint8_t cmd = TM1637_COM_SET_DISPLAY;;
  switch ( value ) {
    case 0:
      cmd |= TM1637_SET_DISPLAY_OFF;
      break;
    case 1:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_1;
      break;
    case 2:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_2;
      break;
    case 3:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_4;
      break;
    case 4:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_10;
      break;
    case 5:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_11;
      break;
    case 6:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_12;
      break;
    case 7:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_13;
      break;
    case 8:
      cmd |= TM1637_SET_DISPLAY_ON | TM1637_SET_DISPLAY_14;
      break;
    };
#if TM1637_DEBUG
    bool ack = command(cmd);
    TM1637_DEBUG_PRINT(F("SET_DISPLAY:\t")); TM1637_DEBUG_PRINTLN((
      cmd
    ), BIN);
    TM1637_DEBUG_PRINT(F("Acknowledged:\t")); TM1637_DEBUG_PRINTLN(ack);
#else
    SevenSegmentTM1637_commandSingle(cmd);
#endif
};

void SevenSegmentTM1637_setContrast(uint8_t value) {
	SevenSegmentTM1637_setBacklight(value);
}

void SevenSegmentTM1637_on(void) {
	SevenSegmentTM1637_setBacklight(TM1637_DEFAULT_BACKLIGHT);
};

void SevenSegmentTM1637_off(void) {
	SevenSegmentTM1637_setBacklight(0);
  SevenSegmentTM1637_clear();
};

// SevenSegmentTM1637 public methods
void  SevenSegmentTM1637_blink(uint8_t blinkDelay, uint8_t repeats, uint8_t maxBacklight, uint8_t minBacklight) {
  for (uint8_t i=0; i < repeats; i++) {
    setBacklight(minBacklight);     // turn backlight off
    delay(blinkDelay);
    setBacklight(maxBacklight);     // turn backlight on
    delay(blinkDelay);
  }
  // restore backlight
  setBacklight(_backLightValue);
}

void  SevenSegmentTM1637_setPrintDelay(uint16_t printDelay) {
  _printDelay = printDelay;
};

bool  SevenSegmentTM1637_getColonOn(void) {
  return (_colonOn);
};

void  SevenSegmentTM1637_setColonOn(bool setToOn) {
    _colonOn = setToOn;
}




void  SevenSegmentTM1637_printRaw(uint8_t rawByte, uint8_t position) {
  uint8_t cmd[2];
  cmd[0] = TM1637_COM_SET_ADR | position;
  cmd[1] = rawByte;
  if (position == 1) { cmd[1]|=(_colonOn)?TM1637_COLON_BIT:0; };
  SevenSegmentTM1637_commandMulti(cmd, 2);
};

void  SevenSegmentTM1637_printRawBuffer(const uint8_t* rawBytes, size_t length, uint8_t position) {
  // if fits on display
  if ( (length + position) <= _numCols) {
    uint8_t cmd[5] = {0, };
    cmd[0] = TM1637_COM_SET_ADR | (position & 0x07);  // sets address
    memcpy(&cmd[1], rawBytes, length);       // copy bytes

    // do we have to print a colon?
    if ( position < 2 ) { // printing after position 2 has never a colon
      if ( position == 0 && length >= 2) {
        // second index is the colon
        cmd[2] |= (_colonOn)?TM1637_COLON_BIT:0;
      } else {
        // first index is the colon
        cmd[1] |= (_colonOn)?TM1637_COLON_BIT:0;
      }
    }
    // TM1637_DEBUG_PRINT(F("ADDR :\t")); TM1637_DEBUG_PRINTLN(cmd[0],BIN);
    // TM1637_DEBUG_PRINT(F("DATA0:\t")); TM1637_DEBUG_PRINTLN(cmd[1],BIN);
    SevenSegmentTM1637_commandMulti(cmd, length+1);                           // send to display
  }
  // does not fit on display, need to print with delay
  else {
    // First print 1-4 characters
    uint8_t numtoPrint = _numCols - position;
    SevenSegmentTM1637_printRawBuffer(rawBytes, numtoPrint, position);
    HAL_Delay(_printDelay);

    // keep printing 4 characters till done
    uint8_t remaining = length - numtoPrint + 3;
    uint8_t i         = 1;
    while( remaining >= _numCols) {
    	SevenSegmentTM1637_printRawBuffer(&rawBytes[i], _numCols, 0);
    	 HAL_Delay(_printDelay);
      remaining--;
      i++;
    };
  }

};

// Helpers
uint8_t SevenSegmentTM1637_encodeChar(char c) {
  if ( c < ' ') { // 32 (ASCII)
    return 0;
  }
  return pgm_read_byte_near(Asciimap + c - ' ');
};

uint8_t SevenSegmentTM1637_encodeInteger(int16_t d) {
  // can only encode single digit
  if ( d > 9 || d < 0) {
    return 0;
  };
  return pgm_read_byte_near(Asciimap + d + '0' - ' ');
};

size_t  SevenSegmentTM1637_encodeString(uint8_t* buffer, const char* str, size_t bufferSize) {
  size_t i;

  for (i=0; i < bufferSize; i++) {
    if (str[i] == '\0' ) {
      return i;
    }
    buffer[i] = encode( str[i] );
  };
  return i;
}

size_t  SevenSegmentTM1637_encodeBuffer(uint8_t* buffer, const uint8_t* byteArr, size_t bufferSize) {
  size_t i;

  for (i=0; i < bufferSize; i++) {
    buffer[i] = SevenSegmentTM1637_encodeChar( (char)byteArr[i] );
  };
  return i;
}

void    SevenSegmentTM1637_shiftLeft(uint8_t* buffer, size_t length) {
  for (uint8_t i=0; i < length ; i++) {
    buffer[i] = buffer[i+1];
  }
}

// SevenSegmentTM1637 LOW LEVEL
bool    SevenSegmentTM1637_commandSingle(uint8_t cmd) {
  return SevenSegmentTM1637_commandMultiLL(_pinClk, _pinDIO, cmd);
};

bool SevenSegmentTM1637_commandMultiLL(uint32_t pinClk, uint32_t pinDIO, uint8_t cmd) {

	SevenSegmentTM1637_comStart(pinClk, pinDIO);
	SevenSegmentTM1637_comWriteByte( cmd);
   bool acknowledged = SevenSegmentTM1637_comAck(pinClk, pinDIO);
  SevenSegmentTM1637_comStop(pinClk, pinDIO);
  return  acknowledged;
}

bool    SevenSegmentTM1637_commandMulti(const uint8_t* commands, uint8_t length) {
   return SevenSegmentTM1637_command(_pinClk, _pinDIO, commands, length);
};

bool    SevenSegmentTM1637_command(uint32_t pinClk, uint32_t pinDIO, const uint8_t* commands, uint8_t length) {
	  bool acknowledged = true;
	  SevenSegmentTM1637_comStart(pinClk, pinDIO);
	  for (uint8_t i=0; i < length;i++) {
		  SevenSegmentTM1637_comWriteByte( commands[i]);
	    acknowledged &= 	  SevenSegmentTM1637_comAck(pinClk, pinDIO);
	  };
	  SevenSegmentTM1637_comStop(pinClk, pinDIO);
	  return acknowledged;
}

uint8_t SevenSegmentTM1637_comReadByte(void) {

  uint8_t readKey = 0;

  SevenSegmentTM1637_comStart();
  SevenSegmentTM1637_comWriteByte(TM1637_COM_SET_DATA | TM1637_SET_DATA_READ);
  SevenSegmentTM1637_comAck();

  SevenSegmentTM1637_pinAsInput(_pinDIO);

  SevenSegmentTM1637_pinSet(_pinDIO);
  SevenSegmentTM1637_delayMicroseconds(5);

  for ( uint8_t i=0; i < 8; i++) {

	readKey >>= 1;
	SevenSegmentTM1637_pinClear(_pinClk);
	SevenSegmentTM1637_delayMicroseconds(30);

	SevenSegmentTM1637_pinSet(_pinClk);

	if ( isHigh(_pinDIO) ) {
	  readKey = readKey | 0x80;
	};

	SevenSegmentTM1637_delayMicroseconds(30);


  };
  SevenSegmentTM1637_pinAsOutput(_pinDIO);
  SevenSegmentTM1637_comAck();
  SevenSegmentTM1637_comStop();
  return readKey;
};
/**
 *
 * @param command
 */
void    SevenSegmentTM1637_comWriteByte(uint8_t command)  {
	SevenSegmentTM1637_comWriteByteLL(_pinClk, _pinDIO, command);
};
/**
 *
 * @param pinClk
 * @param pinDIO
 * @param command
 */
void SevenSegmentTM1637_comWriteByteLL(uint32_t pinClk,
		uint32_t pinDIO, uint8_t command) {
  // CLK in bits
    for (int i = 0; i < 8; ++i) {
        _tm1637ClkLow();
        if (command & 0x01) {
            _tm1637DioHigh();
        }
        else {
            _tm1637DioLow();
        }
        _tm1637DelayUsec(3);
        command >>= 1;
        _tm1637ClkHigh();
        _tm1637DelayUsec(3);
    }
}
/**
 *
 */
void SevenSegmentTM1637_comStart()  {
	SevenSegmentTM1637_comStartLL(_pinClk, _pinDIO);
};
/**
 *
 * @param pinClk
 * @param pinDIO
 */
void SevenSegmentTM1637_comStartLL(uint32_t pinClk, uint32_t pinDIO) {
	/* set CLK high */
	_tm1637Start();
}
/**
 * Communication stop API
 */
void SevenSegmentTM1637_comStop( ){
	/* stop communication */
	_tm1637Stop();
};
/**
 * Communication Stop API
 * @param pinClk CLK pin
 * @param pinDIO DIO pin
 */
void SevenSegmentTM1637_comStopLL(uint32_t pinClk, uint32_t pinDIO) {
	/* set clock low */
	SevenSegment_pinClear(pinClk);
	/* delay for clk */
	SevenSegmentTM1637_delayMicroseconds(TM1637_CLK_DELAY_US);
	/* set DIO low */
	SevenSegment_pinClear(pinDIO);
	/* delay for DIO */
	SevenSegmentTM1637_delayMicroseconds(TM1637_CLK_DELAY_US);
	/* set CLK high */
	SevenSegment_pinSet(pinClk);
	/* delay for clk */
	SevenSegmentTM1637_delayMicroseconds(TM1637_CLK_DELAY_US);
	/* set DIO high */
	SevenSegment_pinSet(pinDIO);
}
/**
 * ComACK API
 * @return returns true on success
 */
bool SevenSegmentTM1637_comAck(){
	/* start communication */
	return SevenSegmentTM1637_comAckLL(_pinClk, _pinDIO);
};

/**
 * Low level communication API
 * @param pinClk Clock Pin,
 * @param pinDIO DIO Pin,
 * @return returns true on success
 */
bool SevenSegmentTM1637_comAckLL(uint32_t pinClk, uint32_t pinDIO) {
	/* flag for acknowledge */
	bool acknowledged = false;

	/* set clock low */
	SevenSegment_pinClear(pinClk);
	/*reverse state of pin from output to input */
	SevenSegment_pinAsInputPullUp(pinDIO);
	/* delay for clk */
	SevenSegmentTM1637_delayMicroseconds(TM1637_CLK_DELAY_US);
	/*  Ack should pull the pin low again */
	acknowledged = SevenSegment_getPinState(pinDIO);
	/* set clk pin to high */
	SevenSegment_pinSet(pinClk);
	/* delay for clk */
	SevenSegmentTM1637_delayMicroseconds(TM1637_CLK_DELAY_US);
	/* set clk pin to low */
	SevenSegment_pinClear(pinClk);
	/* set dio pin to output*/
	SevenSegment_pinAsOutput(pinDIO);
	/* return ack status */
	return acknowledged;
}
