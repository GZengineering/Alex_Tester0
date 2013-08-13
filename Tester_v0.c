#include <p24FJ16GA002.h>
#include "Tester_v0.h"
#include <stdio.h>

_CONFIG1(JTAGEN_OFF & FWDTEN_OFF);
_CONFIG2(POSCMOD_NONE & OSCIOFNC_OFF & FNOSC_FRC);

int main() 
{
	////Setup////
	CHARGE_EN_RELAY_TRIS		= 0;			//Output
	DISCHARGE_EN_PMOS_TRIS		= 0;     		//Output
	CHARGE_EN_PMOS_TRIS 		= 0;        	//Output
	FLASH_DISABLE_TRIS			= 0;			//Output	   
	VPACK_OUT_TRIS				= 1;			//Input 
	VPACK_IN_TRIS				= 1;			//Input
	VSUPPLY_TRIS				= 1;			//Input 
	VSHUNT_CHARGE_AMP_TRIS 		= 1;    		//Input
	VSHUNT_DISCHARGE_AMP_TRIS	= 1;            //Input
	
	DISCHARGE_EN_PMOS_BIT 		= 0;            
	CHARGE_EN_PMOS_BIT 			= 0;
	CHARGE_EN_RELAY_BIT 		= 0;
	FLASH_DISABLE_BIT			= 1;			//Disable Flash
	
	MEM_ADDR_STRUCT Mem_Addr_Struct;
	Mem_Addr_Struct.Mem_Addr_Total = 0; 
	
	unsigned char DataHigh		= 0;		//Buffer to store high data bits
	unsigned char DataLow			= 0;		//Buffer to store low data bits
	
	
	char ReadRXBuff;
	int count				= 0;
	long int TIMERCOUNT		= 0;				//Keep track of the number of times the timer overflows
	float temp;
	float Vref				= 2.5;
	double TIME				= 0;
	double SampleInterval	= 3;			//In seconds
	double Vambient;
	
	Timer1_Init();                         //Initialize Timer 1
	ADC_Init();                            //Initialize ADC
	UART_Init();
	
	printf("#Press L to begin light test (x will cancel)\n");
	
	while(1)
	{
	   temp = 0;								// Clear temp value
	   ReadRXBuff = U1STAbits.URXDA;
	   if (ReadRXBuff)
	   {
	     temp = U1RXREG;  
	     if (temp == 76|| temp == 108)    	    //Press L or l to start Nomad Test
	     {
	       	AD1CHSbits.CH0SA  = VSHUNT_CHARGE_AMP_ADC_PIN;;		//Set ADC pin to next sample ambient voltage  **AMBIENT_ADC_PIN = 0x0C = 0b1100
			temp = ADC1BUF0 & 0x03FF;           // Done to clear out value 					**Why specifically use ADC1BUF0, why not ADC1BUF1, or ADC1BUFC? 
			//AMBIENTPOWER_EN_BIT= 1;
	       	//AMBIENT_CODE_EN    = 1;
			count 	  = 0;
			printf("#Begin Light Test\n");                    
	     }
	     
	     if (temp == 88 || temp == 120)      //Press x or X to idle
	     {
		AMBIENTPOWER_EN_BIT	= 0;
	    AMBIENT_CODE_EN		= 0;
		count				= 0; 
		printf("#Idle\n");         
	     }
	   }
	
	   if (AMBIENT_CODE_EN && IFS0bits.T1IF == 1) 
	   { 
	    IFS0bits.T1IF = 0;						// Reset the Timer 1 flag  
		TIMERCOUNT++;
		temp = TIMERCOUNT; 
		TIME = temp*SampleInterval/60;			// Mins
		printf("%.2f",TIME);
	   	if (TIME < 0)							//If run-time overflows, go idle
	   	{
	         printf("#Run-Time Overflow");
	         AMBIENT_CODE_EN = 0;                                      
	   	}            
	   //Calculate the ambient Voltage//           
	       temp = ADC1BUF0 & 0x03FF;           			// Store the 10-bit ADC value
	       Vambient = (Vref*(temp/1024));
	       printf("	%.2f\n",Vambient);
	
	       if (Vambient < 0.01)
	       {
	         count++;
		  if (count > 200)
		  {
			AMBIENTPOWER_EN_BIT= 0;
			AMBIENT_CODE_EN = 0;
		  }			
	       }
	       else count = 0;	  
	   }
	}

}

