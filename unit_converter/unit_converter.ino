//ITEMS FOR TFT DISPLAY
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Instantiate library


// //ITEMS FOR THE ROTARY ENCODER
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"

#define ROTARY_ENCODER_A_PIN 22
#define ROTARY_ENCODER_B_PIN 35
#define ROTARY_ENCODER_BUTTON_PIN 1
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);


//SETUP FOR THE ON BOARD LED
int freq = 2000;    // frequency
int channel = 0;    // aisle
int resolution = 8;   // Resolution
const int led = 4;

// //LIST OF DIFFERENT MENU ITEMS
// enum MenuState { MAIN_MENU, KW, KVA, W, AMPS, FAULT, CABLE_TRAY, CABLE_DUCT };
// MenuState currentMenu = MAIN_MENU;

// Variable to store button press count
int buttonPressCount = 1;

//CHECK TO SEE IF THE DISPLAY HAS CHANGED
String currentLayout = "A";

//DEFINE THE TEXT FOR THE DISPLAY WITH DEFAULT VALUES
int Pos_1 = 0;
String Pos_1_display = "POS_1";
String Pos_2 = "POS_2";
String Pos_3 = "POS_3";
String Pos_4 = "POS_4";
String Pos_3_1 = "POS_3_1";
String Pos_3_2 = "POS_3_2";
String Pos_3_3 = "POS_3_3";
String Pos_3_4 = "POS_3_4";
String Pos_3_5 = "POS_3_5";
String Pos_3_6 = "POS_3_6";
String Pos_3_7 = "POS_3_7";
String Pos_4_1 = "POS_4_1";
String Pos_4_2 = "POS_4_2";
String Pos_4_3 = "POS_4_3";
String Pos_4_4 = "POS_4_4";
String Pos_4_5 = "POS_4_5";
String Pos_4_6 = "POS_4_6";
String Pos_4_7 = "POS_4_7";

int paddingValue = 22;

//GLOBAL VARIABLES FOR CHECKING IF THE ROTARY ENCODER HAS STOPPED
unsigned long lastEncoderChange = 0;
bool encoderStopped = false;


//SETUP FOR THE ROTARY ENCODER
void rotary_onButtonClick()
{
    static unsigned long lastTimePressed = 0; // Soft debouncing
    if (millis() - lastTimePressed < 500)
    {
            return;
    }
    lastTimePressed = millis();

    // Increment the counter and wrap around if it exceeds 7
    buttonPressCount++;
    if (buttonPressCount > 7) {
        buttonPressCount = 1;
    }
    tft.fillScreen(TFT_BLACK);    // Clear the screen with a black background
}

void rotary_loop() {
    if (rotaryEncoder.encoderChanged()) {
        lastEncoderChange = millis(); // Update the last change timestamp
        encoderStopped = false; // Reset the stopped flag
        // Serial.print("Value: ");
        // Serial.println(rotaryEncoder.readEncoder());
        Pos_1 = rotaryEncoder.readEncoder();
        int maxLength = 5;
        Pos_1_display = padString(String(rotaryEncoder.readEncoder()), maxLength);


    } else {
        if (!encoderStopped && millis() - lastEncoderChange > 500) {
            // More than 1 second has passed since the last change
            encoderStopped = true;
            // Serial.println("Encoder stopped: ");
            // Serial.println(millis());
        }
    }

    if (rotaryEncoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
    }
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}


