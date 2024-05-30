//ITEMS FOR TFT DISPLAY
#include <TFT_eSPI.h> // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();       // Instantiate library

//USED FOR THE CABLE GROUPING CALC 
#include <cmath>  // Include the cmath library for math functions

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

//SETUP FOR THE CABLE LOOKUP FUNCTION
#define METHOD_E_SINGLE_PHASE 0
#define METHOD_E_THREE_PHASE 1
#define METHOD_D_SINGLE_PHASE 2
#define METHOD_D_THREE_PHASE 3
// Cross-sectional areas
const float crossSectionalAreas[17] = {1.5, 2.5, 4, 6, 10, 16, 25, 35, 50, 70, 95, 120, 150, 185, 240, 300, 400};
// Ratings for different methods
const int cableRatings[17][4] = {
  {22, 19, 22, 18},
  {31, 26, 29, 24},
  {41, 35, 37, 30},
  {53, 45, 46, 38},
  {72, 62, 60, 50},
  {97, 83, 78, 64},
  {128, 110, 99, 82},
  {157, 135, 119, 98},
  {190, 163, 140, 116},
  {241, 207, 173, 143},
  {291, 251, 204, 169},
  {336, 290, 231, 192},
  {386, 332, 261, 217},
  {439, 378, 292, 243},
  {516, 445, 336, 280},
  {592, 510, 379, 316},
  {683, 590, 0, 0}
};

// //ARRAY FOR SINGLE CORE CABLES
// int cableRatings2[13][7] = {
//   // {Cable size, Method C - 2 cables, Method C - 3 cables, Method F - 2 cables, Method F - 3 cables flat, Method F - 3 cables trefoil, Method D - Singles Ducted (A)}
//   {50, 193, 179, 205, 189, 181, 104},
//   {70, 245, 225, 259, 238, 231, 133},
//   {95, 296, 269, 313, 285, 280, 162},
//   {120, 342, 309, 360, 327, 324, 187},
//   {150, 393, 352, 413, 373, 373, 212},
//   {185, 447, 399, 469, 422, 425, 244},
//   {240, 525, 465, 550, 492, 501, 288},
//   {300, 594, 515, 624, 547, 567, 327},
//   {400, 687, 575, 723, 618, 657, 374},  // -1 indicating N/A
//   {500, 763, 622, 805, 673, 731, 424},  // -1 indicating N/A
//   {630, 843, 669, 891, 728, 809, -1},  // -1 indicating N/A
//   {800, 919, 710, 976, 777, 886, -1},  // -1 indicating N/A
//   {1000, 975, 737, 1041, 808, 945, -1} // -1 indicating N/A
// };

