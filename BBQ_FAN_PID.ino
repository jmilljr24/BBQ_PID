#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#include <PID_v1.h>
#define PIN_OUTPUT 9

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

//Define Variables for PID
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
//double Kp=2, Ki=5, Kd=1; //orignal
double Kp=2, Ki=1, Kd=5; //test1

PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

float temp1 = 55;

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  //delay(1000);
  Serial.println("BBQ PID");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.setTextSize(2); // Set the text size
  display.setTextColor(WHITE); // Set the text color 
  
  // Start up the library
  sensors.begin();
  
  Input = temp1;
  Setpoint = 225;
  
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop(void)
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  sensors.requestTemperatures(); // Send the command to get temperatures

  
  Serial.print("Ambient temperature is ");
  temp1 = sensors.getTempFByIndex(0);
  Serial.println(temp1, 1); //change second variable for number of decimal places
  
  if (temp1 > 50)
  {
  myPID.SetMode(AUTOMATIC);
   
  Input = temp1;
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
  Serial.println(Output); 
  }
  else
  {
  myPID.SetMode(MANUAL);
  analogWrite(PIN_OUTPUT, 70);
  }
  
  
  display.clearDisplay(); // Clear the display
  display.setCursor(0,0); // Set the cursor at the beginning of the display
  display.print(temp1,1); // Print temperature with single decimal
  display.println("F"); // Append the unit
  display.display(); // Display the temperature and unit on the display
  
  display.setCursor(0,20); // Set the cursor at the beginning of the display
  display.print(Output,0); // Print temperature with single decimal
  //display.println("c"); // Append the unit
  display.display(); // Display the temperature and unit on the display
  
 /* if (temp1 = 0 )
  { 
    myPID.SetMode(MANUAL);
    Output = 60;
    myPID.SetMode(AUTOMATIC);
  } */
    
}
