//Tester_v0.h

#define VPACK_IN_TRIS				TRISAbits.TRISA1
#define VPACK_IN_ADC_EN_BIT			AD1PCFGbits.PCFG1		//Corresponding Analog port
#define VPACK_IN_ADC_PIN 			0x01

#define VPACK_OUT_TRIS				TRISBbits.TRISB15
#define VPACK_OUT_ADC_EN_BIT		AD1PCFGbits.PCFG9		//Corresponding Analog port
#define VPACK_OUT_ADC_PIN 			0x09

#define UART1_TX_FUNCTION			3		//Define output Peripheral Pin Select function

#define CHARGE_EN_RELAY_TRIS		TRISBbits.TRISB11
#define CHARGE_EN_RELAY_BIT			PORTBbits.RB11

#define CHARGE_EN_PMOS_TRIS			TRISBbits.TRISB10
#define CHARGE_EN_PMOS_BIT			PORTBbits.RB10

#define DISCHARGE_EN_PMOS_TRIS			TRISAbits.TRISA2		
#define DISCHARGE_EN_PMOS_BIT			PORTAbits.RA2				

#define VSHUNT_CHARGE_AMP_TRIS				TRISBbits.TRISB14
#define VSHUNT_CHARGE_AMP_ADC_EN_BIT		AD1PCFGbits.PCFG10		//Corresponding Analog port
#define VSHUNT_CHARGE_AMP_ADC_PIN 			0x0A

#define VSHUNT_DISCHARGE_AMP_TRIS			TRISBbits.TRISB13
#define VSHUNT_DISCHARGE_AMP_ADC_EN_BIT		AD1PCFGbits.PCFG11		//Corresponding Analog port
#define VSHUNT_DISCHARGE_AMP_ADC_PIN 		0x0B

#define VSUPPLY_TRIS				TRISBbits.TRISB12
#define VSUPPLY_EN_BIT				AD1PCFGbits.PCFG12		//Corresponding Analog port
#define VSUPPLY_ADC_PIN 			0x0C

#define INCOMING_UART_INPUT				U1STAbits.URXDA
#define UART_INPUT						U1RXREG

#define FCY   40000000L  //define your instruction frequency, FCY = FOSC/2
  
#define CYCLES_PER_MS ((unsigned long)(FCY * 0.001))        //instruction cycles per millisecond
#define CYCLES_PER_US ((unsigned long)(FCY * 0.000001))   	//instruction cycles per microsecond

#define DELAY_MS(ms)  __delay32(CYCLES_PER_MS * ((unsigned long) ms));   //__delay32 is provided by the compiler, delay some # of milliseconds
#define DELAY_US(us)  __delay32(CYCLES_PER_US * ((unsigned long) us));    //delay some number of microseconds
  

void Timer1_Init (void);
void ADC_Init (void);
void UART_Init (void);

//a word on the PIC24 is 24 bits. A char is 8 bits. A long is 24 bits? A full memory address will be 24 bits, broken into 3 chars.
typedef struct {
  union  {
	struct  {
	  unsigned char Mem_Addr_Lo;
	  unsigned char Mem_Addr_Mid;
	  unsigned char Mem_Addr_Hi;
	};
	struct  {
	  long int Mem_Addr_Total;
	};
  };
} MEM_ADDR_STRUCT;
