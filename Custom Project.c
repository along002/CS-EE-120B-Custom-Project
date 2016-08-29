/*
 * 
 *
 * Created: 8/11/2016 7:49:45 PM
 *  Author: along002
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\along002\Documents\Atmel Studio\6.2\GccApplication1\GccApplication1\bit.h"
#include "C:\Users\along002\Documents\Atmel Studio\6.2\GccApplication1\GccApplication1\timer.h"
#include "C:\Users\along002\Documents\Atmel Studio\6.2\GccApplication1\GccApplication1\io.c"
#include "C:\Users\along002\Documents\Atmel Studio\6.2\GccApplication1\GccApplication1\keypad.h"
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;
struct wall {
	unsigned char xposition;
	unsigned char yposition0;	
	unsigned char yposition1;
	unsigned char yposition2;
	unsigned char yposition3;
};
struct wall walls[100];
struct wall fire1;
struct wall player2_fire1;
void car_top(){ //http://solar-blogg.blogspot.com/2009/02/displaying-custom-5x8-characters-on.html
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 0); //Set character RAM address to zero
	LCD_WriteData(0b10000); //Define our first character
	LCD_WriteData(0b11100);
	LCD_WriteData(0b11110);
	LCD_WriteData(0b11111);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);
}
//LCD_WriteData(0x00)
void car_bot(){
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 8); //Set character RAM address to zero
	LCD_WriteData(0x00); //Define our first character
	LCD_WriteData(0x00); //row2
	LCD_WriteData(0x00); //row3
	LCD_WriteData(0x00); //row4
	LCD_WriteData(0b10000); //row5
	LCD_WriteData(0b11100); //row6
	LCD_WriteData(0b11110); //row7
	LCD_WriteData(0b11111); //row8
}
//LCD_WriteData(0x01)
void obstacle3(){
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 16); //Set character RAM address to zero
	LCD_WriteData(0x1F); //Define our first character
	LCD_WriteData(0x1F); //row2
	LCD_WriteData(0x1F); //row3
	LCD_WriteData(0x1F); //row4
	LCD_WriteData(0x00); //row5
	LCD_WriteData(0x00); //row6
	LCD_WriteData(0x00); //row7
	LCD_WriteData(0x00); //row8
}
//LCD_WriteData(0x02)
void obstacle4(){
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 24); //Set character RAM address to zero
	LCD_WriteData(0x00); //Define our first character
	LCD_WriteData(0x00); //row2
	LCD_WriteData(0x00); //row3
	LCD_WriteData(0x00); //row4
	LCD_WriteData(0x1F); //row5
	LCD_WriteData(0x1F); //row6
	LCD_WriteData(0x1F); //row7
	LCD_WriteData(0x1F); //row8
}
//LCD_WriteData(0x03)
void car_top_obstacle(){
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 32);
	LCD_WriteData(0b10000); //Define our first character
	LCD_WriteData(0b11100); //row2
	LCD_WriteData(0b11110); //row3
	LCD_WriteData(0b11111); //row4
	LCD_WriteData(0b11111); //row5
	LCD_WriteData(0b11111); //row6
	LCD_WriteData(0b11111); //row7
	LCD_WriteData(0b11111); //row8	
}
//LCD_WriteData(0x04)
void car_bot_obstacle(){
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 40);
	LCD_WriteData(0b11111); //Define our first character
	LCD_WriteData(0b11111); //row2
	LCD_WriteData(0b11111); //row3
	LCD_WriteData(0b11111); //row4
	LCD_WriteData(0b10000); //row5
	LCD_WriteData(0b11100); //row6
	LCD_WriteData(0b11110); //row7
	LCD_WriteData(0b11111); //row8
}
//LCD_WriteData(0x05)
void broken_obs(){
	LCD_Cursor(0);
	LCD_WriteCommand(0x40 + 48);
	LCD_WriteData(0b11101); //Define our first character
	LCD_WriteData(0b01111); //row2
	LCD_WriteData(0b10111); //row3
	LCD_WriteData(0b11011); //row4
	LCD_WriteData(0b01101); //row5
	LCD_WriteData(0b10011); //row6
	LCD_WriteData(0b01111); //row7
	LCD_WriteData(0b10111); //row8
}
//LCD_WriteData(0x06)
void Generate_Terrain(){
	for(unsigned char i = 0; i < 100; ++i){
		walls[i].xposition = (i * 2) + 16;
	}
	for(unsigned char i = 0; i < 100; ++i){
		int r1 = (rand() % 10);// generate random numbers
		int r2 = (rand() % 10);
		int r3 = (rand() % 10);
		int r4 = (rand() % 10);
		
		r1 = (r1 > 1) ? 1 : 0;
		r2 = (r2 > 1) ? 0 : 1;
		r3 = (r3 > 1) ? 1 : 0;
		r4 = (r4 > 1) ? 1 : 0;
		
		int r5 = (rand() % 100);
		int r6 = (rand() % 100);
		if(r5 > 1 && r5 < 20){
			r1 = 3;
			r2 = 3;
		}
		if(r6 > 1 && r6 < 20){
			r3 = 3;
			r4 = 3;
		}
		walls[i].yposition0 = r1 + '0';
		walls[i].yposition1 = r2 + '0';
		walls[i].yposition2 = r3 + '0';
		walls[i].yposition3 = r4 + '0';
					
		if(walls[i].yposition0 == '1' && walls[i].yposition1 == '1' && walls[i].yposition2 == '1'
				&& walls[i].yposition3 == '1'){
			walls[i].yposition2 = '0';
		}/*
		if(walls[i].yposition0 == '0' && walls[i].yposition1 == '0' && walls[i].yposition2 == '0'
				&& walls[i].yposition3 == '0'){
		walls[i].yposition0 = '1';
		walls[i].yposition3 = '1';
		}*/
	}
}

