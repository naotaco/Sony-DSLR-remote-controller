#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#define F_CPU 1.0E6     //1MHz
#include <util/delay.h>
#define ADC_VREF 0x45   /* 0100 0101 vcc:10bit:PC5: ADCH */

int main(void){
	unsigned char i;
  	int th; 
  	ADMUX=ADC_VREF;

 	th=0;
	for(i=0;i<5;i++){		//4 times measurement
  		ADCSRA=0xC6;			//ADC enabled start clock/64
  		while (( ADCSRA & 0x10)==0){} // PC5 is ADC input 
  		th = th+ADCW;
  		ADCSRA=0x96; 
	}
 	th=th/5;		//average generated voltage
	DDRB=0xFF;
	int flag = 0;
	int timer1 = 0;
	int timer0= 0;
 	for(;;){
  		ADCSRA=0xC6;		//ADC enabled start clock/64
  		while (( ADCSRA & 0x10)==0){} 
  		ADMUX=0;		//avoid analog in order to activate digital output
  		if(ADCW < (th-350) ){
  			DDRC=0x20;
  			//PORTB=0x01;
			
  			if(flag ==0){
  				timer1++;
  				for(i=0;i<100;i++){}
  			}
  			for(i=0;i<4;i++){
  				PORTC=0x30;
  				
  				_delay_us(10);
				PORTC=0x10;
  				_delay_us(10);
  			}
  		}
  		else if(ADCW >= (th-300) ){
  			DDRC=0x20;
  			//PORTB=0x00;
  			if(flag ==1){
  				timer0++;
  				for(i=0;i<100;i++){}
  			}
  			for(i=0;i<20;i++){
  				PORTC=0x00;
  				_delay_us(10);
  				
  			}
  			
  		}
 		if( timer1 > 30){
 			flag = 1;
 			timer1 = 0;
 		}
 		else if(timer0 > 30){
 			flag = 0;
 			timer0 = 0;
 		}
 		
 		if(flag == 1){
 			PORTB=0x01;
 		}
 		else if(flag ==0){
 			PORTB = 0x00;
 		}

  		DDRC=0;		//avoid digital output
  		PORTC=0;		//three-state inputs
  		ADMUX=ADC_VREF;
  		ADCSRA=0x96;
   	}	
}
