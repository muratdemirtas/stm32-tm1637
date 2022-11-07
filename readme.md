
SevenSegmentTM1637(GPIO_PIN_5, GPIO_PIN_4);
SevenSegmentTM1637_begin(TM1637_MAX_COLOM, TM1637_MAX_LINES); 
SevenSegmentTM1637_clear();
SevenSegmentTM1637_setBacklight(100);
SevenSegmentTM1637_writeBuffer("sc0", 3 );
