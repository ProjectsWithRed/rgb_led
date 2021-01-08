#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


const char auth[] = "ocQ8G2wg5HpWFQMY1AjIj67VerpcMb7A";
const char ssid[] = "Albatat";
const char pass[] = "Albatat123";

// To store the red, green, blue slider values coming from the Blynk app.
int redValue, greenValue, blueValue;

String spacer = " ";  // Used to seperate the slider values.


void setup() {
    Serial.begin(19200);
    Blynk.begin(auth, ssid, pass);
}


// Getting the slider values from Blynk app.
BLYNK_WRITE(V0) {
    redValue = param.asInt();
}
BLYNK_WRITE(V1) {
    greenValue = param.asInt();
}
BLYNK_WRITE(V2) {
    blueValue = param.asInt();
}



void loop() {
    Blynk.run();

    // Combining all slider values into one big string to send to the Attiny at once.
    String rgbData = redValue + spacer + greenValue + spacer + blueValue;
    Serial.println(rgbData);

    delay(10);  // Added to make sure the Attiny can keep up and keep everything stable.
}