int cnt = -16;
unsigned char pause1,playerpos,SameFlag,PauseFlag,delete2,Lost_Animation,Lost_Animation_Elapsed,Fire_Button,playerpos2,player2_fire = 0;
unsigned long Seed;
int x = 0;
int period1 = 3;
enum States0{wait0,pausewait,wait,up,down,fire,pause,player2_down,player2_up,player2_fire_button,soft_reset} State0;
int button(int state){
	switch(State0){
		case wait0:
			pause1 = 4;
			++Seed;
			if(~PINB & 0x04){				
				State0 = pausewait;
			}
			else{
				State0 = wait0;
				}
		break;
		
		case pausewait:
			Fire_Button = 0;
			if(~PINB & 0x04){
				State0 = pausewait;
			}
			else{
				srand(Seed);
				pause1 = 0;
				LCD_ClearScreen();
				LCD_Cursor(1);
				LCD_WriteData(0x00);
				Generate_Terrain();
				State0 = wait;	
			}
			break;
			
		case wait:
			if (~PINB & 0x01 && pause1 != 3){
				if(playerpos < 3){
					playerpos++;
					SameFlag = 1;
				}
				State0 = down;
			}
			else if(~PINB & 0x02 && pause1 != 3){
				if(playerpos > 0){
					playerpos--;
					SameFlag = 1;
				}
				State0 = up;
			}
			else if(~PINB & 0x04){
				if(pause1 == 0){
					pause1 = 1;
				}
				else if(pause1 == 1){
					pause1 = 0;
				}
				else if(pause1 == 3){
					cnt = 0;
					pause1 = 0;
					playerpos = 0;
					SameFlag = 0;
					PauseFlag = 0;
					delete2 = 0;
					Lost_Animation_Elapsed = 0;
					Fire_Button = 0;
					playerpos2 = 0;
					player2_fire = 0; 
					Seed += 20;
					srand(Seed);
					Generate_Terrain();
					LCD_Cursor(1);
					LCD_WriteData(0x00);			
				}
				else{}
				State0 = pause;
			}
			else if(~PINB & 0x08){
					fire1.xposition = 2;
					Fire_Button = 0;
					if(playerpos == 3 || playerpos == 2){
						fire1.yposition3 = '3';
						fire1.yposition2 = '3';
						fire1.yposition1 = '0';
						fire1.yposition0 = '0';
					}
					else if(playerpos == 0 || playerpos == 1){
						fire1.yposition3 = '0';
						fire1.yposition2 = '0';
						fire1.yposition1 = '3';
						fire1.yposition0 = '3';
					}
					if(fire1.xposition == player2_fire1.xposition || fire1.xposition - 1 == player2_fire1.xposition){
						if(fire1.yposition0 == player2_fire1.yposition0){
							player2_fire1.yposition0 = '0';
							player2_fire1.yposition1 = '0';
							//player2_fire1.yposition2 = '0';
							//player2_fire1.yposition3 = '0';
						}
						else if(fire1.yposition2 == player2_fire1.yposition2){
							player2_fire1.yposition2 = '0';
							player2_fire1.yposition3 = '0';
							//player2_fire1.yposition1 = '0';
							//player2_fire1.yposition0 = '0';
						}
						else{}
						player2_fire1.xposition = 0;
					}					
				state = fire;
			}
			else if(GetKeypadKey() == '4'){
				playerpos2 = 1;
				State0 = player2_down;
			}			
			else if(GetKeypadKey() == '7'){
				playerpos2 = 0;
				State0 = player2_up;
			}
			else if(GetKeypadKey() == '8'){
				player2_fire = 1;
				State0 = player2_fire_button;
			}
			else if (GetKeypadKey() == 'D'){
					LCD_ClearScreen();
					cnt = 0;
					pause1 = 0;
					playerpos = 0;
					SameFlag = 0;
					PauseFlag = 0;
					delete2 = 0;
					Lost_Animation_Elapsed = 0;
					Fire_Button = 0;
					playerpos2 = 0;
					player2_fire = 0;
					Seed += 20;
					srand(Seed);
					Generate_Terrain();
					LCD_Cursor(1);
					LCD_WriteData(0x00);
					State0 = soft_reset;
			}
			else{State0 = wait;}
		break;
		
		case player2_fire_button:
			State0 = (GetKeypadKey() == '8') ? player2_fire_button : wait;
		break;
		case player2_down:
			State0 = (GetKeypadKey() == '4') ? player2_down : wait;
		break;
		
		case soft_reset:
			State0 = (GetKeypadKey() == 'D') ? soft_reset : wait;
		break;
		
		case player2_up:
		State0 = (GetKeypadKey() == '4') ? player2_up : wait;
		break;
		
		case fire:
			State0 = (~PINB & 0x08) ? fire : wait;
		break;
			
		case up:
			State0 = (~PINB & 0x01) ? up : wait;
		break;
		
		case down:
			State0 = (~PINB & 0x02) ? down : wait;
		break;
		
		case pause:
			State0 = (~PINB & 0x04) ? pause : wait;
		break;
		
		default: break;	
	}
	return state;
}

