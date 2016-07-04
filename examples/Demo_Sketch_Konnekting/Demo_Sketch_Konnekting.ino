//#include <avr/pgmspace.h>
#include <KnxDevice.h>

// ################################################
// ### DEBUG CONFIGURATION
// ################################################
#define DEBUG
#include <DebugUtil.h>

// Get right serial port for debugging
#ifdef __AVR_ATmega32U4__
    // Leonardo/Micro/ProMicro use the USB serial port
    #define DEBUGSERIAL Serial
#elif ESP8266
    // ESP8266 use the 2nd serial port with TX only
    #define DEBUGSERIAL Serial1
#else
    // All other, (ATmega328P f.i.) use software serial
    #include <SoftwareSerial.h>
    SoftwareSerial softserial(10, 11); // RX, TX
    #define DEBUGSERIAL softserial
#endif





// ################################################
// ### KONNEKTING Configuration
// ################################################
#ifdef __AVR_ATmega328P__
#define KNX_SERIAL Serial // Nano/ProMini etc. use Serial
//#elif __AVR_ATmega32U4__
//#define KNX_SERIAL Serial1 // Leonardo/Micro etc. use Serial1
//#elif ESP8266
//#define KNX_SERIAL Serial // ESP8266 use Serial
#endif

#define MANUFACTURER_ID 57005
#define DEVICE_ID 255
#define REVISION 0
#define PROG_LED_PIN 8
#define PROG_BUTTON_PIN 3



// ################################################
// ### KONNEKTING ComObjects and Parameters
// ################################################
KnxComObject KnxDevice::_comObjectsList[] = {
    Konnekting.createProgComObject(),
    /* Index 0 */ KnxComObject(KNX_DPT_1_001, COM_OBJ_LOGIC_IN),
    /* Index 1 */ KnxComObject(KNX_DPT_1_001, COM_OBJ_SENSOR),
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do no change this code

// Definition of parameter size
byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 */ PARAM_UINT16
};
const byte KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do no change this code





// ################################################
// ### Sketch Configuration & Variables
// ################################################
#define TEST_LED 4 //or change it to another pin

unsigned long diffmillis = 0;
unsigned long lastmillis = millis();
int laststate = false;



unsigned long blink;
bool blinkState = true;

int readEeprom(int index) {
    return EEPROM.read(index);
}

void writeEeprom(int index, int val) {
    return EEPROM.write(index, val);
}

void updateEeprom(int index, int val) {
    return EEPROM.update(index, val);
}

// ################################################
// ### SETUP
// ################################################
void setup() {

    
    pinMode(TEST_LED, OUTPUT);  
    pinMode(PROG_LED_PIN, OUTPUT);  
    
    // Start debug serial with 9600 bauds
    DEBUGSERIAL.begin(19200);

    // wait for serial port to connect. Needed for Leonardo/Micro/ProMicro only
#if defined(__AVR_ATmega32U4__)
    while (!DEBUGSERIAL)
#endif
        
    DEBUGSERIAL.println("STARTING\n");

    // make debug serial port known to debug class
    Debug.setPrintStream(&DEBUGSERIAL);
    Debug.print(F("Demo Sketch Konnekting\n"));

    Konnekting.setEepromReadFunc(&readEeprom);
    Konnekting.setEepromWriteFunc(&writeEeprom);
    Konnekting.setEepromUpdateFunc(&updateEeprom);
    
//    // Initialize KNX enabled Arduino Board
    Konnekting.init(KNX_SERIAL,
            PROG_BUTTON_PIN,
            PROG_LED_PIN,
            MANUFACTURER_ID,
            DEVICE_ID,
            REVISION);

    diffmillis = (int) Konnekting.getUINT16Param(0); //blink every xxxx ms

    Debug.println(F("Toggle every %d ms."), diffmillis);
    Debug.println(F("Setup is ready. go to loop..."));
    blink = millis();
}

// ################################################
// ### LOOP
// ################################################
void loop() {
    Knx.task();
    unsigned long currentmillis = millis();
    // only do measurements and other sketch related stuff if not in programming mode
//    if (Konnekting.isReadyForApplication()) {
//        if (currentmillis - lastmillis >= diffmillis) {
//            Debug.println(F("Actual state: %d"), laststate);
//            Knx.write(2, laststate);
//            laststate = !laststate;
//            lastmillis = currentmillis;
//            
//            digitalWrite(TEST_LED, HIGH);
//
//        }
//    }
    if (currentmillis-blink > 1000) {
        DEBUGSERIAL.print("Toggle Blink: ");
        DEBUGSERIAL.println(blinkState);
        digitalWrite(TEST_LED, blinkState);
        blinkState = !blinkState;
        blink = millis();
    }
}

// ################################################
// ### KNX EVENT CALLBACK
// ################################################

void knxEvents(byte index) {
//    // nothing to do in this sketch
//    switch (index) {
//
//        case 0: // object index 1 has been updated
//
//            if (Knx.read(1)) {
//                digitalWrite(TEST_LED, HIGH);
//                Debug.println(F("Toggle LED: on"));
//            } else {
//                digitalWrite(TEST_LED, LOW);
//                Debug.println(F("Toggle LED: off"));
//            }
//            break;
//
//        default:
//            break;
//    }
};

