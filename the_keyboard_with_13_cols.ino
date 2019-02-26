#include <HID-Project.h>
#include <HID-Settings.h>


							/*
							P|9 |8 |6 |7 |10|21|16|14|15|18|19|20|0 |
							*|0 |1 |2 |3 |4 |5 |6 |7 |8 |9 |10|11|12|
							0|~ |Q |W |E |R |T |Y |U |I |O |P |[ |] |
							1|TB|A |S |D |F |G |H |J |K |L |; |' |BS|
							2|ZЯ|??|Z |X |C |V |B |N |M |, |. |/ |ENTER|
							3|	|Sh|Me|AL|Lf|Hm|Sp|  |Rt|Ct|BK|Fn| |
								 if nu t  et oe ac	  ig rl	ac
							*/



int pins[] = {9,8,6,7,10,21,16,14,15,18,19,20,0};

int layer1[2][4][13] = { {  {53,20,26,8,21,23,28,24,12,18,19,0x2F,0x30,},
							{43,4,22,7,9,10,11,13,14,15,0x33,0x34,42,},
							{0,224,29,27,6,25,5,17,16,0x36,0x37,0x38,40,},
							{0,225,0,226, 80,0, 44,0, 79,224,0,0,0,} },

																		{ {41,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x2D,0x2E,},
																			{43,4,22,7,9,10,11,13,14,52,51,0x31,0x4C,},
																			{0,224,29,27,6,25,5,17,16,0x36,0x37,0x38,40,},
																			{0,225,0,226, 0x52,0, 44,0, 0x51,224,0,0,0,} } };


bool pressed[4][13] = { {0,0,0,0,0,0,0,0,0,0,0,0,0,},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,},
						{0,0,0,0,0,0,0,0,0,0,0,0,0,} };

int fn_state = 0; /*
0 - Дефолтный режим: Fn не была нажата, вводим обычные символы.
1 - Зажатый режим: Fn в данный момент зажата, вместе с ней нажимаются другие кнопки. Когда Fn отпустят, режим возвращается в 0.
2 - Одиночный режим: Fn была нажата и отпущена, сбрасывается в 0 после первого нажатия любой кнопки, кроме Fn.
3 - Fn lock: Fn была нажата дважды, символы вводятся со второго слоя до тех пор, пока не будет нажата Fn ещё раз.
*/
bool fn_pressed = 0;
bool waitforbutton = 0;
bool fn_lock = 0;
bool unlocked = 0;
int backlight = 0;
bool use_backlight = 1;
bool text = false;
int  text_millis = 0;

void setup() {
	for(int i=0; i<13; i++){
		pinMode(pins[i], INPUT_PULLUP);
	}
	for(int i=2; i<6; i++){
		pinMode(i, OUTPUT);
		digitalWrite(i, HIGH);
	}
	Keyboard.begin();
	//Serial.begin(9600);
	pinMode(1, OUTPUT);
	digitalWrite(1, HIGH);
}

void loop() {
	for(int d=2; d<6; d++){
		digitalWrite(d, LOW);
		for(int j=0; j<13; j++){
			if(digitalRead(pins[j]) == (pressed[d-2][j])){
				pressed[d-2][j] = !pressed[d-2][j];
/*Если зажата*/ if(pressed[d-2][j] == 1){
					if(d < 5){
					    text = true;
					}
					if(text == true){
						if(d == 5 && j == 2){
							Keyboard.press(KEY_LEFT_CTRL);
							Keyboard.write(49); //отправляем ctrl+1
							Keyboard.release(KEY_LEFT_CTRL);
						}
						if(d == 5 && j == 5){
							Keyboard.press(KEY_LEFT_CTRL);
							Keyboard.write(50); //отправляем ctrl+2
							Keyboard.release(KEY_LEFT_CTRL);
						}
						if(d == 5 && j == 10){
							Keyboard.press(KEY_LEFT_CTRL);
							Keyboard.write(51); //отправляем ctrl+3
							Keyboard.release(KEY_LEFT_CTRL);
						}   
					}
					if(text == false){
						if(d == 5 && j == 2){
							Keyboard.press(KEY_MENU);
						}
						if(d == 5 && j == 5){
							Consumer.press(CONSUMER_BROWSER_HOME);
						}
						if(d == 5 && j == 10){
							Consumer.press(CONSUMER_BROWSER_BACK);
						}   
					}
					if(d == 4 && j == 0){ // Нажали ЯZ
						Keyboard.press(KeyboardKeycode(0xE0));
						Keyboard.write(KeyboardKeycode(44));
							//Keyboard.press(KEY_LEFT_CTRL);
							//Keyboard.write(KEY_SPACE);
							//Keyboard.release(KEY_LEFT_CTRL);
					}
					Keyboard.press(KeyboardKeycode(layer1[(pressed[3][11])][(d-2)][j]));
					//Keyboard.press(KeyboardKeycode(layer1[fn_state][(d-2)][j]));
				}
/*Если отжата*/ if(pressed[d-2][j] == 0){
					if(text == false){
						if(d == 5 && j == 2){
							Keyboard.release(KEY_MENU);
						}
						if(d == 5 && j == 5){
							Consumer.release(CONSUMER_BROWSER_HOME);
						}
						if(d == 5 && j == 10){
							Consumer.release(CONSUMER_BROWSER_BACK);
						}   
					}
					if(d == 4 && j == 0){ // Нажали ЯZ
						Keyboard.release(KeyboardKeycode(0xE0));
					}
					Keyboard.release(KeyboardKeycode(layer1[(pressed[3][11])][(d-2)][j]));
					//Keyboard.release(KeyboardKeycode(layer1[fn_state][(d-2)][j]));
				}
			}
		}
		digitalWrite(d, HIGH);
	}
	text_millis++;
	delay(1);
	if(text_millis > 10000){
	    text = false;
	    text_millis = 0;
	}
}
