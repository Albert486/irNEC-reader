// Zakłócenia, kiedy pin reset nie jest podciągniętny przez rezystor 1k
// do VCC

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#define I bit_is_set(PIND, PD2)		// Pin z Irdą
#define O bit_is_clear(PIND, PD2)	// --..--

unsigned int timer;

unsigned char s9ms()	// Impuls startu trwajacy 9ms
{
	
	_delay_us(10);
	
	timer = 0;
	
	while((O) && (timer < 550))
	{
		timer++;
		_delay_us(2);
	}
	
	if((timer > 455) && (timer < 485))
		return 0;
	else
		return 1;
		
	
}

unsigned char s4ms()	// Przerwa
{
	
	_delay_us(10);
	
	timer = 0;
	
	while((I) && (timer < 340))
	{
		_delay_us(1);
		timer++;
	}
	
	if((timer > 245) && (timer < 275))
		return 0;
	else
		return 1;
		
	
}

unsigned char s560us()	// Sygnal przed bitem
{
	
	_delay_us(10);
	
	timer = 0;
	
	while((O) && (timer < 100))
	{
		_delay_us(1);
		timer++;
	}
	
	if((timer > 20) && (timer < 55))
		return 0;
	else
		return 1;
		
	
}

unsigned char bit()		// ============= Odczyt bitu === //
{
	
	_delay_us(10);
	
	timer = 0;
	
	while((I) && (timer < 2500))
	{
		_delay_us(1);
		timer++;
	}
	
	if(timer > 2400)		//	teoretycznie do 100ms
	{
		
		while((O) && timer < 300){_delay_us(1); timer++;}	// a tutaj wicy niz 100ms
		
		return 2;
		
	}
	
	else
	
		if(timer > 60)
		
			return 0;
			
		else
		
			if(timer <= 60)
		
				return 1;
			
	
}




	
void main()
{

	unsigned char i;
	unsigned char data[32], byte, nbyte, tmp;
	
	DDRD = 0;
	DDRB = 255;
	PORTD = 1;
	
	while(1)
	{

		while(I){}
		
		s9ms();
		s4ms();
		
		for (i = 0; i < 32; i++)
		{
			
			s560us();
			
			tmp = bit();
			
			if(tmp != 2)
				
				data[i] = tmp;
			
			else
				
				break;
				
		}
		
		if(tmp != 2)
			
			s560us();
			
		// Przeliczam...
		byte = 0;
		byte += data[16] * 1; // LSB (bit o najmniejszej wadze)
		byte += data[17] * 2;
		byte += data[18] * 4;
		byte += data[19] * 8;
		byte += data[20] * 16;
		byte += data[21] * 32;
		byte += data[22] * 64;
		byte += data[23] * 128;
		
		nbyte = 255;
		nbyte -= data[24] * 1;	// LSB
		nbyte -= data[25] * 2;
		nbyte -= data[26] * 4;
		nbyte -= data[27] * 8;
		nbyte -= data[28] * 16;
		nbyte -= data[29] * 32;
		nbyte -= data[30] * 64;
		nbyte -= data[31] * 128;
		
		// byte i nbyte muszą się równać
		// nbyte to negacja byte
		
		if((byte == 186) && (nbyte == 186)) // 246
		{
			
			PORTB |=  (1 << PB0);	// Dioda pali, jeżeli pasuje kod
			
			if(tmp == 2)
		
				PORTB |= (1 << PB1);	// Ta pali, jeżeli przycisk przytrzymany jest
			
			else
		
				PORTB &= ~(1 << PB1);
				
		}
			
			else
		
		if(byte == nbyte)
			
			PORTB  &=  ~(1 << PB0);
			
		
		
	}
	
}
