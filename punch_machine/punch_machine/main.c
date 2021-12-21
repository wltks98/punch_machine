
/*
 * GccApplication4.c
 *
 * Created: 2021-11-12 오후 2:02:33
 * Author : USER
 */ 
#define F_CPU 16000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
 #include <util/delay.h>
#include <avr/io.h>


 #include <avr/interrupt.h>
#define TRIG 0
#define ECHO 1
#define SOUND_VELOCITY 340UL

 
	 
volatile unsigned int max_score=0,score=0;		 
	 
unsigned char digit[10]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7c,~0x07,~0x7f,~0x67};
unsigned char fnd_sel[3]={0x01,0x02,0x04}; 	 
volatile int a=0;		


void calDistance() { //점수계산
	TCCR1B=0x03;
	
	PORTF = 0x00;
	_delay_us(10);
	PORTF = 0x01;
	_delay_us(10);
	PORTF = 0x00;
	
	while(!(PINF & 0x02));
	
	TCNT1 = 0x0000;
	
	while(PINF & 0x02);
	
	TCCR1B=0x00;
	
	unsigned int d1=(unsigned int)(SOUND_VELOCITY * (TCNT1*4/2)/10000);
	
	
	_delay_ms(200);
	TCCR1B=0x03;
	
	PORTF = 0x00;
	_delay_us(10);
	PORTF = 0x01;
	_delay_us(10);
	PORTF = 0x00;
	
	while(!(PINF & 0x02));
	
	TCNT1 = 0x0000;
	
	while(PINF & 0x02);
	
	TCCR1B=0x00;
	
	unsigned int d2=(unsigned int)(SOUND_VELOCITY * (TCNT1*4/2)/10000);
	
	
	
	score = (unsigned int)(d1 - d2)/0.2;
	score= (score*5)%1000;
	if(max_score<score){
		max_score=score;
		new_record();
	}
	
}

void display_fnd(int score)
{
	int i,fnd[3];
	fnd[2]=(score/100)%10;
	fnd[1]=(score/10)%10;
	fnd[0]=score%10;
	for(i=0; i<3; i++)
	{
		PORTA =digit[fnd[i]];
		PORTC=fnd_sel[i];
		_delay_ms(2);
		
	}
	
}
 
 
void display_fnd2(int score) 
{
	int i,fnd[3];
	fnd[2]=(score/100)%10;
	fnd[1]=(score/10)%10;
	fnd[0]=score%10;
	for(i=0; i<3; i++)
	{
		PORTD =digit[fnd[i]];
		PORTG=fnd_sel[i];
		_delay_ms(2);
		
	}
	
} 
 
 
void buzzer(int hz, int count) { 
	int i, j, ms; 
	ms = 500.0/hz; 
	for(i=0; i<count; i++) {
		 PORTB = 0x10; 
		 _delay_ms(ms); 
		 PORTB = 0x00;
		 _delay_ms(ms); 
		 }
		  
}

void ring_buzzer(){ //시작시 나오는 소리
	for(int i=0; i<20; i++) {
		buzzer(480, 12);
		buzzer(320, 8);
	}
	_delay_ms(500);
	for(int i=0; i<20; i++) {
		buzzer(480, 12);
		buzzer(320, 8);
	}
	_delay_ms(500);
	for(int i=0; i<20; i++) {
		buzzer(480, 12);
		buzzer(320, 8);
	}
	_delay_ms(500);
	for(int i=0; i<30; i++) {

		buzzer(247,8);
		buzzer(262,8);

	}
	_delay_ms(500);
	calDistance();
}



ISR(INT4_vect) //스위치1은 인터럽트4
{
	a=1;
	
	_delay_ms(10);
	
}

void new_record(){  //기록 갱신시 나오는 소리
	for(int i=0; i<10; i++) {
	buzzer(131,8);
	buzzer(147,8);
	buzzer(165,8);
	buzzer(175,8);
	buzzer(196,8);
	buzzer(220,8);
	buzzer(247,8);
	buzzer(262,8);
	}
}




int main(void)
{
	
	DDRF = 0x01;
	
	DDRD=0xff;	//C 포트는 fnd 데이터 신호
	DDRG=0x0f;	//G 포트는 fnd 선택 신호
	
	
	DDRA=0xff;	//C 포트는 fnd 데이터 신호
	DDRC=0x0f;	//G 포트는 fnd 선택 신호
	
	
	DDRE=0xcf;	//PE4(스위치1)은 입력
	
	EICRB=0x0a;	//INT4=falling edge
	EIMSK=0x30; //INT4 interrupt enable
	sei();
	DDRB = 0x10;
	while(1){
		
		if(a==1){
			a=0;
			ring_buzzer();
			
		}

		display_fnd(score);
		display_fnd2(max_score);
	}
	
	
}