void setup(void) {
  pinMode(3, OUTPUT);    // Set pin 3 as high - this is being used as positive voltage for rotary encoder. 
  digitalWrite(3, HIGH);
  
  Serial.begin(115200); // Begin serial communication at 115200 baud rate
  tft.init();           // Initialize the display
  tft.setRotation(3);   // Set the display orientation

  //SETUP FOR THE SCREEN
  tft.fillScreen(TFT_BLACK);    // Clear the screen with a black background
  // Set text color and background
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  // Set the text datum to top left
  tft.setTextDatum(TL_DATUM);

  //Initialize GPIO, turn off tricolor light
  pinMode(4, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(4, 0);
  digitalWrite(16, 0);
  digitalWrite(17, 0);
  ledcSetup(channel, freq, resolution); // set channel
  //KEY
  pinMode(0,INPUT_PULLUP);

  //we must initialize rotary encoder
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  //set boundaries and if values should cycle or not
  //in this example we will set possible values between 0 and 1000;
  bool circleValues = false;
  rotaryEncoder.setBoundaries(0, 10000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

  /*Rotary acceleration introduced 25.2.2021.
  * in case range to select is huge, for example - select a value between 0 and 1000 and we want 785
  * without accelerateion you need long time to get to that number
  * Using acceleration, faster you turn, faster will the value raise.
  * For fine tuning slow down.
  */
  //rotaryEncoder.disableAcceleration(); //acceleration is now enabled by default - disable if you dont need it
  rotaryEncoder.setAcceleration(500); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration

}


void loop() {
  //THIS FUNCTION WILL  BE THE MAIN MENU - IT WILL ALLOW YOU TO CYCLE BETWEEN FUNCTIONS
  //display_values_LAYOUT_A();
  //delay(1);
  //change_LED_colour();
  rotary_loop();
  
  
  if (encoderStopped == true) {
    //CALCULATE THE VALUES BASED ON MENU ITEM
    if (buttonPressCount == 1) {
        display_KW(); // Run the display kW program
    } else if (buttonPressCount == 2) {
        display_KVA(); // Run the display kVA program
    } else if (buttonPressCount == 3) {
        display_W(); // Run the display Watt program
    } else if (buttonPressCount == 4) {
        display_AMPS(); // Run the display Amperes program
    } else if (buttonPressCount == 5) {
        display_FAULT(); // Run the display Fault Detection program
    } else if (buttonPressCount == 6) {
        display_CABLE_TRAY(); // Run the display Cable Tray program
    } else if (buttonPressCount == 7) {
        display_CABLE_DUCT(); // Run the display Cable Duct program
    }


    display_values_LAYOUT_A();
    
  }
  else {
    Serial.println("Not Stopped - Passing");
    display_values_LAYOUT_B();
  }
  
}





void display_values_LAYOUT_A() {
//BLANK THE SCREEN IF THE LAYOUT HAS CHANGED
if (currentLayout != "A") {
  tft.fillScreen(TFT_BLACK);    // Clear the screen with a black background
  currentLayout = "A";
}

// Draw a horizontal line
  int x = 0;                      // Starting x-coordinate
  int y = 60;       // Starting y-coordinate 
  int lineLength = tft.width();   // Line length, full width of the display
  uint32_t color = TFT_WHITE;     // Line color, white
  tft.drawFastHLine(x, y, lineLength, color); // Draw the horizontal line
  // Draw a horizontal line
  x = 0;                      // Starting x-coordinate
  y = 95;       // Starting y-coordinate
  tft.drawFastHLine(x, y, lineLength, color); // Draw the horizontal line
  // Draw a vertical line
  x = tft.width() / 2;      // Starting x-coordinate, middle of the screen
  y = 0;                    // Starting y-coordinate
  int lineLength_v = tft.height(); // Line length, full height of the display
  tft.drawFastVLine(x, y, lineLength_v, color); // Draw the vertical line

  //THE MAIN FUNCTIONS WILL DISPLAY TEXT IN THE SAME PLACE - TO PREVENT HAVING TO DRAW CUSTOM ONES IT WILL JUST BE SENT VALUES FOR EACH ITEM
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); 
  tft.drawString(Pos_1_display, 0, 5, 6);  // '2' is the font size, you can adjust as necessary
  tft.setTextColor(TFT_GREEN, TFT_BLACK); 
  tft.drawString(Pos_2, 180, 15, 4);  // '2' is the font size, you can adjust as necessary

  //DRAW THE SUB MENU TITLES
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.drawString(Pos_3, 0, 65, 4);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4, 170, 65, 4);  // '2' is the font size, you can adjust as necessary

  //DRAW THE REMAINING ITEMS IN THE LAST COLUMNS
  tft.setTextColor(TFT_WHITE, TFT_BLACK); 
  tft.drawString(Pos_3_1, 0, 100, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_3_2, 0, 120, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_3_3, 0, 140, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_3_4, 0, 160, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_3_5, 0, 180, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_3_6, 0, 200, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_3_7, 0, 220, 2);  // '2' is the font size, you can adjust as necessary

  tft.drawString(Pos_4_1, 170, 100, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4_2, 170, 120, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4_3, 170, 140, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4_4, 170, 160, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4_5, 170, 180, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4_6, 170, 200, 2);  // '2' is the font size, you can adjust as necessary
  tft.drawString(Pos_4_7, 170, 220, 2);  // '2' is the font size, you can adjust as necessary

}