void Timer1_Init (void)
{
  T1CONbits.TGATE = 0;         // No gated time accumulation
  T1CONbits.TSYNC = 0;         // Do not sync to an external clock
  T1CONbits.TCS = 0;           // Internal Ocillator Fosc/2
  T1CONbits.TCKPS = 0b11;      // Prescalar register 
  PR1 = 0xB6DB;                // Timer will count to this value before flag is set FFFF=4.2s,B6DB=3s
  IFS0bits.T1IF = 0;           // Timier 1 interrupt flag is cleared
  //IEC0bits.T1IE = 1;         // Enable Timer 1 Interrupt
  T1CONbits.TON = 1;           // Turn on Timer 1
}


/**
I have no idea how to know what to set and what values they need to be. 
Why not set every bit of each control register?
*/
void ADC_Init (void)
{
  VSHUNT_CHARGE_AMP_ADC_EN_BIT		= 0;	// Pin set as analog input, A/D samples voltage
  VSHUNT_DISCHARGE_AMP_ADC_EN_BIT	= 0;  	// Pin set as analog input, A/D samples voltage
  VPACK_OUT_ADC_EN_BIT			= 0; 	// Pin set as analog input, A/D samples voltage
  VPACK_IN_ADC_EN_BIT			= 0;	// Pin set as analog input, A/D samples voltage
  VSUPPLY_EN_BIT				= 0;	// Pin set as analog input, A/D samples voltage
  AD1CHSbits.CH0NA  = 0;       // Negative muxA value is Vss 									/*	-- datasheet says setting this value is VR-	 */
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

  AD1PCFGbits.PCFG4 = 1;      //Disable Analog for AN4
  AD1PCFGbits.PCFG5 = 1;      //Disable Analog for AN5

  /////
  //Unlock Registers
  asm volatile ( "MOV #OSCCON, w1 \n"
                 "MOV #0x46, w2   \n"
                 "MOV #0x57, w3   \n"
                 "MOV.b w2, [w1]  \n"
                 "MOV.b w3, [w1]  \n"
                 "BCLR OSCCON,#6");

  //Configure Input Functions
    //Assign function of U1RX (in peripheral input register 18) to pin RP3
    RPINR18bits.U1RXR = 3;

  //Configure Output Functions
    //Assign pin RP2 (in peripheral output register 1) to function as U1TX ("3")
    RPOR1bits.RP2R = UART1_TX_FUNCTION;              

  //Lock Registers
  asm volatile ( "MOV #OSCCON, w1 \n"
                 "MOV #0x46, w2   \n"
                 "MOV #0x57, w3   \n"
                 "MOV.b w2, [w1]  \n"
                 "MOV.b w3, [w1]  \n"
                 "BSET OSCCON,#6");
  /////

  //Initialize UxBRG to set baud rate
  U1BRG = 25;               // PIC24FJ16: baud 9600; PIC24FJ64: baud 4800
  //Set the number of data bits, stop bits and parity PDSEL<1:0>(UxMODE<2:1>) and STSL(UxMODE<0>)
  U1MODEbits.PDSEL = 00;    //8-bit, no parity
  U1MODEbits.STSEL = 0;     //One stop bit


  /*  If transmit interrupts are desired, set the UxTXIE control bit in the corresponding 
      interrupt enable control register (IEC). Specify the interrupt priority usin the 
      UxTXIP<2:0> control bits in the corresponding interrupt priority control register (IPC). 
      Select the transmit interrupt mode by writing the UTXISEL (UxMODE<15>) bit.
  */

  //Enable the UART module by setting the UARTEN (UxMODE<15>) bit
  U1MODEbits.UARTEN = 1;
  /*  Enable the transmission by setting the UTXEN (UxSTA<10>) bit. This will also set the g
      transmit interrupt flag UxTXIF bit. During the initialization, the interrupt request of 
      the UART module transmitter UxTXIF bit should be cleared. Also in the interrupt service 
      routine the interrupt request UxTXIF should be cleared
  */
  U1STAbits.UTXEN = 1; 


}


