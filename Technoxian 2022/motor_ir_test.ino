
// Include Libraries
#include "Arduino.h"
#include "DCMDriverL298.h"
#include "Button.h"


// Pin Definitions
#define DCMOTORDRIVERL298_PIN_INT1	2
#define DCMOTORDRIVERL298_PIN_ENB	6
#define DCMOTORDRIVERL298_PIN_INT2	3
#define DCMOTORDRIVERL298_PIN_ENA	5
#define DCMOTORDRIVERL298_PIN_INT3	4
#define DCMOTORDRIVERL298_PIN_INT4	7
#define REFLECTIVEIR_1_PIN_COLL	8
#define REFLECTIVEIR_2_PIN_COLL	9
#define REFLECTIVEIR_3_PIN_COLL	10
#define REFLECTIVEIR_4_PIN_COLL	11
#define REFLECTIVEIR_5_PIN_COLL	12
#define REFLECTIVEIR_6_PIN_COLL	13



// Global variables and defines

// object initialization
DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);
Button ReflectiveIR_1(REFLECTIVEIR_1_PIN_COLL);
Button ReflectiveIR_2(REFLECTIVEIR_2_PIN_COLL);
Button ReflectiveIR_3(REFLECTIVEIR_3_PIN_COLL);
Button ReflectiveIR_4(REFLECTIVEIR_4_PIN_COLL);
Button ReflectiveIR_5(REFLECTIVEIR_5_PIN_COLL);
Button ReflectiveIR_6(REFLECTIVEIR_6_PIN_COLL);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    ReflectiveIR_1.init();
    ReflectiveIR_2.init();
    ReflectiveIR_3.init();
    ReflectiveIR_4.init();
    ReflectiveIR_5.init();
    ReflectiveIR_6.init();
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // L298N Motor Driver with Dual Hobby DC motors - Test Code
    //Start both motors. note that rotation direction is determined by the motors connection to the driver.
    //You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
    dcMotorDriverL298.setMotorA(200,1);
    dcMotorDriverL298.setMotorB(200,0);
    delay(2000);
    //Stop both motors
    dcMotorDriverL298.stopMotors();
    delay(2000);

    }
    else if(menuOption == '2') {
    // Reflective IR Sensor TCRT5000L #1 - Test Code
    //read Reflective IR Sensor. 
    //if the sensor detects an echo function will return LOW (0). 
    //if it is not detect an echo function will return HIGH (1).
    bool ReflectiveIR_1_Val = ReflectiveIR_1.read();
    Serial.print(F("Val: ")); Serial.println(ReflectiveIR_1_Val);
    }
    else if(menuOption == '3') {
    // Reflective IR Sensor TCRT5000L #2 - Test Code
    //read Reflective IR Sensor. 
    //if the sensor detects an echo function will return LOW (0). 
    //if it is not detect an echo function will return HIGH (1).
    bool ReflectiveIR_2_Val = ReflectiveIR_2.read();
    Serial.print(F("Val: ")); Serial.println(ReflectiveIR_2_Val);
    }
    else if(menuOption == '4') {
    // Reflective IR Sensor TCRT5000L #3 - Test Code
    //read Reflective IR Sensor. 
    //if the sensor detects an echo function will return LOW (0). 
    //if it is not detect an echo function will return HIGH (1).
    bool ReflectiveIR_3_Val = ReflectiveIR_3.read();
    Serial.print(F("Val: ")); Serial.println(ReflectiveIR_3_Val);
    }
    else if(menuOption == '5') {
    // Reflective IR Sensor TCRT5000L #4 - Test Code
    //read Reflective IR Sensor. 
    //if the sensor detects an echo function will return LOW (0). 
    //if it is not detect an echo function will return HIGH (1).
    bool ReflectiveIR_4_Val = ReflectiveIR_4.read();
    Serial.print(F("Val: ")); Serial.println(ReflectiveIR_4_Val);
    }
    else if(menuOption == '6') {
    // Reflective IR Sensor TCRT5000L #5 - Test Code
    //read Reflective IR Sensor. 
    //if the sensor detects an echo function will return LOW (0). 
    //if it is not detect an echo function will return HIGH (1).
    bool ReflectiveIR_5_Val = ReflectiveIR_5.read();
    Serial.print(F("Val: ")); Serial.println(ReflectiveIR_5_Val);
    }
    else if(menuOption == '7') {
    // Reflective IR Sensor TCRT5000L #6 - Test Code
    //read Reflective IR Sensor. 
    //if the sensor detects an echo function will return LOW (0). 
    //if it is not detect an echo function will return HIGH (1).
    bool ReflectiveIR_6_Val = ReflectiveIR_6.read();
    Serial.print(F("Val: ")); Serial.println(ReflectiveIR_6_Val);
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) L298N Motor Driver with Dual Hobby DC motors"));
    Serial.println(F("(2) Reflective IR Sensor TCRT5000L #1"));
    Serial.println(F("(3) Reflective IR Sensor TCRT5000L #2"));
    Serial.println(F("(4) Reflective IR Sensor TCRT5000L #3"));
    Serial.println(F("(5) Reflective IR Sensor TCRT5000L #4"));
    Serial.println(F("(6) Reflective IR Sensor TCRT5000L #5"));
    Serial.println(F("(7) Reflective IR Sensor TCRT5000L #6"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing Reflective IR Sensor TCRT5000L #1"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing Reflective IR Sensor TCRT5000L #2"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing Reflective IR Sensor TCRT5000L #3"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing Reflective IR Sensor TCRT5000L #4"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing Reflective IR Sensor TCRT5000L #5"));
    		else if(c == '7') 
    			Serial.println(F("Now Testing Reflective IR Sensor TCRT5000L #6"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}