void display_values_LAYOUT_B() {
  //BLANK THE SCREEN IF THE LAYOUT HAS CHANGED
  if (currentLayout != "B") {
    tft.fillScreen(TFT_BLACK);    // Clear the screen with a black background
    currentLayout = "B";
  }  
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); 
  tft.drawString(Pos_1_display, 50, 80, 8);  // '2' is the font size, you can adjust as necessary 

}


String padString(String input, int maxLength) {
  if (input.length() > maxLength) {
    // Truncate the string if it's longer than the maximum length
    return input.substring(0, maxLength);
  }

  // Pad the string with spaces if it's shorter than the maximum length
  while (input.length() < maxLength) {
    input += " ";
  }

  return input;
}


void change_LED_colour() {
    if(digitalRead(0))
  {
    while(digitalRead(0));
    digitalWrite(4,!digitalRead(4));
    digitalWrite(16,!digitalRead(16));
    digitalWrite(17,!digitalRead(17));
  }
  
  // digitalWrite(4, 0);
  // digitalWrite(16, 1);
  // digitalWrite(17, 1);
  // delay(500);
  // digitalWrite(4, 1);
  // digitalWrite(16, 0);
  // digitalWrite(17, 1);
  // delay(500);
  // digitalWrite(4, 1);
  // digitalWrite(16, 1);
  // digitalWrite(17, 0);
  // delay(500);
  // digitalWrite(4, 1);
  // digitalWrite(16, 1);
  // digitalWrite(17, 1);
  // delay(500);
}

//THIS IS WHERE THE CALCULATIONS FOR EACH OF THE FUNCTIONS HAPPENS AND IT UPDATES THE GLOBAL DISPLAY VALUES
void display_KW() {
  // Assuming Pos_1 is an integer, convert Pos_1 to float for precise calculations
  float Pos_1_float = float(Pos_1);

  // FUNCTION FOR CONVERTING KW INTO KVA AND AMPS
  Pos_2 = padString("Kilowatts",paddingValue);
  Pos_3 = padString("Amps",paddingValue);
  Pos_4 = padString("kVA",paddingValue);
  Pos_3_1 = padString("230V : " + String((Pos_1_float / 230.0) * 1000.0), paddingValue);
  Pos_3_2 = padString("400V : " + String(Pos_1_float / (1.73 * 400.0) * 1000.0), paddingValue);
  Pos_3_3 = padString("6.6kV : " + String(Pos_1_float / (1.73 * 6600.0) * 1000.0), paddingValue);
  Pos_3_4 = padString("11kV : " + String(Pos_1_float / (1.73 * 11000.0) * 1000.0), paddingValue);
  Pos_3_5 = padString("33kV : " + String(Pos_1_float / (1.73 * 33000.0) * 1000.0), paddingValue);
  Pos_3_6 = padString("       ", paddingValue);  // Assuming you want to pad spaces as well
  Pos_3_7 = padString("       ", paddingValue);  // Assuming you want to pad spaces as well
  Pos_4_1 = padString("0.95pf : " + String(Pos_1_float / 0.95), paddingValue);
  Pos_4_2 = padString("0.9pf : " + String(Pos_1_float / 0.9), paddingValue);
  Pos_4_3 = padString("0.85pf : " + String(Pos_1_float / 0.85), paddingValue);
  Pos_4_4 = padString("0.8pf : " + String(Pos_1_float / 0.8), paddingValue);
  Pos_4_5 = padString("0.7pf : " + String(Pos_1_float / 0.7), paddingValue);
  Pos_4_6 = padString("0.6pf : " + String(Pos_1_float / 0.6), paddingValue);
  Pos_4_7 = padString("0.5pf : " + String(Pos_1_float / 0.5), paddingValue);
}