//ARRAY FOR SINGLE CORE CABLES
int cableRatings2[13][7] = {
  // {Cable size, Method C - 2 cables, Method C - 3 cables, Method F - 2 cables, Method F - 3 cables flat, Method F - 3 cables trefoil, Method D - Singles Ducted (A)}
  {50, 237, 220, 282, 232, 222, 199},
  {70, 303, 277, 357, 299, 285, 241},
  {95, 367, 333, 436, 352, 346, 282},
  {120, 425, 383, 504, 405, 397, 311},
  {150, 488, 437, 566, 462, 463, 342},
  {185, 557, 496, 643, 529, 518, 375},
  {240, 656, 579, 749, 626, 600, 419},
  {300, 755, 662, 842, 707, 679, 459},
  {400, 853, 737, 929, 813, 775, 489},  // -1 indicating N/A
  {500, 962, 791, 1032, 928, 887, 523},  // -1 indicating N/A
  {630, 1082, 861, 1139, 1049, 1017, 563},  // -1 indicating N/A
  {800, 1170, 904, 1204, 1141, 1105, 587},  // -1 indicating N/A
  {1000, 1261, 961, 1289, 1214, 1210, 621} // -1 indicating N/A
};
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
    if (buttonPressCount > 11) {
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
  rotaryEncoder.setBoundaries(0, 90000, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)

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
        display_CABLE_MULTIPLE_TRAY(); // Run the display Cable Duct program
    }else if (buttonPressCount == 8) {
        display_CABLE_DUCT(); // Run the display Cable Duct program
    }else if (buttonPressCount == 9) {
        display_CABLE_MULTIPLE_DUCT(); // Run the display Cable Duct program
    }else if (buttonPressCount == 10) {
        display_CABLE_MULTIPLE_TRAY_SINGLES(); // Run the display Cable Duct program
    }else if (buttonPressCount == 11) {
        display_CABLE_MULTIPLE_DUCT_SINGLES(); // Run the display Cable Duct program
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


// Function to find the minimum cable size for Method E - Single Phase
String findCableSizeForMethodESinglePhase(int current) {
    return findCableSize(current, METHOD_E_SINGLE_PHASE);
}

// Function to find the minimum cable size for Method E - Three Phase
String findCableSizeForMethodEThreePhase(int current) {
    return findCableSize(current, METHOD_E_THREE_PHASE);
}

// Function to find the minimum cable size for Method D - Single Phase
String findCableSizeForMethodDSinglePhase(int current) {
    return findCableSize(current, METHOD_D_SINGLE_PHASE);
}

// Function to find the minimum cable size for Method D - Three Phase
String findCableSizeForMethodDThreePhase(int current) {
    return findCableSize(current, METHOD_D_THREE_PHASE);
}

// Function to find the minimum cable size for Method F - 3 cables trefoil
String findCableSizeForMethodFThreePhaseSinglesTrefoil(int current) {
    return findCableSize2(current, 5);
}

// Function to find the minimum cable size for Method D - 3 cables trefoil
String findCableSizeForMethodDThreePhaseSinglesDucted(int current) {
    return findCableSize2(current, 6);
}

// FUNCTION TO FIND THE LAST CURRENT RATING WHICH WAS LARGER AND RETURN THE LAST CABLE SIZE
String findCableSize(int current, int methodIndex) {
    float lastValidSize = -1;  // Stores the last valid cable size

    for (int i = 16; i >= 0; i--) {  // Start from the largest area
        if (cableRatings[i][methodIndex] >= current) {
            lastValidSize = crossSectionalAreas[i];  // Update last valid size
        } else if (lastValidSize != -1) {
            // We've found a smaller rating after finding a valid size
            return String(lastValidSize);  // Convert float to String before returning
        }
    }

    if (lastValidSize == -1) {
        return "N/A   ";  // Return "N/A" if no valid size is found
    } else {
        return String(lastValidSize);  // Ensure the last valid size is returned as a String
    }
}

//METHOD USED FOR FINDING CABLE SIZE WHEN THE CABLES ARE DUCTED. 
String findCableSize2(int current, int methodIndex) {
    int lastValidSize = -1;  // Stores the last valid cable size

    for (int i = 12; i >= 0; i--) {  // Start from the largest area
        if (cableRatings2[i][methodIndex] >= current) {
            lastValidSize = cableRatings2[i][0];  // Update last valid size with the cable size
        } else if (lastValidSize != -1) {
            // We've found a smaller rating after finding a valid size
            return String(lastValidSize);  // Convert int to String before returning
        }
    }

    if (lastValidSize == -1) {
        return "N/A";  // Return "N/A" if no valid size is found
    } else {
        return String(lastValidSize);  // Ensure the last valid size is returned as a String
    }
}


void display_CABLE_TRAY() {
  //FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING
  float Pos_1_float = float(Pos_1);
  Pos_2 = padString("Amps", paddingValue);
  Pos_3 = padString("     1No", paddingValue);
  Pos_4 = padString("Multicore", paddingValue);
  Pos_3_1 = padString("Ladder / Tray - SPN", paddingValue);
  Pos_3_2 = padString("Cg 1: " + findCableSizeForMethodESinglePhase(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_3_3 = padString("Cg 2: " + findCableSizeForMethodESinglePhase(ceil(Pos_1_float / 0.87)) + "mm", paddingValue);
  Pos_3_4 = padString("Cg 3: " + findCableSizeForMethodESinglePhase(ceil(Pos_1_float / 0.82)) + "mm", paddingValue);
  Pos_3_5 = padString("Cg 4-5: " + findCableSizeForMethodESinglePhase(ceil(Pos_1_float / 0.8)) + "mm", paddingValue);
  Pos_3_6 = padString("Cg 6-7: " + findCableSizeForMethodESinglePhase(ceil(Pos_1_float / 0.79)) + "mm", paddingValue);
  Pos_3_7 = padString("Cg 8-20: " + findCableSizeForMethodESinglePhase(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
  Pos_4_1 = padString("Ladder / Tray - TPN", paddingValue);
  Pos_4_2 = padString("Cg 1: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_4_3 = padString("Cg 2: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float / 0.87)) + "mm", paddingValue);
  Pos_4_4 = padString("Cg 3: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float / 0.82)) + "mm", paddingValue);
  Pos_4_5 = padString("Cg 4-5: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float / 0.8)) + "mm", paddingValue);
  Pos_4_6 = padString("Cg 6-7: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float / 0.79)) + "mm", paddingValue);
  Pos_4_7 = padString("Cg 8-20: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
}

void display_CABLE_DUCT() {
  ////FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING
  float Pos_1_float = float(Pos_1);
  Pos_2 = padString("Amps", paddingValue);
  Pos_3 = padString("     1No", paddingValue);
  Pos_4 = padString("Multicore", paddingValue);
  Pos_3_1 = padString("Ducted - SPN", paddingValue);
  Pos_3_2 = padString("Cg 1: " + findCableSizeForMethodDSinglePhase(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_3_3 = padString("Cg 2: " + findCableSizeForMethodDSinglePhase(ceil(Pos_1_float / 0.87)) + "mm", paddingValue);
  Pos_3_4 = padString("Cg 3: " + findCableSizeForMethodDSinglePhase(ceil(Pos_1_float / 0.82)) + "mm", paddingValue);
  Pos_3_5 = padString("Cg 4-5: " + findCableSizeForMethodDSinglePhase(ceil(Pos_1_float / 0.8)) + "mm", paddingValue);
  Pos_3_6 = padString("Cg 6-7: " + findCableSizeForMethodDSinglePhase(ceil(Pos_1_float / 0.79)) + "mm", paddingValue);
  Pos_3_7 = padString("Cg 8-20: " + findCableSizeForMethodDSinglePhase(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
  Pos_4_1 = padString("Ducted - TPN", paddingValue);
  Pos_4_2 = padString("Cg 1: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_4_3 = padString("Cg 2: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float / 0.87)) + "mm", paddingValue);
  Pos_4_4 = padString("Cg 3: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float / 0.82)) + "mm", paddingValue);
  Pos_4_5 = padString("Cg 4-5: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float / 0.8)) + "mm", paddingValue);
  Pos_4_6 = padString("Cg 6-7: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float / 0.79)) + "mm", paddingValue);
  Pos_4_7 = padString("Cg 8-20: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
}



void display_CABLE_MULTIPLE_TRAY() {
  ////FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING
  float Pos_1_float = float(Pos_1);
  Pos_2 = padString("Amps", paddingValue);
  Pos_3 = padString("Parallel", paddingValue);
  Pos_4 = padString("Multicore", paddingValue);
  Pos_3_1 = padString("Ladder / tray - TPN", paddingValue);
  Pos_3_2 = padString("Cg 1 1x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_3_3 = padString("Cg 8-20 1x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
  Pos_3_4 = padString("Cg 1 2x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float/2)) + "mm", paddingValue);
  Pos_3_5 = padString("Cg 8-20 2x: " + findCableSizeForMethodEThreePhase(ceil((Pos_1_float/2) / 0.78)) + "mm", paddingValue);
  Pos_3_6 = padString("Cg 1 3x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float/3)) + "mm", paddingValue);
  Pos_3_7 = padString("Cg 8-20 3x: " + findCableSizeForMethodEThreePhase(ceil((Pos_1_float/3) / 0.78)) + "mm", paddingValue);
  Pos_4_1 = padString("Ladder / tray - TPN", paddingValue);
  Pos_4_2 = padString("Cg 1 4x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float/4)) + "mm", paddingValue);
  Pos_4_3 = padString("Cg 8-20 4x: " + findCableSizeForMethodEThreePhase(ceil((Pos_1_float/4) / 0.78)) + "mm", paddingValue);
  Pos_4_4 = padString("Cg 1 5x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float/5)) + "mm", paddingValue);
  Pos_4_5 = padString("Cg 8-20 5x: " + findCableSizeForMethodEThreePhase(ceil((Pos_1_float/5) / 0.78)) + "mm", paddingValue);
  Pos_4_6 = padString("Cg 1 6x: " + findCableSizeForMethodEThreePhase(ceil(Pos_1_float/6)) + "mm", paddingValue);
  Pos_4_7 = padString("Cg 8-20 6x: " + findCableSizeForMethodEThreePhase(ceil((Pos_1_float/6) / 0.78)) + "mm", paddingValue);
}


void display_CABLE_MULTIPLE_DUCT() {
  ////FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING
  float Pos_1_float = float(Pos_1);
  Pos_2 = padString("Amps", paddingValue);
  Pos_3 = padString("Parallel", paddingValue);
  Pos_4 = padString("Multicore", paddingValue);
  Pos_3_1 = padString("Ducted - TPN", paddingValue);
  Pos_3_2 = padString("Cg 1 1x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_3_3 = padString("Cg 8-20 1x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
  Pos_3_4 = padString("Cg 1 2x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float/2)) + "mm", paddingValue);
  Pos_3_5 = padString("Cg 8-20 2x: " + findCableSizeForMethodDThreePhase(ceil((Pos_1_float/2) / 0.78)) + "mm", paddingValue);
  Pos_3_6 = padString("Cg 1 3x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float/3)) + "mm", paddingValue);
  Pos_3_7 = padString("Cg 8-20 3x: " + findCableSizeForMethodDThreePhase(ceil((Pos_1_float/3) / 0.78)) + "mm", paddingValue);
  Pos_4_1 = padString("Ducted - TPN", paddingValue);
  Pos_4_2 = padString("Cg 1 4x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float/4)) + "mm", paddingValue);
  Pos_4_3 = padString("Cg 8-20 4x: " + findCableSizeForMethodDThreePhase(ceil((Pos_1_float/4) / 0.78)) + "mm", paddingValue);
  Pos_4_4 = padString("Cg 1 5x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float/5)) + "mm", paddingValue);
  Pos_4_5 = padString("Cg 8-20 5x: " + findCableSizeForMethodDThreePhase(ceil((Pos_1_float/5) / 0.78)) + "mm", paddingValue);
  Pos_4_6 = padString("Cg 1 6x: " + findCableSizeForMethodDThreePhase(ceil(Pos_1_float/6)) + "mm", paddingValue);
  Pos_4_7 = padString("Cg 8-20 6x: " + findCableSizeForMethodDThreePhase(ceil((Pos_1_float/6) / 0.78)) + "mm", paddingValue);
}


void display_CABLE_MULTIPLE_TRAY_SINGLES() {
  ////FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING
  float Pos_1_float = float(Pos_1);
  Pos_2 = padString("Amps", paddingValue);
  Pos_3 = padString("Parallel", paddingValue);
  Pos_4 = padString("Trefoil", paddingValue);
  Pos_3_1 = padString("Ladder / tray - TPN", paddingValue);
  Pos_3_2 = padString("Cg 1 1x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_3_3 = padString("Cg 8-20 1x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
  Pos_3_4 = padString("Cg 1 2x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float/2)) + "mm", paddingValue);
  Pos_3_5 = padString("Cg 8-20 2x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil((Pos_1_float/2) / 0.78)) + "mm", paddingValue);
  Pos_3_6 = padString("Cg 1 3x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float/3)) + "mm", paddingValue);
  Pos_3_7 = padString("Cg 8-20 3x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil((Pos_1_float/3) / 0.78)) + "mm", paddingValue);
  Pos_4_1 = padString("Ladder / tray - TPN", paddingValue);
  Pos_4_2 = padString("Cg 1 4x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float/4)) + "mm", paddingValue);
  Pos_4_3 = padString("Cg 8-20 4x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil((Pos_1_float/4) / 0.78)) + "mm", paddingValue);
  Pos_4_4 = padString("Cg 1 5x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float/5)) + "mm", paddingValue);
  Pos_4_5 = padString("Cg 8-20 5x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil((Pos_1_float/5) / 0.78)) + "mm", paddingValue);
  Pos_4_6 = padString("Cg 1 6x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil(Pos_1_float/6)) + "mm", paddingValue);
  Pos_4_7 = padString("Cg 8-20 6x: " + findCableSizeForMethodFThreePhaseSinglesTrefoil(ceil((Pos_1_float/6) / 0.78)) + "mm", paddingValue);
}


void display_CABLE_MULTIPLE_DUCT_SINGLES() {
  ////FUNCTION FOR GIVING SIZE OF CABLE USING ASSOCIATED GROUPING
  float Pos_1_float = float(Pos_1);
  Pos_2 = padString("Amps", paddingValue);
  Pos_3 = padString("Parallel", paddingValue);
  Pos_4 = padString("Singles", paddingValue);
  Pos_3_1 = padString("Ducted - TPN", paddingValue);
  Pos_3_2 = padString("Cg 1 1x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float)) + "mm", paddingValue);
  Pos_3_3 = padString("Cg 8-20 1x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float / 0.78)) + "mm", paddingValue);
  Pos_3_4 = padString("Cg 1 2x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float/2)) + "mm", paddingValue);
  Pos_3_5 = padString("Cg 8-20 2x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil((Pos_1_float/2) / 0.78)) + "mm", paddingValue);
  Pos_3_6 = padString("Cg 1 3x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float/3)) + "mm", paddingValue);
  Pos_3_7 = padString("Cg 8-20 3x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil((Pos_1_float/3) / 0.78)) + "mm", paddingValue);
  Pos_4_1 = padString("Ducted - TPN", paddingValue);
  Pos_4_2 = padString("Cg 1 4x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float/4)) + "mm", paddingValue);
  Pos_4_3 = padString("Cg 8-20 4x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil((Pos_1_float/4) / 0.78)) + "mm", paddingValue);
  Pos_4_4 = padString("Cg 1 5x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float/5)) + "mm", paddingValue);
  Pos_4_5 = padString("Cg 8-20 5x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil((Pos_1_float/5) / 0.78)) + "mm", paddingValue);
  Pos_4_6 = padString("Cg 1 6x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil(Pos_1_float/6)) + "mm", paddingValue);
  Pos_4_7 = padString("Cg 8-20 6x: " + findCableSizeForMethodDThreePhaseSinglesDucted(ceil((Pos_1_float/6) / 0.78)) + "mm", paddingValue);
}


