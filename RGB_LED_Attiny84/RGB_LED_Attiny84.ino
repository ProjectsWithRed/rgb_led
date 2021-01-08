#include <SoftwareSerial.h>

const int RX = PA0;
const int TX = PA1;
SoftwareSerial Serial1(RX, TX);


const int R_LED_PIN = PA5;
const int G_LED_PIN = PA6;
const int B_LED_PIN = PA7;

const int R_POT = PA4;
const int G_POT = PA3;
const int B_POT = PA2;

// Used to determine which method to use to control the RGB LED, the pots or the sliders.
const int POT_THRESH = 15;
const int APP_THRESH = 5;
int currMethod;
int lastPotsV;
int lastSlidersV;

// Will store latest values for the pots and app.
int potRV, potGV, potBV;
int appRV, appGV, appBV;



void setup() {
    Serial1.begin(19200);

    pinMode(R_LED_PIN, OUTPUT);
    pinMode(G_LED_PIN, OUTPUT);
    pinMode(B_LED_PIN, OUTPUT);
}



void loop() {

    potRV = analogRead(R_POT);
    potGV = analogRead(G_POT);
    potBV = analogRead(B_POT);
    
   
    if(Serial1.available()) {
        String raw = Serial1.readStringUntil('\n');

        // Extracting red, green, blue values coming from the ESP(app).         
        raw.trim();
        char* c = const_cast<char*>(raw.c_str());
        sscanf(c, "%d %d %d", &appRV, &appGV, &appBV);
    }

    // Determining which method to use by checking which method was used last. If the difference
    // between the current values and last values is larger then the thresholds, the used method is changed.
    int potsDiff = abs(lastPotsV - (potRV + potGV + potBV));
    int appDiff = abs(lastSlidersV - (appRV + appGV + appBV));

    if(potsDiff >= POT_THRESH && currMethod != 0) {
        currMethod = 0;  // 0 for pots
    } else if (appDiff >= APP_THRESH && currMethod != 1) {
        currMethod = 1;  // 1 for app sliders.
    }

    if(currMethod == 0) {
        analogWrite(R_LED_PIN, map(potRV, 0, 1023, 0, 255));
        analogWrite(G_LED_PIN, map(potGV, 0, 1023, 0, 255));
        analogWrite(B_LED_PIN, map(potBV, 0, 1023, 0, 255));
    } else {
        analogWrite(R_LED_PIN, appRV);
        analogWrite(G_LED_PIN, appGV);
        analogWrite(B_LED_PIN, appBV);
    }
    
    // Updating the last pot and slider values so the method check can happen for the next loop iteration.
    lastPotsV = potRV + potGV + potBV;
    lastSlidersV = appRV + appGV + appBV;

    delay(1);
}