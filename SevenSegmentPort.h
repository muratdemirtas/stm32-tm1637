/*
 * SevenSegmentPort.h
 *
 *  Created on: Sep 25, 2022
 *      Author: murat
 */


#include "SevenSegmentTM1637.h"
void SevenSegment_pinSet(uint32_t pin);
void SevenSegment_pinClear(uint32_t pin);
bool SevenSegment_getPinState(uint32_t pin);
void SevenSegment_pinAsOutput(uint32_t pin);
void SevenSegment_pinAsInputPullUp(uint32_t pin);
void SevenSegmentTM1637_delayMicroseconds(unsigned int i);