void display_KVA() {
  // Convert Pos_1 to float for precise calculations
  float Pos_1_float = float(Pos_1);

  Pos_2 = padString("kVA",paddingValue);
  Pos_3 = padString("Amps",paddingValue);
  Pos_4 = padString("Kilowatts",paddingValue);
  Pos_3_1 = padString("230V : " + String((Pos_1_float / 230) * 1000), paddingValue);
  Pos_3_2 = padString("400V : " + String(Pos_1_float / (1.73 * 400) * 1000), paddingValue);
  Pos_3_3 = padString("6.6kV : " + String(Pos_1_float / (1.73 * 6600) * 1000), paddingValue);
  Pos_3_4 = padString("11kV : " + String(Pos_1_float / (1.73 * 11000) * 1000), paddingValue);
  Pos_3_5 = padString("33kV : " + String(Pos_1_float / (1.73 * 33000) * 1000), paddingValue);
  Pos_3_6 = padString("       ", paddingValue);
  Pos_3_7 = padString("       ", paddingValue);
  Pos_4_1 = padString("0.95pf : " + String(Pos_1_float * 0.95), paddingValue);
  Pos_4_2 = padString("0.9pf : " + String(Pos_1_float * 0.9), paddingValue);
  Pos_4_3 = padString("0.85pf : " + String(Pos_1_float * 0.85), paddingValue);
  Pos_4_4 = padString("0.8pf : " + String(Pos_1_float * 0.8), paddingValue);
  Pos_4_5 = padString("0.7pf : " + String(Pos_1_float * 0.7), paddingValue);
  Pos_4_6 = padString("0.6pf : " + String(Pos_1_float * 0.6), paddingValue);
  Pos_4_7 = padString("0.5pf : " + String(Pos_1_float * 0.5), paddingValue);
}


void display_W() {
  // Convert Pos_1 to float for precise calculations
  float Pos_1_float = float(Pos_1);

  Pos_2 = padString("Watts",paddingValue);
  Pos_3 = padString("Amps",paddingValue);
  Pos_4 = padString("VA",paddingValue);
  Pos_3_1 = padString("230V : " + String(Pos_1_float / 230), paddingValue);
  Pos_3_2 = padString("400V : " + String(Pos_1_float / (1.73 * 400)), paddingValue);
  Pos_3_3 = padString("6.6kV : " + String(Pos_1_float / (1.73 * 6600)), paddingValue);
  Pos_3_4 = padString("11kV : " + String(Pos_1_float / (1.73 * 11000)), paddingValue);
  Pos_3_5 = padString("33kV : " + String(Pos_1_float / (1.73 * 33000)), paddingValue);
  Pos_3_6 = padString("       ", paddingValue);
  Pos_3_7 = padString("       ", paddingValue);
  Pos_4_1 = padString("0.95pf : " + String(Pos_1_float / 0.95), paddingValue);
  Pos_4_2 = padString("0.9pf : " + String(Pos_1_float / 0.9), paddingValue);
  Pos_4_3 = padString("0.85pf : " + String(Pos_1_float / 0.85), paddingValue);
  Pos_4_4 = padString("0.8pf : " + String(Pos_1_float / 0.8), paddingValue);
  Pos_4_5 = padString("0.7pf : " + String(Pos_1_float / 0.7), paddingValue);
  Pos_4_6 = padString("0.6pf : " + String(Pos_1_float / 0.6), paddingValue);
  Pos_4_7 = padString("0.5pf : " + String(Pos_1_float / 0.5), paddingValue);
}


