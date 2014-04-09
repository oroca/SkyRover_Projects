

typedef unsigned int  size_t;
typedef char          prog_char;

//-- I/O
//
uint32_t PORTB;
uint32_t TWBR;
uint32_t PINB;
uint32_t TWINT;
uint32_t TWCR;
uint32_t TWDR;
uint32_t TWEA;
uint32_t TWEN;
uint32_t TWSTO;
uint32_t TWSTA;
uint32_t TWSR;
uint32_t PORTC;

uint32_t ADPS0;
uint32_t ADPS1;
uint32_t ADPS2;
uint32_t ADCSRA;

#ifndef F_CPU 
#define F_CPU 72000000L
#endif



#define _BV(x)  x


//-- EEPROM.ino
//
void eeprom_write_block(
  const void * __src,
  void * __dst,
  size_t __n)
{

}

void eeprom_read_block(
  void * __dst,
  const void * __src,
  size_t __n)
{

}