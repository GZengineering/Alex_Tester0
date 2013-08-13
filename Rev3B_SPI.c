/**
* Flash memory functions.  
* These functions are used to interface with the flash memory unit on the device.
* It 
*/

#include "p24FJ16GA002.h"
#include "REV3B_SPI.h"

/**
* Initialize Flash Memory 
*/
/*
void FLASH_MEM_Init (void)
{
  //MISO
    MISO_TRIS = 1;			//Set pin as input
	//Assign function of SDI1R (in peripheral input register 20) to pin RP5
    RPINR20bits.SDI1R = 5;			
  
  //MOSI
	MOSI_TRIS = 0;			//Set pin as output
    //Assign pin RP6 (in peripheral output register 3) to function as SPI1TX ("7")
    RPOR3bits.RP6R = SPI_TX_FUNCTION;

  //SPICLK
	SPI_CLK_TRIS = 0;		//Set pin as output
	//Assign pin RP4 (in peripheral output register 2) to function as SPI1CLK ("8")
    RPOR2bits.RP4R = SPI_CLK_FUNCTION;          

  IFS0bits.SPI1IF			= 0;	// Clear the interrup flag
  SPI1CON1bits.DISSCK		= 0;	// 0= Internal SPIx clock is enabled
  SPI1CON1bits.DISSDO		= 0;	// 0= SDOx pin is controlled by the module
  SPI1CON1bits.MODE16 		= 0;	// 0= set 8-bit mode
  SPI1CON1bits.SMP			= 0;	// 0= Input data sampled at middle of data output time (not sure what this does)
  SPI1CON1bits.SSEN			= 0;	
  SPI1CON1bits.CKP 			= 0;	// 0= Idle state for clock is a low level; active state is a high level
  SPI1CON1bits.CKE 			= 0;	// 0= Serial output data changes on transition from Idle clock state to active clock state
  SPI1CON1bits.MSTEN 		= 1; 	// 1 =  Master mode; 0 =  Slave mode
  SPI1CON1bits.PPRE 		= 0b11; // 11= Primary Prescalar  =1:1
  SPI1CON1bits.SPRE			= 0b111;// 111= Secondary Prescalar=1:1
  SPI1CON2 					= 0;	// 0= non-framed mode
  SPI1STATbits.SPIROV		= 0;	// 0= Clear the Receive Overflow flag bit
  SPI1STATbits.SISEL		= 1;    // 1= Interrupt when data is available in receive buffer
  SPI1STATbits.SPIEN 		= 1;	//Enables SPI module and configures SCKx,SDOx,SDIx and ~SSx as serial port pins

}
*/


/**
* 
*/
/*
void UNBLOCK_MEMORY(void)
{
  unsigned int temp;
///Write to Status Register and Unblock all of the memory
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0x50;						//Write to Status Register Enable Command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  SPI1BUF = 0b10101010;					//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  Nop();
  Nop();
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0x01;						//Write to Status Register command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0x00;						//Status Register Bits (Disable Block Protection)
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
}
*/

/**
* Memory ID?  get JEDEC_ID of Memory chip.
*/
/*
void JEDEC_ID (void)
{
  unsigned int temp;
////JEDEC_ID
  //IFS0bits.SPI1IF			= 0;
  FLASH_DISABLE_BIT = 0;			// Enable Flash Chip
  SPI1BUF = 0b10011111;					//JEDEC-ID command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0b10101010;					//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  //printf("%x\n",temp);
  SPI1BUF = 0b10101010;					//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  //printf("%x\n",temp);
  SPI1BUF = 0b10101010;					//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;						//Read the data from the chip
  FLASH_DISABLE_BIT = 1;          		// Disable Flash Chip  	
  //printf("%x\n",temp);
}
*/

/*
* Get Manuf_ID of memory chip.
*/
/*
unsigned int MANUF_ID(void)
{
  //printf("In");
  unsigned int temp;
////READ_ID
  //IFS0bits.SPI1IF			= 0;
  FLASH_DISABLE_BIT = 0;				// Enable Flash Chip
  SPI1BUF = 0x90;					//READ-ID command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0x00;					//Address Byte 1
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0x00;					//Address Byte 2
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0x00;					//Address Byte 3
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0b10101010;					//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("%x\n",temp);
  return temp;
}
*/

/**
* Read the status register of the flash chip.
*/
/*
void READ_STATUS_REG(void)
{
  unsigned int temp;
///Read Status Register
  FLASH_DISABLE_BIT = 0;				// Enable Flash Chip
  SPI1BUF = 0x05;						//Read Status Register command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0xAA;						//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("%x\n",temp);
}
*/

