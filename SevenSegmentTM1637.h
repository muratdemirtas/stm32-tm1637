
#ifndef SevenSegmentTM1637_H
#define SevenSegmentTM1637_H


#include "SevenSegmentAsciiMap.h"
#include "stm32f0xx_hal.h"

#define TM1637_COLON_BIT       0x80

#define TM1637_DEBUG_BEGIN(x)
#define TM1637_DEBUG_PRINT(...)
#define TM1637_DEBUG_PRINTLN(...)
#define TM1637_DEBUG_WRITE(x)
#define TM1637_DEBUG_MESSAGE(x)
#define TM1637_DEBUG_MESSAGELN(x)

#define pinAsOutput(P) void
#define pinAsInput(P) void
#define pinAsInputPullUp(P) void
#define digitalLow(P) HAL_GPIO_WritePin(GPIOC, P, 0)
#define digitalHigh(P) HAL_GPIO_WritePin(GPIOC, P, 1)
#define isHigh(P) (HAL_GPIO_ReadPin(GPIOC, P) == 1)
#define isLow(P) (HAL_GPIO_ReadPin(GPIOC, P) == 0)
#define digitalState(P) HAL_GPIO_ReadPin(GPIOC, P)

// COMPILE TIME USER CONFIG ////////////////////////////////////////////////////
#define TM1637_DEBUG                  false   // true for serial debugging
#define TM1637_BEGIN_DELAY            500     // ms
#define TM1637_PRINT_BUFFER_SIZE      128     // lower if you don't need it

// Default values //////////////////////////////////////////////////////////////
#define TM1637_DEFAULT_PRINT_DELAY    300     // 300 ms delay between characters
#define TM1637_DEFAULT_BLINK_DELAY    50      // ms
#define TM1637_DEFAULT_CLOCK_BLINK_DELAY 500  // the default delay for when using printTime
#define TM1637_DEFAULT_BLINK_REPEAT   10
#define TM1637_DEFAULT_CURSOR_POS     0       // 0-MAX-1 (e.g 3)
#define TM1637_DEFAULT_COLON_ON       false   //
#define TM1637_DEFAULT_BACKLIGHT      100     // 0..100

#define TM1637_MAX_LINES    1                 // number of display lines
#define TM1637_MAX_COLOM    4                 // number of coloms (digits)
#define TM1637_MAX_CHARS    128

// PROGRAM CONFIG (ONLY CHANGE WHEN YOU KNOW WHAT YOU RE DOING:)////////////////
#define TM1637_CLK_DELAY_US 2           // clock delay for communication
// mine works with 1us, perhaps increase if display does not function ( tested upto 1ms)


// COMMANDS ////////////////////////////////////////////////////////////////////
#define TM1637_COM_SET_DATA     0x40 // 0x40 (1) Data set
#define TM1637_COM_SET_ADR      0xC0 // 0xC0 (2) Address command set
#define TM1637_COM_SET_DISPLAY  0x80 // 0x80 (3) Display control command set

// Data set (1) (use logical OR to contruct complete command)
#define TM1637_SET_DATA_WRITE   0x0 // Write data to the display register
#define TM1637_SET_DATA_READ    0x2 // Read the key scan data
#define TM1637_SET_DATA_A_ADDR  0x0 // Automatic address increment
#define TM1637_SET_DATA_F_ADDR  0x4 // Fixed address
#define TM1637_SET_DATA_M_NORM  0x0 // Normal mode
#define TM1637_SET_DATA_M_TEST  0x20 // Test mode

// Address settings (2) (use logical OR to contruct complete command)
#define TM1637_SET_ADR_00H      0x00  // addr 00
#define TM1637_SET_ADR_01H      0x01  // addr 01
#define TM1637_SET_ADR_02H      0x02  // addr 02
#define TM1637_SET_ADR_03H      0x03  // addr 03
#define TM1637_SET_ADR_04H      0x04  // addr 04 (only TM1637)
#define TM1637_SET_ADR_05H      0x05  // addr 05 (only TM1637)
// The command is used to set the display register address; if the address is set to 0C4H or higher, the data is ignored, until the effective address is set; when the power is on, the default is set to 00H address.