void display_AMPS() {
  // Convert Pos_1 to float for precise calculations
  float Pos_1_float = float(Pos_1);

  Pos_2 = padString("Amps",paddingValue);
  Pos_3 = padString("kW",paddingValue);
  Pos_4 = padString("kVA",paddingValue);
  Pos_3_1 = padString("230V : " + String(Pos_1_float * 230/1000), paddingValue);
  Pos_3_2 = padString("400V : " + String(Pos_1_float * ((1.73 * 400)/1000)), paddingValue);
  Pos_3_3 = padString("6.6kV : " + String(Pos_1_float * ((1.73 * 6600)/1000)), paddingValue);
  Pos_3_4 = padString("11kV : " + String(Pos_1_float * ((1.73 * 11000)/1000)), paddingValue);
  Pos_3_5 = padString("33kV : " + String(Pos_1_float * ((1.73 * 33000)/1000)), paddingValue);
  Pos_3_6 = padString("       ", paddingValue);
  Pos_3_7 = padString("       ", paddingValue);
  Pos_4_1 = padString("230V @ 0.9 : " + String((Pos_1_float * 230)/0.9), paddingValue);
  Pos_4_2 = padString("400V @ 0.9 : " + String((Pos_1_float * (1.73 * 400)/1000)/0.9), paddingValue);
  Pos_4_3 = padString("400V @ 0.85 : " + String((Pos_1_float * (1.73 * 400)/1000)/0.85), paddingValue);
  Pos_4_4 = padString("400V @ 0.8 : " + String((Pos_1_float * (1.73 * 400)/1000)/0.8), paddingValue);
  Pos_4_5 = padString("6.6kV @ 0.9 : " + String((Pos_1_float * (1.73 * 6600)/1000)/0.9), paddingValue);
  Pos_4_6 = padString("11kV @ 0.9 : " + String((Pos_1_float * (1.73 * 11000)/1000)/0.9), paddingValue);
  Pos_4_7 = padString("33kV @ 0.9 : " + String((Pos_1_float * (1.73 * 33000)/1000)/0.9), paddingValue);
}


void display_FAULT() {
  // Convert Pos_1 to float for precise calculations
  float Pos_1_float = float(Pos_1);
  float full_load_current = (Pos_1_float / (1.73 * 400) * 1000);

  Pos_2 = padString("kVA",paddingValue);
  Pos_3 = padString("Amps",paddingValue);
  Pos_4 = padString("Fault Current",paddingValue);
  Pos_3_1 = padString("Full Load Current : ", paddingValue);
  Pos_3_2 = padString("400V : " + String(full_load_current) + "A", paddingValue);
  Pos_3_3 = padString("       ", paddingValue);
  Pos_3_4 = padString("       ", paddingValue);
  Pos_3_5 = padString("       ", paddingValue);
  Pos_3_6 = padString("       ", paddingValue);
  Pos_3_7 = padString("       ", paddingValue);
  Pos_4_1 = padString("4.75% : " + String((full_load_current / 0.0475)/1000) + "kA", paddingValue);
  Pos_4_2 = padString("5% : " + String((full_load_current / 0.05)/1000) + "kA", paddingValue);
  Pos_4_3 = padString("5.5% : " + String((full_load_current / 0.055)/1000) + "kA", paddingValue);
  Pos_4_4 = padString("6% : " + String((full_load_current / 0.06)/1000) + "kA", paddingValue);
  Pos_4_5 = padString("6.5% : " + String((full_load_current / 0.065)/1000) + "kA", paddingValue);
  Pos_4_6 = padString("       ", paddingValue);
  Pos_4_7 = padString("       ", paddingValue);
}


void display_CABLE_TRAY() {
  //FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING

}

void display_CABLE_DUCT() {
  ////FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING

}