/**
* Erase 4 kilobytes starting 
*/
/*
void _4KBYTE_ERASE(char address1,char address2,char address3)
{
  unsigned int temp;	  
///Erase 4KBytes
  WRITE_ENABLE();
  //Erase Sector
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0x20;						//Erase the next 4KBytes
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address1;					//Address Byte 1
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address2;					//Address Byte 2
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address3;					//Address Byte 3
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("# 4KByte Sector Erased\n");
  for(temp =0;temp<5000;temp++);
}
*/

/**
* Erase 32 kilobytes at the given address bits
*/
/*
void _32KBYTE_ERASE(char address1,char address2,char address3)
{
  unsigned int temp;	  
///Erase 32KBytes
  WRITE_ENABLE();
  //Erase Sector
  FLASH_DISABLE_BIT = 0;	sss			//Enable Flash Chip
  SPI1BUF = 0x52;						//Erase the next 4KBytes
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address1;					//Address Byte 1
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address2;					//Address Byte 2
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address3;					//Address Byte 3
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("# 32KByte Sector Erased\n");
  for(temp =0;temp<10000;temp++);
}
*/

/* 
* Erase 64 kilobytes at the 
*/
/*
void _64KBYTE_ERASE(char address1,char address2,char address3)
{
  unsigned int temp;	  
///Erase 64KBytes
  WRITE_ENABLE();
  //Erase Sector
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0xD8;						//Erase the next 4KBytes
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address1;					//Address Byte 1
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address2;					//Address Byte 2
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address3;					//Address Byte 3
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("# 64KByte Sector Erased\n");
  for(temp =0;temp<10000;temp++);
}
*/

/**
* Clear the data on the flash chip
*/
/*
void CHIP_ERASE(void)
{
  unsigned int temp;
///Chip Erase
  WRITE_ENABLE();
  for(temp =0;temp<100;temp++);
  //Erase Chip
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0x60;						//Chip Erase Command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;

  for(temp =0;temp<20000;temp++);	
}
*/

/**
* Read a byte from memory.
*/
/*
unsigned int READ_BYTE(char address1,char address2,char address3)
{
  unsigned int temp;
///Read Byte
  FLASH_DISABLE_BIT = 0;				// Enable Flash Chip
  SPI1BUF = 0x03;						//Read Byte command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address1;						//Address Byte 1
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address2;						//Address Byte 2
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address3;						//Address Byte 3
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = 0xAA;						//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("%x\n",temp);
  return temp;
}
*/

/**
* 
*/
/*
unsigned int READ_BYTE_AUTO(char state)
{
  unsigned int temp;
  if(state == 0)
  {
	FLASH_DISABLE_BIT = 0;				// Enable Flash Chip
	SPI1BUF = 0x03;						//Read Byte command
	while(SPI1STATbits.SPIRBF == 0);
	temp = SPI1BUF;
	SPI1BUF = 0;						//Address Byte 1
	while(SPI1STATbits.SPIRBF == 0);
	temp = SPI1BUF;
	SPI1BUF = 0;						//Address Byte 2
	while(SPI1STATbits.SPIRBF == 0);
	temp = SPI1BUF;
	SPI1BUF = 0;						//Address Byte 3
	while(SPI1STATbits.SPIRBF == 0);
	temp = SPI1BUF;
  }
  if(state == 1)
  {
	SPI1BUF = 0xAA;						//Dummy Byte
	while(SPI1STATbits.SPIRBF == 0);
	temp = SPI1BUF;
	return temp;
  }
  return 0;
}
*/

/**
* Enable the chip to write to be able to write to memory.
*/
/*
void WRITE_ENABLE(void)
{
  unsigned int temp;
//Write Enable
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0x06;						//Write Enable Command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  SPI1BUF = 0b10101010;					//Dummy Byte
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  Nop();
  Nop();
}
*/

/**
* Write the given data to the given address
*/
/*
void WRITE_BYTE(char address1,char address2,char address3, char data)
{
  unsigned int temp;
//Write Byte
  WRITE_ENABLE();
  //Program Byte
  FLASH_DISABLE_BIT = 0;				//Enable Flash Chip
  SPI1BUF = 0x02;						//Program Byte command
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address1;						//Address Byte 1
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address2;						//Address Byte 2
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = address3;						//Address Byte 3
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  SPI1BUF = data;						//Byte being written
  while(SPI1STATbits.SPIRBF == 0);
  temp = SPI1BUF;
  FLASH_DISABLE_BIT = 1;				// Disable Flash Chip
  //printf("ByteWritten\n")
}
*/
