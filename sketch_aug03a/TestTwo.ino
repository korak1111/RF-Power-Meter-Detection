#include <SPI.h>
/* CS D10 (Chip Select)
 * SCK D13 (Clock)
 * MISO D12 (Master-In, Slave-Out)
 * 
 * MOSID11 (Matser-out, Slave-In)
 */

// MCP3201 12-Bit ADC to Arduino Nano Ver. 3.0
// Simple read of the 12-bit data and converted
// to read the applied volts. Used an MCP1541 4.096V
// reference IC. With a good reference regulator
// like the MCP1541, this "volt meter" will match
// or beat many comercial volt meters in the
// 0V to 4.096V scale down to 1mv.
//
// Nano pins     MCP3201 Pins    Function
//    D10           5             CS (NOT)
//    D12           6             Dout
//    D13           7             CLK
//                  8             Vdd (5V)
//                  1             Ref+
//                  2             IN+
//                  3             IN- (to GND)
//                  4             Vss (GND)
//
// Note: MCP1541 REQUIRES ~.1uf cap between the
// output and ground. Looking at the printed face
// of the MCP1541 (TO92 package), the leads are (left-to-right):
// ground, output, input. Input can be Vcc 5V.



const byte DAT = 12; // SPI MISO Pin
const byte CLK = 13; // SPI Clock Pin
const byte CS = 10;  // SPI SS Pin (Chip Select)

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

digitalWrite(CS,LOW);
reading = SPI.transfer16(0x0000); // "reading" captured, but it needs
digitalWrite(CS,HIGH);            // the following 2 line to clean it up:

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

float voltage = reading * (4.096 / 4095); // Using 4.096V Ref IC
Serial.println(voltage, 3);    // voltage shown on terminal (1mv res.)
delay(250);                    // Adjust delay as needed

}