enum States{Init,Dec} State;
int realtsk(int state){
	switch(State){
		case Init:
			if(pause1 == 0){
				State = Dec;
			}
		break;
		
		case Dec:
		if(x < 100){
			++x;
		}
		if(x == 10){
			period1 = 2;
		}
		if(x == 20){
			period1 = 1;
		}
		if(x % period1 == 0){
				if(player2_fire == 1){
					player2_fire = 0;
					player2_fire1.xposition = 15;
					if(playerpos2 == 0){
						player2_fire1.yposition0 = '3';
						player2_fire1.yposition1 = '3';
						player2_fire1.yposition2 = '0';
						player2_fire1.yposition3 = '0';
					}
					else if(playerpos2 == 1){
						player2_fire1.yposition0 = '0';
						player2_fire1.yposition1 = '0';
						player2_fire1.yposition2 = '3';
						player2_fire1.yposition3 = '3';						
					} 
					else{}
				}
				if(player2_fire1.xposition > 0 && player2_fire1.xposition < 16){
					player2_fire1.xposition--;
					if(player2_fire1.xposition == 0){
						player2_fire1.xposition = 17;
						if(player2_fire1.yposition2 == '3'){
							LCD_Cursor(17);
							LCD_WriteData(' ');	
						}
						else if(player2_fire1.yposition0 == '3'){
							LCD_Cursor(1);
							LCD_WriteData(' ');
						}
						else{}
					}
				}
				++cnt;
				if(fire1.xposition >= 1 && fire1.xposition < 16){
					fire1.xposition++;
				}
				for(unsigned char j = 0; j < 100; ++j){
					if(walls[j].xposition > 0){
						walls[j].xposition--;
					}				
					if(walls[j].xposition == 1){
						if(walls[j].yposition0 != '0' && playerpos == 0){
							pause1 = 3;
						}
						else if(walls[j].yposition1 != '0' && playerpos == 1){
							pause1 = 3;
						}
						else if(walls[j].yposition2 != '0' && playerpos == 2){
							pause1 = 3;
						}
						else if(walls[j].yposition3 != '0' && playerpos == 3){
							pause1 = 3;
						}
						else{}
					}
					if(player2_fire1.xposition == 1){
						if(player2_fire1.yposition0 == '3' && (playerpos == 0 || playerpos == 1)){
							pause1 = 3;
						}
						else if(player2_fire1.yposition3 == '3' && (playerpos == 2 || playerpos == 3)){
							pause1 = 3;
						}
						else{}
					}
					if(j % 2){
						delete2 = 1;
					}
					if(fire1.xposition == walls[j].xposition && fire1.xposition > 1 || fire1.xposition - 1 == walls[j].xposition){
						if(fire1.yposition0 == '3' && walls[j].yposition0 == '3'){
							walls[j].yposition0 = '0';
							walls[j].yposition1 = '0';
							//LCD_Cursor(fire1.xposition);
							//LCD_WriteData(0x78);
						}
						else if(fire1.yposition3 == '3' && walls[j].yposition3 == '3'){
							walls[j].yposition2 = '0';
							walls[j].yposition3 = '0';
							//LCD_Cursor(fire1.xposition + 16);
							//LCD_WriteData(0x78);							
						}	
						else if(fire1.yposition0 == '3' && walls[j].yposition0 == '0' && walls[j].yposition1 == '0'){
							
						}
						else if(fire1.yposition3 == '3' && walls[j].yposition2 == '0' && walls[j].yposition3 == '0'){
							
						}
						else{fire1.xposition = 17;}				
					}
					if(fire1.xposition == player2_fire1.xposition || fire1.xposition - 1 == player2_fire1.xposition){
						if(fire1.yposition0 == player2_fire1.yposition0){
							player2_fire1.yposition0 = '0';
							player2_fire1.yposition1 = '0';
							//player2_fire1.yposition2 = '0';
							//player2_fire1.yposition3 = '0';
						}
						else if(fire1.yposition2 == player2_fire1.yposition2){
							player2_fire1.yposition2 = '0';
							player2_fire1.yposition3 = '0';
							//player2_fire1.yposition1 = '0';
							//player2_fire1.yposition0 = '0';
						}
						else{}
						player2_fire1.xposition = 0;
					}
					if(walls[j].xposition == player2_fire1.xposition){
						if(walls[j].yposition0 != '0' || walls[j].yposition1 != '0' && player2_fire1.yposition0 == '3'){
							walls[j].yposition0 = '0';
							walls[j].yposition1 = '0';
						}
						else if(walls[j].yposition3 != '0' || walls[j].yposition2 && player2_fire1.yposition3 == '3'){
							walls[j].yposition3 = '0';
							walls[j].yposition2 = '0';							
						}
						else{}
					}
				}
				Lost_Animation = (Lost_Animation == 1) ? 0 : 1;
				if(pause1 == 3){
					Lost_Animation_Elapsed++;
				}
				}
		break;
		
		
		default: break;
	}	
	return state;
}
enum States2{game,gameover,crash} State2;
int displaytsk(int state){
	switch(State2){
		case game:
		if(pause1 == 0){
			//LCD_Cursor(16);
			//LCD_WriteData(cnt + '0');
			if(player2_fire1.xposition > 0 && player2_fire1.xposition < 16){
				if(player2_fire1.yposition0 == '3'){
					unsigned char k = 0;
					for(int l = 0; l < 100; ++l){
						if(walls[l].xposition == (player2_fire1.xposition + 1) && walls[l].yposition0 != '0'){
							k = 1;
						}
					}
					if(k == 0){
						LCD_Cursor(player2_fire1.xposition + 1);
						LCD_WriteData(' ');
					}
					LCD_Cursor(player2_fire1.xposition);
					LCD_WriteData(0xDB);
				}
				else if(player2_fire1.yposition2 == '3'){
					unsigned char k = 0;
					for(int l = 0; l < 100; ++l){
						if(walls[l].xposition == (player2_fire1.xposition + 1) && walls[l].yposition3 != '0'){
							k = 1;
						}
					}
					if(k == 0){					
						LCD_Cursor(player2_fire1.xposition + 17);
						LCD_WriteData(' ');
					}
					LCD_Cursor(player2_fire1.xposition + 16);
					LCD_WriteData(0xDB);
				}
			}
			if(playerpos2 == 0){
				LCD_Cursor(32);
				LCD_WriteData(' ');
				LCD_Cursor(16);
				LCD_WriteData(0xE0);
			}
			if(playerpos2 == 1){
				LCD_Cursor(16);
				LCD_WriteData(' ');
				LCD_Cursor(32);
				LCD_WriteData(0xE0);
			}
			if(fire1.xposition >= 1 && fire1.xposition < 16 && fire1.yposition1 == '3'){
				if(fire1.xposition != 2){
					LCD_Cursor(fire1.xposition - 1);
					LCD_WriteData(' ');
				}
				LCD_Cursor(fire1.xposition);
				LCD_WriteData(0xA5);
			}
			else if(fire1.xposition >= 1 && fire1.xposition < 16 && fire1.yposition3 == '3'){
				if(fire1.xposition != 2){
					LCD_Cursor(fire1.xposition + 15);
					LCD_WriteData(' ');
				}
				LCD_Cursor(fire1.xposition + 16);
				LCD_WriteData(0xA5);
			}
			if(SameFlag != 0){//player position
				LCD_Cursor(1);
				LCD_WriteData(' ');
				LCD_Cursor(17);
				LCD_WriteData(' ');
				if(playerpos == 0){
					LCD_Cursor(1);
					LCD_WriteData(0x00);
				}
				else if(playerpos == 1){
					LCD_Cursor(1);
					LCD_WriteData(0x01);
				}
				else if(playerpos == 2){
					LCD_Cursor(17);
					LCD_WriteData(0x00);
				}
				else if(playerpos == 3){
					LCD_Cursor(17);
					LCD_WriteData(0x01);
				}
				SameFlag = 0;
			}
			for(int i = 0; i < 100; ++i){ // top display
				if(walls[i].xposition <= 14 && walls[i].xposition > 1){
					unsigned char k = 0;
					if(walls[i].xposition + 1 == player2_fire1.xposition && player2_fire1.yposition0 == '3'){
						k = 1;
					}
					if(k == 0){
						if(walls[i].yposition0 == '0' && walls[i].yposition1 == '0'){
							
						}
						else{
							LCD_Cursor(walls[i].xposition + 1);
							LCD_WriteData(' ');
						}
					}
					LCD_Cursor(walls[i].xposition);
					if(walls[i].yposition0 == '1' && walls[i].yposition1 == '1'){
						LCD_WriteData(0xFF);
					}
					else if(walls[i].yposition0 == '1' && walls[i].yposition1 == '0'){
						LCD_WriteData(0x02);
					}
					else if(walls[i].yposition0 == '0' && walls[i].yposition1 == '0'){
						/*
						unsigned char k = 0;
						if(player2_fire1.xposition == walls[i].xposition && player2_fire1.yposition0 == '3'){
							k = 1;
						}
						if(k == 0){
							LCD_WriteData(' ');
						}*/
					}
					else if(walls[i].yposition0 == '0' && walls[i].yposition1 == '1'){
						LCD_WriteData(0x03);
					}
					else if(walls[i].yposition0 == '3'){
						LCD_WriteData(0x06);
					}
					else{LCD_DisplayString(1,"Error Top");}
				}
				if((walls[i].xposition == fire1.xposition) && (fire1.yposition0 == walls[i].yposition0)){
					walls[i].yposition0 = '0';
					walls[i].yposition1 = '0';
				}
			}
			
			for(int i = 0; i < 100; ++i){ // bottom display
				if(walls[i].xposition <= 14 && walls[i].xposition > 1){
					
					unsigned char k = 0;
					if(walls[i].xposition + 1 == player2_fire1.xposition && player2_fire1.yposition3 == '3'){
						k = 1;
					}
					if(k == 0){
						
						if(walls[i].yposition3 == '0' && walls[i].yposition2 == '0'){
							
						}
						else{
							LCD_Cursor(walls[i].xposition + 17);
							LCD_WriteData(' ');
						}
					}					
					LCD_Cursor(walls[i].xposition + 16);
					if(walls[i].yposition2 == '1' && walls[i].yposition3 == '1'){
						LCD_WriteData(0xFF);
					}
					else if(walls[i].yposition2 == '1' && walls[i].yposition3 == '0'){
						LCD_WriteData(0x02);
					}
					else if(walls[i].yposition2 == '0' && walls[i].yposition3 == '0'){
						/*
						unsigned char k = 0;
						if(player2_fire1.xposition == walls[i].xposition && player2_fire1.yposition2 == '3'){
							k = 1;
						}
						if(k == 0){
							LCD_WriteData(' ');
						}*/
					}
					else if(walls[i].yposition2 == '0' && walls[i].yposition3 == '1'){
						LCD_WriteData(0x03);
					}
					else if(walls[i].yposition3 == '3'){
						LCD_WriteData(0x06);
					}					
					else{LCD_DisplayString(1,"Error Bot");}
				}
				if((walls[i].xposition == fire1.xposition) && (fire1.yposition3 == walls[i].yposition3)){
					walls[i].yposition2 = '0';
					walls[i].yposition3 = '0';
				}				
			}
			if(delete2 == 1){
				LCD_Cursor(2);
				LCD_WriteData(' ');
				LCD_Cursor(18);
				LCD_WriteData(' ');
				delete2 = 0;				
			}
		}			
		else if(pause1 == 3){
			State2 = crash;
		}
		else if(pause1 == 4){
			if(PauseFlag == 0){
				LCD_DisplayString(1,"Press Pause     To Start");
				PauseFlag = 1;
			}
		}
		else{}
		break;
		
		case crash:
			if(playerpos == 0 || playerpos == 1){
				LCD_Cursor(1);
				LCD_WriteData(' ');
				LCD_Cursor(1);
				if(Lost_Animation == 0){
					LCD_WriteData('X');
				}
				else{
					LCD_WriteData('*');
				}
			}
			if(playerpos == 2 || playerpos == 3){
				LCD_Cursor(17);
				LCD_WriteData(' ');
				LCD_Cursor(17);
				if(Lost_Animation == 0){
					LCD_WriteData('X');
				}
				else{
					LCD_WriteData('*');
				}
			}
			if(Lost_Animation_Elapsed == 10){
				LCD_DisplayString(1,"Game Over");
				LCD_Cursor(17);
				LCD_WriteData('S');
				LCD_WriteData('c');
				LCD_WriteData('o');
				LCD_WriteData('r');
				LCD_WriteData('e');
				LCD_WriteData(' ');
				LCD_WriteData('=');
				LCD_WriteData(' ');
				
				unsigned char score1 = 0;
				score1 = cnt / 1000;
				cnt = cnt - score1 * 1000;
				unsigned char score2 = 0;
				score2 = cnt / 100;
				unsigned char score3 = 0;
				cnt = cnt - score2 * 100;
				score3 = cnt / 10;
				unsigned char score4 = 0;
				cnt = cnt - score3 * 10;
				score4 = cnt;
				if(score1 != 0){
					LCD_WriteData(score1 + '0');
				}
				if(score2 != 0){
					LCD_WriteData(score2 + '0');
				}
				if(score3 != 0){
					LCD_WriteData(score3 + '0');
				}
				if(score4 != 0){
					LCD_WriteData(score4 + '0');
				}
				LCD_WriteData('!');	
				State2 = gameover;						
			}
		break;
		
		case gameover:
		if(pause1 == 0){
			LCD_ClearScreen();
			State2 = game;
		}
		break;
		
		default: break;
	}
	return state;
}

