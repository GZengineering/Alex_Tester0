// ////////
// INCLUDES
#include <p24FJ16GA002.h>
#include <stdio.h>
#include "Tester_v0.h"
#include "REV3B_SPI.h"

// ///////
// DEFINES
#define GA0 1;

// ///////
// CONFIGS
#ifdef GA0
_CONFIG1(JTAGEN_OFF & FWDTEN_OFF);
_CONFIG2(POSCMOD_NONE & OSCIOFNC_OFF & FNOSC_FRC);
#else
//Watchdog Prescaler ratio of 1:128; Standard Watchdog Timer enabled; Watchdog Timer is disabled;
//Emulator functions are shared with PGEC1/PGED1; Writes to program memory are allowed;
//Code protection is disabled; JTAG port is disabled
_CONFIG1(FWPSA_PR128 & WINDIS_OFF & FWDTEN_OFF & ICS_PGx1 & GWRP_OFF & GCP_OFF & JTAGEN_OFF);
//Primary Oscillator disabled; Use default SCL1/SDA1 pins for I2C1; 
//Once set, the IOLOCK bit cannot be cleared; OSCO pin functions as port I/O (RA3); Sw Disabled, Mon Disabled; 
//Fast RC Oscillator (FRC); IESO mode (Two-Speed Start-up) enabled
_CONFIG2(POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_ON & OSCIOFNC_ON & FCKSM_CSDCMD & FNOSC_FRC & IESO_ON);
//SOSC pins have digital I/O functions (RA4, RB4); Default regulator start-up time used;
//Segmented code protection disabled; Last page and Flash Configuration words are unprotected;
//Write Protect from WPFP to the last page of memory
_CONFIG3(SOSCSEL_IO & WUTSEL_LEG & WPDIS_WPDIS & WPCFG_WPCFGDIS & WPEND_WPENDMEM); 
#endif

// ////
// MAIN
int main() 
{
	 ////Setup////
	 CHARGE_EN_RELAY_TRIS		= 0;			//Output
	 DISCHARGE_EN_PMOS_TRIS	= 0;     		//Output
	 CHARGE_EN_PMOS_TRIS 		= 0;        	//Output
	 FLASH_DISABLE_TRIS		= 0;			//Output	   
	 VPACK_OUT_TRIS			= 1;			//Input 
	 VPACK_IN_TRIS				= 1;			//Input
	 VSUPPLY_TRIS				= 1;			//Input 
	 VSHUNT_CHARGE_AMP_TRIS 	= 1;    		//Input
	 VSHUNT_DISCHARGE_AMP_TRIS	= 1;            //Input
	
	 DISCHARGE_EN_PMOS_BIT 	= 0;            
	 CHARGE_EN_PMOS_BIT 		= 0;
	 CHARGE_EN_RELAY_BIT 		= 0;
	 FLASH_DISABLE_BIT			= 1;			//Disable Flash
	
	 MEM_ADDR_STRUCT Mem_Addr_Struct;
	 Mem_Addr_Struct.Mem_Addr_Total = 0;
	
	 Timer1_Init();							//Initialize Timer 1
	 ADC_Init();								//Initialize ADC
	 UART_Init();
	 //FLASH_MEM_Init();
	 //UNBLOCK_MEMORY();

	unsigned char temp;

	printf("\nTester Unit running...\n\n");

	while(1)
	{
		if(INCOMING_UART_INPUT)
		{
			temp = UART_INPUT;
			
			switch(temp)
			{
				case 'c':
				case 'C':
				{
					printf("Hub sent a 'Charge' request.\n");
					break;
				}
				case 'd':
				case 'D':
				{
					printf("Hub sent a 'Discharge' request.\n");
					break;
				}
				default:
					printf("Hub sent an unfamiliar request.\n");
			} /*End Switch*/
		}/*End if(INCOMING_UART_INPUT)*/
	} /*End while*/
} /*End Main*/

