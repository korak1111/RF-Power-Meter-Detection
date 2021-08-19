/**
 * Basic ADC reading example.
 * - connects to ADC
 * - reads value from channel
 * - converts value to analog voltage
 * 
 * Cyrus Rupa for Toronto Transit Commission August 8, 2021
 */

#include <SPI.h>
#include <Mcp320x.h>

#define SPI_CS      2       // SPI slave select
#define ADC_VREF    2500     // 2.5V Vref
#define ADC_CLK     1600000  // SPI clock 1.6MHz
#define SET_POINT 2000  //Mininum Signal Value for Green light
#define LED_GREEN 3
#define LED_RED 4

MCP3201 adc(ADC_VREF, SPI_CS);

void setup() {

  // configure PIN mode
  pinMode(SPI_CS, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // set initial PIN state
  digitalWrite(SPI_CS, HIGH);

  // initialize serial
  Serial.begin(115200);

  // initialize SPI interface for MCP3201
  SPISettings settings(ADC_CLK, MSBFIRST, SPI_MODE0);
  SPI.begin();
  SPI.beginTransaction(settings);
}

void loop() {

  uint32_t t1;
  uint32_t t2;

  // start sampling
  //Serial.println("Reading...");

  t1 = micros();
  uint16_t raw = adc.read(MCP3201::Channel::SINGLE_0);
  t2 = micros();

  // get analog value
  uint16_t val = adc.toAnalog(raw);

  // readed value
  //Serial.print("value: ");
  Serial.println(raw);
  //Serial.print(" (");
  //Serial.print(val);
  //Serial.println(" mV)");

  // sampling time
  //Serial.print("Sampling time: ");
 // Serial.print(static_cast<double>(t2 - t1) / 1000, 4);
  //Serial.println("ms");

  //Signal Checking
  //isValid(val);
  delay(1000);
}

void isValid(uint16_t input){
  if (input<=SET_POINT){ //Signal is insuffcient for safe travel
    Serial.println("Suffcient RF Signal");
    digitalWrite(LED_GREEN, HIGH); // Turn the GREEN LED on
    digitalWrite(LED_RED, LOW); // Turn the RED LED off
    //Send audio
  }else{
    Serial.println("Insuffcient RF Signal");
    digitalWrite(LED_RED, HIGH); // Turn the RED LED on
    digitalWrite(LED_GREEN, LOW); // Turn the GREEN LED off
    //Send audio
  }
}
