#include <Arduino.h>
#include <Wire.h>
#include "const.h"
#include "state_machine.h"
#include "UART_intf.h"
#include "hd44780.h"
#include "hd44780ioClass/hd44780_I2Cexp.h" // Include the I2C expander IO class

// LCD dimensions (most common size is 16x2)
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// Create LCD object with updated I2C address (0x40)
hd44780_I2Cexp lcd(0x40, LCD_COLS, LCD_ROWS);

void scanI2C() {
  byte error, address;
  int devices = 0;
  
  Serial.println("Scanning I2C bus...");
  
  for(address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("!");
      devices++;
    }
  }
  
  if (devices == 0) {
    Serial.println("No I2C devices found");
  }
  Serial.println("Scanning complete");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Remove the internal pull-up settings - they're not strong enough
  pinMode(PB7, INPUT_PULLUP); // SDA
  pinMode(PB6, INPUT_PULLUP); // SCL
  
  Wire.setSDA(PB7);
  Wire.setSCL(PB6);
  Wire.setClock(10000); // Slow down the clock
  Wire.begin();
  
  delay(1000);
  
  Serial.println("Running I2C scanner...");
  scanI2C();
  
  // Initialize the LCD
  int status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status) { // non zero status means initialization failed
    Serial.print("LCD initialization failed: ");
    Serial.println(status);
    while(1); // halt
  }
  
  // Clear the display
  lcd.clear();
  
  // Set cursor to first column, first row
  lcd.setCursor(0, 0);
  
  // Display "Hi YV" on the LCD
  lcd.print("Hi YV");
  
  Serial.println("LCD initialized and message displayed");
  
  initStateMachine();
  initUART();

  Serial.printf("Initial State: %d\n", getStateMachineState());
}

void loop() {
  stateMachine(); 
  writeUART();
  
  // You can add LCD updates here if needed
  // For example, to show the current state:
  lcd.setCursor(0, 1);
  lcd.print("State: ");
  lcd.print(getStateMachineState());

  delay(5000);
}