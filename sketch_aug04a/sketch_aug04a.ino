#include <SPI.h>
/* CS D10 (Chip Select)
 * SCK D13 (Clock)
 * MISO D12 (Master-In, Slave-Out)
 * 
 * MOSID11 (Matser-out, Slave-In)
 */

// MCP3201 12-Bit ADC to Arduino Uno
// Simple read of the 12-bit data. 
//
// UNO pins     MCP3201 Pins    Function
//    D10           5             CS (NOT)
//    D12           6             Dout
//    D13           7             CLK
//                  8             Vdd (5V)
//                  1             Ref+
//                  2             IN+
//                  3             IN- (to GND)
//                  4             Vss (GND)
//




const byte DAT = 12; // SPI MISO Pin
const byte CLK = 13; // SPI Clock Pin
const byte CS = 10;  // SPI SS Pin (Chip Select)
const int TEMPERATURE = 0;  // SPI SS Pin (Chip Select)

void setup() {

Serial.begin(9600);
SPI.beginTransaction(SPISettings(1500000, MSBFIRST, SPI_MODE0));
SPI.begin();

pinMode(DAT,INPUT);
pinMode(CS,OUTPUT);


digitalWrite(CS,LOW);   // CS pin is cycled low then high because
digitalWrite(CS,HIGH);  // the PWR ON state of the pin is unknown.

digitalWrite(CLK,LOW);

}

void loop() {

unsigned int reading = 0;      // "reading" is the raw ADC value
unsigned int tempOut = 0;      //"tempOut" is an analog value of the temperature reading from the AD8318 Chip

digitalWrite(CS,LOW);
reading = SPI.transfer16(0x0000); // "reading" captured, but it needs
digitalWrite(CS,HIGH);            // the following 2 line to clean it up:
 tempOut=analogRead(TEMPERATURE);
//*********** IMPORTANT PART: *************************************
reading = reading << 3;   //Must shift out the 3 MSB's (trash bits)
reading = reading >> 4;   //Must shift back 4 MSB's (pushes off LSB)
//The above two lines clears the top 3 MSB that are unknown "trash",
//  then shifts back 4 bits to not only bring in all "0" for the 4
//  MSB's, but ALSO pushes off the LSB which came in from the LBS
//  reading that is transmitted in LSB order (another "trash" bit).
//*****************************************************************

// If using another reference voltage, replace the following
// "4.096" with the actual voltage used for reference.

float voltage = reading * (2.502 / 4095); // Using 4.096V Ref IC
//Serial.print("RF Signal: ");
//Serial.print(voltage, 3);    // voltage shown on terminal (1mv res.)
//Serial.print(" Temperature: ");
//Serial.println(tempOut);
Serial.println(reading);    // voltage shown on terminal (1mv res.)
delay(125);                    // Adjust delay as needed

}