void Timer1_Init (void)
{
  T1CONbits.TGATE = 0;         // No gated time accumulation
  T1CONbits.TSYNC = 0;         // Do not sync to an external clock
  T1CONbits.TCS = 0;           // Internal Clock
  T1CONbits.TCKPS = 0b11;      // Prescalar register 
  PR1 = 0xB6DB;                // Timer will count to this value before flag is set FFFF=4.2s,B6DB=3s
  IFS0bits.T1IF = 0;           // Timier 1 interrupt flag is cleared
  //IEC0bits.T1IE = 1;         // Enable Timer 1 Interrupt
  T1CONbits.TON = 1;           // Turn on Timer 1
}


void ADC_Init (void)
{
  VSHUNT_CHARGE_AMP_ADC_EN_BIT		= 0;	// Pin set as analog input, A/D samples voltage
  VSHUNT_DISCHARGE_AMP_ADC_EN_BIT	= 0;  	// Pin set as analog input, A/D samples voltage
  VPACK_OUT_ADC_EN_BIT			= 0; 	// Pin set as analog input, A/D samples voltage
  VPACK_IN_ADC_EN_BIT			= 0;	// Pin set as analog input, A/D samples voltage
  VSUPPLY_EN_BIT				= 0;	// Pin set as analog input, A/D samples voltage
  AD1CHSbits.CH0NA  = 0;       // Negative muxA value is Vss
  AD1CON2bits.VCFG = 0b001;    // Voltage range from Vref+ to Vss
  AD1CON3bits.SAMC  = 0b00111; // Auto-sample time bits
  AD1CON3bits.ADCS = 0X3F;     // Number of instruction cycles between conversions
  AD1CON1bits.ASAM  = 1;       // Sampling begins when the SAMP bit is set 
  AD1CON1bits.SSRC = 0b111;    // Auto conversion
  AD1CON2bits.SMPI = 0b0000;   // Interrupts at the completion of conversion for each sample/convert sequence
  AD1CON2bits.BUFM = 0;        // Buffer configured as one 16-word buffer
  AD1CON1bits.FORM = 0b00;     // Data output format set to integer
  AD1CON1bits.ADON = 1;        // Turn on ADC
  AD1CON1bits.SAMP = 1;        // Begin sampling
}


void UART_Init (void)
{
  /*
  /////
  //Unlock Registers
  asm volatile ( "MOV #OSCCON, w1 \n"
                 "MOV #0x46, w2   \n"
                 "MOV #0x57, w3   \n"
                 "MOV.b w2, [w1]  \n"
                 "MOV.b w3, [w1]  \n"
                 "BCLR OSCCON,#6");
  */
  //Configure Input Functions
    //Assign function of U1RX (in peripheral input register 18) to pin RP8
    RPINR18bits.U1RXR = 8;

  //Configure Output Functions
    //Assign pin RP9 (in peripheral output register 1) to function as U1TX ("3")
    RPOR4bits.RP9R = UART1_TX_FUNCTION;              
  /*
  //Lock Registers
  asm volatile ( "MOV #OSCCON, w1 \n"
                 "MOV #0x46, w2   \n"
                 "MOV #0x57, w3   \n"
                 "MOV.b w2, [w1]  \n"
                 "MOV.b w3, [w1]  \n"
                 "BSET OSCCON,#6");
  /////
  */
  //Initialize UxBRG to set baud rate
  //U1BRG = 25;               // PIC24FJ16: baud 9600;
  U1BRG = 12;	//19200
  //Set the number of data bits, stop bits and parity PDSEL<1:0>(UxMODE<2:1>) and STSL(UxMODE<0>)
  U1MODEbits.PDSEL = 00;    //8-bit, no parity
  U1MODEbits.STSEL = 0;     //One stop bit

  //Enable the UART module by setting the UARTEN (UxMODE<15>) bit
  U1MODEbits.UARTEN = 1;
  /*  Enable the transmission by setting the UTXEN (UxSTA<10>) bit. This will also set the g
      transmit interrupt flag UxTXIF bit. During the initialization, the interrupt request of 
      the UART module transmitter UxTXIF bit should be cleared. Also in the interrupt service 
      routine the interrupt request UxTXIF should be cleared
  */
  U1STAbits.UTXEN = 1; 
}
