/*
 * SPI.c
 *
 *  Created on: 22 Feb 2023
 *      Author: Alaa Wahba
 */

#include "inc/SPI.h"

void SPI_Init() {
#ifdef MASTER_MODE
	/* GPIO pins Configuration */
	SET(DDRB, MOSI);
	SET(DDRB, SS);
    SET(DDRB, SCLK);
	CLEAR(DDRB, MISO);

	/* Master Configuration */
	// Enable SPI
    SET(SPCR, SPE);
    
	// Configure it as Master
    SET(SPCR, MSTR);
	// shift clock =clk/16
    SET(SPCR, SPR0);
    CLEAR(SPCR, SPR1);
    CLEAR(SPSR, SPI2X);
	
#endif

#ifdef SLAVE_MODE
	/* GPIO pins Configuration */
	CLEAR(DDRB, MOSI);
	CLEAR(DDRB, SS);
    CLEAR(DDRB, SCLK);
	SET(DDRB, MISO);
    
		
	/* Slave Configuration */
	// Enable SPI
    SET(SPCR, SPE);
	// Configure it as Slave
    CLEAR(SPCR, MSTR);
	

#endif

}
uint8 SPI_SendRecieveData(uint8 Data) {
    SPDR = Data;
	while (!GET(SPSR, SPIF))
		;
	return SPDR;

}