int main()
{
// Set Data Direction Registers
// Buttons PORTA[0-7], set AVR PORTA to pull down logic
DDRA = 0xF0; PORTA = 0x0F;
DDRB = 0x00; PORTB = 0xFF;;
DDRC = 0xFF; PORTC = 0x00;// LCD data lines
DDRD = 0xFF; PORTD = 0x00;

// Set the timer and turn it on
LCD_init();
LCD_ClearScreen();

State0 = wait0;
State = Init;
State2 = game;
// Period for the tasks
unsigned long int SMbutton = 50;
unsigned long int SMreal = 500;
unsigned long int SMdisplay = 90;
unsigned long int GCD = 10;

//Recalculate GCD periods for scheduler
unsigned long int SMTick1_period = SMbutton/GCD;
unsigned long int SMTick2_period = SMreal/GCD;
unsigned long int SMTick3_period = SMdisplay/GCD;

static task task1, task2, task3;
task *tasks[3] = { &task1, &task2, &task3};
const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

// Task 1
task1.state = -1;//Task initial state.
task1.period = SMTick1_period;//Task Period.
task1.elapsedTime = SMTick1_period;//Task current elapsed time.
task1.TickFct = &button;//Function pointer for the tick.

// Task 2
task2.state = -1;//Task initial state.
task2.period = SMTick2_period;//Task Period.
task2.elapsedTime = SMTick2_period;//Task current elapsed time.
task2.TickFct = &realtsk;//Function pointer for the tick.

// Task 3
task3.state = -1;//Task initial state.
task3.period = SMTick3_period;//Task Period.
task3.elapsedTime = SMTick3_period; // Task current elasped time.
task3.TickFct = &displaytsk; // Function pointer for the tick.


TimerSet(GCD);
TimerOn();

car_top();
car_bot();
obstacle3();
obstacle4();
car_top_obstacle();
car_bot_obstacle();
broken_obs();

int time = 0;
fire1.xposition = 0;


unsigned short i; // Scheduler for-loop iterator
while(1) {	
	
	// Scheduler code
	for ( i = 0; i < numTasks; i++ ) {
		// Task is ready to tick
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			// Setting next state for task
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			// Reset the elapsed time for next tick.
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}
	
	LCD_Cursor(0);
	while(!TimerFlag);
	TimerFlag = 0;
}

}
