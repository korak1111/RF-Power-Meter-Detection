#include <SPI.h>
#include "Click_RF_Meter_types.h"
#include "Click_RF_Meter_config.h"


void setup() {
  // put your setup code here, to run once:

  systemInit();
  applicationInit();
}

void loop() {
  // put your main code here, to run repeatedly:

  applicationTask();
}
/*
  Example for RF_Meter Click

    Date          : Apr 2019.
    Author        : Jovan Stajkovic

  Test configuration STM32 :

    MCU              : STM32F107VCT6
    Dev. Board       : EasyMx PRO v7 for STM32
    ARM Compiler ver : v6.1.0.0

  ---

  Description :

  The application is composed of three sections :

  - System Initialization - Initializes SPI module, LOG and GPIO structures, sets CS pin as output and AN pin as input.
  - Application Initialization - Initalizes SPI driver and makes an initial log.
  - Application Task - This is an example that shows the capabilities of the RF Meter click by measuring radio frequency signal strenght.

*/



float signal;
char logTxt[ 50 ];

void systemInit()
{
  mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_AN_PIN, _GPIO_INPUT );
  mikrobus_gpioInit( _MIKROBUS1, _MIKROBUS_CS_PIN, _GPIO_OUTPUT );

  mikrobus_spiInit( _MIKROBUS1, &_RFMETER_SPI_CFG[0] );

  mikrobus_logInit( _LOG_USBUART_A, 9600 );

  Delay_ms( 100 );
}

void applicationInit()
{
  rfmeter_spiDriverInit( (T_RFMETER_P)&_MIKROBUS1_GPIO, (T_RFMETER_P)&_MIKROBUS1_SPI );
  Delay_ms( 100 );
  mikrobus_logWrite( "-----------------------", _LOG_LINE );
  mikrobus_logWrite( "    RF Meter Click     ", _LOG_LINE );
  mikrobus_logWrite( "-----------------------", _LOG_LINE );
}

void applicationTask()
{
  signal = rfmeter_getSignalStrenght( _RFMETER_DEF_SLOPE, _RFMETER_DEF_INTERCEPT );

  mikrobus_logWrite( "Signal strenght: ", _LOG_TEXT );
  FloatToStr( signal, logTxt );
  mikrobus_logWrite( logTxt, _LOG_TEXT );
  mikrobus_logWrite( "dBm", _LOG_LINE );

  Delay_ms( 1000 );
  mikrobus_logWrite( "-----------------------", _LOG_LINE );
}