// Display control command set (use logical OR to consruct complete command)
#define TM1637_SET_DISPLAY_1    0x00  // Pulse width 1/16 (0.0625) (0)
#define TM1637_SET_DISPLAY_2    0x01  // Pulse width 2/16 (0.0625) (1)
#define TM1637_SET_DISPLAY_4    0x02  // Pulse width 4/16 (0.0625) (2)
#define TM1637_SET_DISPLAY_10   0x03  // Pulse width 10/16 (0.0625) (3)
#define TM1637_SET_DISPLAY_11   0x04  // Pulse width 11/16 (0.0625) (4)
#define TM1637_SET_DISPLAY_12   0x05  // Pulse width 12/16 (0.0625) (5)
#define TM1637_SET_DISPLAY_13   0x06  // Pulse width 13/16 (0.0625) (6)
#define TM1637_SET_DISPLAY_14   0x07  // Pulse width 14/16 (0.0625) (7)
#define TM1637_SET_DISPLAY_OFF  0x00  // OFF
#define TM1637_SET_DISPLAY_ON   0x08  // ON
// there are a total of 8 brighness values, plus off

// PROTOCOL SPECIFICATION
/*
* Structure
* START COMMAND ACK STOP                  set config or display
* START ADR_CMD DATA ACK STOP             sets single digit
* START ADR_CMD DATA0 .. DATAN ACK STOP   sets multiple digits when in auto mode
*
* There are basicly three things you can do:
*   1. Set some configuration values
*     - read/write mode, auto/manual address, normal/test mode
*   2. Set a (starting) address followed by 1 or N data bytes
*   3. Set the display brightness (pwm) 0-7 and on or off
*
* From the datasheet it might seem that you always have to perform all three commands; setting configuration, setting address and data bytes and display. I'v tested this and this is not true. You can just set only one of these three. But ofcourse you have to make sure that your configuration is set properly. For example if you haven't set the configuration to automatic addresses, you can't just send out 4 data bytes, it won't work. Simlilair, if your display is off and you write some data to it, it won't display. On the other hand most default setting are what you want most of the time.

*/


  // LIQUID CRISTAL API ///////////////////////////////////////////////////////
  // See http://playground.arduino.cc/Code/LCDAPI for more details.

  /* Constructor
  @param [in] pinClk      clock pin (any digital pin)
  @param [in] pinDIO      digital output pin (any digital pin)
  */

  /* Initializes the display
  * Initializes the display, sets some text and blinks the display

  @param [in] cols      optional: number of coloms (digits)
  @param [in] rows      optional: number of rows
  */
  void    SevenSegmentTM1637_init(uint8_t cols, uint8_t rows  );
  /* Implemented for compatibility, see begin() above */
  void    SevenSegmentTM1637_begin(uint8_t cols , uint8_t rows );
  // Print class inheritance ///////////////////////////////////////////////////
  // See https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/Print.h for more details
  /* This library inherent the Print class, this means that all regular print function can be used. For example:
  * printing a number:                    print(78)
  * printint a number in BIN, OCT, HEX..: print(78, BIN)
  * printing a float:                     print(2.85)
  * printing a PROGMEM c string:          print(F("Arduino"))
  *
  * Also the more low level write() function can be used. (Actually all print function eventually call one of these write methods, every class that wants to inherent from the Print class needs to implement these)
  */
  size_t SevenSegmentTM1637_writeByte(uint8_t byte);
  size_t SevenSegmentTM1637_writeStr(const char* str);
  size_t SevenSegmentTM1637_writeBuffer(const uint8_t* buffer, size_t size);

  /* Clears the display
  * Writes zero to all digits and segments, display off.
  */
  void    SevenSegmentTM1637_clear(void);
  /* Sets the cursor position to zero
  */
  void    SevenSegmentTM1637_home(void);
  /* Sets the cursor position to a specfic position
  *
  @param [in] col         colomn (position)
  */
  void    SevenSegmentTM1637_setCursor(uint8_t row, uint8_t col);
  // Liquid cristal optional //////////////////////////////////////////////////
  /* Sets the display backlight
  * The display has 8 PWM modes and an off mode. The function accepts a value from 0 to 100, where 80-100 are the same; full brighness.

  @param [in] value       brightness value (0..80(100))
  */
  void    SevenSegmentTM1637_setBacklight(uint8_t value);
  /* Sets the display contrast (identical to brightness)
  * This function is mainly for compatibility with the LCD API
  */
  void    SevenSegmentTM1637_setContrast(uint8_t value);

  /* Turns the display ON
  * Identical to setting the brightness to the default value.
  */
  void    SevenSegmentTM1637_on(void);
  /* Turns the display ON
  * Identical to setting the brightness to zero and clearing the display.
  */
  void    SevenSegmentTM1637_off(void);

  // SevenSegmentTM1637 METHODS ///////////////////////////////////////////////
  /* Blink the last printed text
  *
  @param [in] blinkDelay    optional: blink delay in ms
  @param [in] repeats       optional: number of blink repeats
  */
  void    SevenSegmentTM1637_blink(uint8_t blinkDelay, uint8_t repeats ,
		  	  	  	  	  	  	  	  	  uint8_t maxBacklight , uint8_t minBacklight );

  // getters and setters ///////////////////////////////////////////////////////
  /* Turn the colon on or off
  * When turing the colon on, the next displayed text/numbers will have a colon
  @param [in] setToOn       sets the colon to on or off
  */
  void    SevenSegmentTM1637_setColonOn(bool setToOn);
  /* Get the currrent colon setting
  */
  bool    SevenSegmentTM1637_getColonOn(void);
  /* Sets the delay for scrolling text
  * When printing more than four characters/ the display will scroll, this setting determines the scrolling speed in ms
  @param [in] printDelay    the print delay in ms
  */
  void    SevenSegmentTM1637_setPrintDelay(uint16_t printDelay);

  // helpers //////////////////////////////////////////////////////////////////
  /* Encodes a character to sevensegment binairy
  *
  @param [in] c             a character to encode
  */
  uint8_t SevenSegmentTM1637_encodeChar(char c);
  /* Encodes a single digit to sevensegment binairy
  *
  @param [in] d             a digit to encode
  */
  uint8_t SevenSegmentTM1637_encodeInteger(int16_t d);
  /* Encodes a null terminated c string (char array) to sevensegment binairy
  *
  @param [out] buffer       holds the encodes char array
  @param [in] str           the c string to encode
  @param [in] bufferSize    the size/length of the buffer
  */
  size_t  SevenSegmentTM1637_encodeString(uint8_t* buffer, const char* str, size_t bufferSize);
  /* Encodes a byte array to sevensegment binairy
  *
  @param [out] buffer       holds the encodes char array
  @param [in] byteArr       the byte array to encode
  @param [in] bufferSize    the size/length of the buffer
  */
  size_t  SevenSegmentTM1637_encodeBuffer(uint8_t* buffer, const uint8_t* byteArr, size_t arrSize);
  /* Shift an array one position to the left
  @param [out] buffer       the buffer to be shifted
  @param [in] length        the length to the buffer
  */
  void    SevenSegmentTM1637_shiftLeft(uint8_t* buffer, size_t length);

  // SevenSegmentTM1637 low level methods (use when you know what you're doing)
  /* Prints raw (encoded) bytes to the display
  *         A
  *       ___
  *  * F |   | B
  * X     -G-
  *  * E |   | C
  *       ___
  *        D
  * Bit:      76543210
  * Segment:  XGFEDCBA
  *
  * For example to print an H, you would set bits BCEFG, this gives B01110110 in binary or 118 in decimal or 0x76 in HEX.
  * Bit 7 (X) only applies to the second digit and sets the colon
  */
  /* Print raw (binary encodes) bytes to the display
  @param [in] rawBytes      Array of raw bytes
  @param [in] length        optional: length to print to display
  @param [in] position      optional: Start position
  */
  void    SevenSegmentTM1637_printRawBytes(const uint8_t* rawBytes, size_t length , uint8_t position );
  /* Print raw (binary encodes) bytes to the display
  @param [in] rawByte       Raw byte
  @param [in] position      optional: Start position
  */
  void    SevenSegmentTM1637_printRawByte(uint8_t rawByte, uint8_t position);
  /* Write command to IC TM1637
  @param [in] cmd         command to send
  @return acknowledged?   command was (successful) acknowledged
  */
  bool    SevenSegmentTM1637_commandSingle(uint8_t cmd)  ;
  extern bool    SevenSegmentTM1637_commandMulti(const uint8_t* command, uint8_t length) ;





#endif
