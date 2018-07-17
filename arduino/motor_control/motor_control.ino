#define FOSC 16000000 
#define BAUD 1000000  
unsigned int MYBURR=0;

int motor_goal = 0;
int ISR_cnt = 0;
int flag = 0;

void setup() {
  // put your setup code here, to run once:
  noInterrupts(); 
  TCCR1A=0; // 레지스터 0으로 초기화
  TCCR1B=0; // TCCR1A의 연결되어 사용된다
  TCNT1=0; // 
  OCR1A=15999; // TOP세팅이다. 카운트를 얼마나 할지를 설정해준다.
  TCCR1B |=(1<<WGM12);// WGM12에 1대입
  TCCR1B |=(1<<CS10);

  TIMSK1 |=(1<<OCIE1A);

  UCSR0A |= (1<<U2X0);
  UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
  UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
  MYBURR = (FOSC/(8*BAUD))-1;

  UBRR0H=(unsigned char)(MYBURR<<8);
  UBRR0L=(unsigned char)(MYBURR);
  sei();
  interrupts();

}

void USART_Transmit(unsigned char SEND)
{
  while(!(UCSR0A & (1 << UDRE0)));
  UDR0 =SEND;
}

int dynamixel_CF_movement(int degree, int fre, int cnt)
{
  int answer = 0;
  answer = (degree / 2) * (1 - cos((PI * cnt) / (fre * 10)));

  return answer;
}

void dynamixel_write_data(int pos)
{
  unsigned char dynamixel_packet[20]={0};
  
  int pos_EN = (pos/360.0)*4096;// 엔코더값
  dynamixel_packet[0]=0xFF; //Header
  dynamixel_packet[1]=0xFF; //Header
  dynamixel_packet[2]=0x01; //ID
  dynamixel_packet[3]=0x05; //Length
  dynamixel_packet[4]=0x03 ; //Instruction
  dynamixel_packet[5]=0x1E ; //Parameter 1{Adress}
  dynamixel_packet[6]=byte(pos_EN) ; //Parameter 2 //LOW
  dynamixel_packet[7]=byte((pos_EN & 0xFF00)>>8) ; //Parameter 3 //High // $연산

  int Check_sum =0;
 
  for(int i=2;i<8;i++)
  {
  Check_sum=Check_sum+dynamixel_packet[i];
  
  }

  dynamixel_packet[8]=~byte(Check_sum); //Check_sum
  
  for(int i = 0;i<9;i++)
  {
    USART_Transmit(dynamixel_packet[i]);
  }

}
  

  ISR(TIMER1_COMPA_vect){
    if(flag == 0) {
     ISR_cnt++;
    motor_goal = dynamixel_CF_movement(90, 300, ISR_cnt);
    dynamixel_write_data(motor_goal); 
    }
    if(ISR_cnt >= 3000)
    {
      flag = 1;
    }
    
  }


void loop() {
  // put your main code here, to run repeatedly:

}

