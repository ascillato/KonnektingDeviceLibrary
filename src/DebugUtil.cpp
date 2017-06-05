#include "DebugUtil.h"

// DebugUtil unique instance creation
DebugUtil DebugUtil::Debug;
DebugUtil& Debug = DebugUtil::Debug;

DebugUtil::DebugUtil() {
}

void DebugUtil::setPrintStream(Stream* printstream) {
    _printstream = printstream;
    print(F("DEBUG! free ram: %d\n"), freeRam());
}

int DebugUtil::freeRam() {
#if defined(ESP8266) || defined(ESP32)
    return ESP.getFreeHeap();
#elif defined(__AVR_ATmega328P__) || (__AVR_ATmega32U4__)
#elif __AVR_ATmega328P__ || __AVR_ATmega32U4__
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#else
    return -1;
#endif
}

void DebugUtil::print(char *format, ...) {
    if (_printstream) {
        char buf[128]; // limit to 128chars
        va_list args;
        va_start(args, format);
        vsnprintf(buf, 128, format, args);
        va_end(args);
        //Serial.print(buf);)
        _printstream->print(buf);
    }

}

void DebugUtil::print(const __FlashStringHelper *format, ...) {
    if (_printstream) {
        char buf[128]; // limit to 128chars
        va_list args;
        va_start(args, format);

#if defined(__AVR__) || defined(ESP8266)   
        vsnprintf_P(buf, sizeof (buf), (const char *) format, args); // progmem for AVR
#else
        vsnprintf(buf, sizeof (buf), (const char *) format, args); // for rest of the world
#endif    
        va_end(args);
        //Serial.print(buf);)    
        _printstream->print(buf);
    }
}

void DebugUtil::println(char *format, ...) {
    if (_printstream) {

        char buf[128]; // limit to 128chars
        va_list args;
        va_start(args, format);
        vsnprintf(buf, 128, format, args);
        va_end(args);
        //Serial.println(buf);)
        _printstream->println(buf);
    }
}

void DebugUtil::println(const __FlashStringHelper *format, ...) {
    if (_printstream) {

        char buf[128]; // limit to 128chars
        va_list args;
        va_start(args, format);

#if defined(__AVR__) || defined(ESP8266)   
        vsnprintf_P(buf, sizeof (buf), (const char *) format, args); // progmem for AVR
#else
        vsnprintf(buf, sizeof (buf), (const char *) format, args); // for rest of the world
#endif    

        va_end(args);
        //Serial.println(buf);)    
        _printstream->println(buf);
    }
}




