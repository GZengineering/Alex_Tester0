

#define SPI_TX_FUNCTION				7		//Define output Peripheral Pin Select function
#define SPI_CLK_FUNCTION			8		//Define output Peripheral Pin Select function


#define FLASH_DISABLE_TRIS				TRISAbits.TRISA4
#define FLASH_DISABLE_BIT				PORTAbits.RA4

#define SPI_CLK_TRIS					TRISBbits.TRISB4

#define MISO_TRIS						TRISBbits.TRISB5

#define MOSI_TRIS						TRISBbits.TRISB6



void UART_Init (void);
void FLASH_MEM_Init (void);
void UNBLOCK_MEMORY (void);
void JEDEC_ID (void);
unsigned int MANUF_ID(void);
void READ_STATUS_REG(void);
void _4KBYTE_ERASE(char address1,char address2,char address3);
void _32KBYTE_ERASE(char address1,char address2,char address3);
void _64KBYTE_ERASE(char address1,char address2,char address3);
void CHIP_ERASE(void);
unsigned int READ_BYTE(char address1,char address2,char address3);
unsigned int READ_BYTE_AUTO(char state);
void WRITE_ENABLE(void);
void WRITE_BYTE(char address1,char address2,char address3, char data);

