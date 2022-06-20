/*
 * main.c
 *
  *      Author: ABDO MOHAMED SAYED
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <avr/delay.h>

#include "DIO_interface.h"
#include "SPI_interface.h"

#include "EEPROM24C08_interface.h"
#include "LCD_interface.h"


int main(void){

	/******** Configure SPI Slave Pins *********
	 * SPI_SS_PIN   (PB4) --> Input
	 * SPI_MOSI_PIN (PB5) --> Input
	 * SPI_MISO_PIN (PB6) --> Output
	 * SPI_SCK_PIN  (PB7) --> Input
	 ********************************************/

	DIO_voidSetPinDirection(SPI_SS_PIN  , DIO_INPUT );
	DIO_voidSetPinDirection(SPI_MOSI_PIN, DIO_INPUT );
	DIO_voidSetPinDirection(SPI_MISO_PIN, DIO_OUTPUT);
	DIO_voidSetPinDirection(SPI_SCK_PIN , DIO_INPUT );

	/* Configure Relay pin */
	DIO_voidSetPinDirection(DIO_PIND0, DIO_OUTPUT);
	DIO_voidSetPinDirection(DIO_PIND1, DIO_OUTPUT);

	/* Configure SPI During RunTime */
	SPI0.EnableControl  = SPI_ENABLE;
	SPI0.Mode 		    = SPI_SLAVE;
	SPI0.ClockMode      = SPI_CLK_MODE0;
	SPI0.DataOrder      = SPI_MSB;
	SPI0.Freq           = SPI_FCPU_BY_4;
	SPI0.InterruptState = SPI_INTERRUPT_DISABLE;

	/* SPI Initialization */
	SPI_voidInit();

	uint8 EEPROM_StoreData[4] = "FAHD";
	uint8 EEPROM_ReadData [4] = {0};
	uint8 SPI_ReadData    [4] = {0};

	/* EEPROM Initialization */
	EEPROM24C08_voidInit();
	/* EEPROM store data */
	EEPROM24C08_voidWriteBlockData(10, EEPROM_StoreData, 4);
	_delay_ms(50);

	/* LCD Initialization */
	LCD_voidInit();
	LCD_voidCursorOff();
	LCD_voidClearScreen();

	uint8 Local_u8Flag = 0;

	while(1){

		Local_u8Flag = 0;

		for(uint8 i=0; i<4; i++){

			if(SPI_ReadData[i] != NULL){

				SPI_ReadData[i] = SPI_u8ReceiveChar();
				_delay_ms(50);

			}

		}


		EEPROM24C08_voidReadBlockData(10, 4, EEPROM_ReadData);
		_delay_ms(50);

		for(uint8 i=0; i<4; i++){

			if(SPI_ReadData[i] == EEPROM_ReadData[i]){

				Local_u8Flag ++;

			}
		}
		if(Local_u8Flag == 4){

			DIO_voidSetPinValue(DIO_PIND0, DIO_HIGH);
			DIO_voidSetPinValue(DIO_PIND1, DIO_HIGH);
			LCD_voidSetPosition(0, 0);
			LCD_voidWriteString("  ENGINE READY  ");
			LCD_voidSetPosition(1, 0);
			LCD_voidWriteString("  STARTING NOW  ");

		}
		else{

			DIO_voidSetPinValue(DIO_PIND0, DIO_LOW);
			DIO_voidSetPinValue(DIO_PIND1, DIO_LOW);
			LCD_voidSetPosition(0, 0);
			LCD_voidWriteString("  ENGINE IS NOT ");
			LCD_voidSetPosition(1, 0);
			LCD_voidWriteString(" READY START NOW ");

		}

	}
}
