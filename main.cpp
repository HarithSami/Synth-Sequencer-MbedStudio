/*  4x7 Beat sequencer
*   Use joystick to navigate and activate beats
*   Press "Play" to play the activated beats
*   Use potentiometers to control speed and volume
*   Author: Harith S. Ibrahim May 2022 
*/

#include "mbed.h"
#include "Joystick.h" 
#include "N5110.h"
#include <cstdio>
#include <string>

#define NOTE_C4  162                                //note definitions
#define NOTE_D4  194
#define NOTE_E4  230
#define NOTE_F4  249
#define NOTE_G4  292
#define NOTE_A4  340
#define NOTE_B4  394
#define NOTE_C5  423

Joystick joystick(PC_3, PC_2);                     //JoyStick 
N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);   //LCD
PwmOut buzzer(PA_15);                              //Buzzer Speaker
DigitalIn playmode(PC_10);                         //Play/pause button 
DigitalIn select(PC_11);                           //Select Button 
AnalogIn octave_pot(PA_5);                             // Audio octave control
AnalogIn speed_pot(PA_6);                              // Audio Speed control

float y_pos, x_pos, playline;                      //cursor coordinates
bool play =0;                                      //boolian variables 
bool a1,a2,a3, a4, a5, columnA,                    //steps & coloumns
     b1, b2, b3, b4, b5, columnB,
     c1, c2, c3, c4, c5, columnC,
     d1, d2, d3, d4, d5, columnD,
     e1, e2, e3, e4, e5, columnE,
     f1, f2, f3, f4, f5, columnF,
     g1, g2, g3, g4, g5, columnG; 
const int C_major_scale[] = {NOTE_C4,             //create array with the required notes (in order)
 NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5}; 
int octave, speed;                                  //octave level
void draw_interface();                            //Functions
void cursor_control();
void boundary(int x, int y);
void play_note(int frequency);
void selection(float x, float y);                //select notes on the screen
void play_mode();
void draw_steps();
void reset_steps();
void check_coloumn();
void check_pot();


int main(){
    joystick.init();
    lcd.init(LPH7366_1);                                                  //initialise for LPH7366-1 LCD (Options are LPH7366_1 and LPH7366_6)
    lcd.setContrast(0.80);                                                //set contrast to 55%
    lcd.setBrightness(0.5);                                               //set brightness to 50% (utilises the PWM)
    reset_steps();                                                        //resets step values to zero
    
    while (1) {
        draw_interface();                                                 //draws the interface
        cursor_control();                                                 //cursor movement
        check_pot();
        check_coloumn();                                                  //coloumns and steps
        if(playmode.read()){                                              //play/pause button
            play = !play;
            playline=100;
        }if(select.read()){                                               //select button
            selection(x_pos,y_pos);
        }if(play){                                                        //play sequence
            play_mode();
        }
        
        draw_steps();
        lcd.drawCircle(x_pos+4,y_pos+3,4,FILL_BLACK);                     //cursor)
        lcd.drawCircle(x_pos+4,y_pos+3,2,FILL_WHITE);   
        lcd.refresh();                                                    //LCD refresh
        ThisThread::sleep_for(5ms);
    }
}

void boundary(int x, int y){    //check that the point remains on the screen

    if (x < 1 ){        //ensure that the point doesnt pass beyond the left hand side of the screen
        x_pos = 1;
    }else if (x > 67){  //ensure that the point doesnt pass beyond the right hand side of the screen-
        x_pos = 67;
    }

    if(y < 1){          //ensure that the point doesnt pass beyond the top of the screen
        y_pos = 1;
    }else if(y > 28){   //ensure that the point doesnt pass beyond the bottom of the screen
        y_pos = 28;
    }
    //playline boundries
        if(playline > 65){          //ensure that the playlines loops again
        playline = 0;
        }
}

void play_note(int frequency){
    buzzer.period_us((float) 1000000.0f/ (float) frequency);    //set the period of the pwm signal (in us)
    buzzer.pulsewidth_us(buzzer.read_period_us()/2);            //set pulse width of the pwm to 1/2 the period
    ThisThread::sleep_for(speed);                               //play sound for 500ms
}

void selection(float x, float y){
    if(y<9){                     //row1
        if(x<11){
            a1=!a1;
        }

        if(x<22 && x>=11){
            b1=!b1;
        }

        if(x<33 && x>=22){
            c1=!c1;
        }

        if(x<44 && x>=33){
            d1=!d1;
        }

        if(x<55 && x>=44){
            e1=!e1;
        }

        if(x<66 && x>=44){
            f1=!f1;
        }
    
        if(x>=66){
            g1=!g1;
        }
    
    }else if(y>=9 && y<18){      //row2
        if(x<11){
            a2=!a2;
        }

        if(x<22 && x>=11){
            b2=!b2;
        }

        if(x<33 && x>=22){
            c2=!c2;
        }

        if(x<44 && x>=33){
            d2=!d2;
        }

        if(x<55 && x>=44){
            e2=!e2;
        }

        if(x<66 && x>=44){
            f2=!f2;
        }
    
        if(x>=66){
            g2=!g2;
        }
    }else if(y>=18 && y<27){     //row3
        if(x<11){
            a3=!a3;
        }

        if(x<22 && x>=11){
            b3=!b3;
        }

        if(x<33 && x>=22){
            c3=!c3;
        }

        if(x<44 && x>=33){
            d3=!d3;
        }

        if(x<55 && x>=44){
            e3=!e3;
        }

        if(x<66 && x>=44){
            f3=!f3;
        }
    
        if(x>=66){
            g3=!g3;
        }
    }else if(y>=27 && y<36){     //row4
        if(x<11){
            a4=!a4;
        }

        if(x<22 && x>=11){
            b4=!b4;
        }

        if(x<33 && x>=22){
            c4=!c4;
        }

        if(x<44 && x>=33){
            d4=!d4;
        }

        if(x<55 && x>=44){
            e4=!e4;
        }

        if(x<66 && x>=44){
            f4=!f4;
        }
    
        if(x>=66){
            g4=!g4;
        }

    }
}

void cursor_control(){
            if(joystick.get_direction() == N){
                y_pos=y_pos-9;
            }else if(joystick.get_direction() == S){
                y_pos=y_pos+9;
            }else if(joystick.get_direction() == E){
                x_pos=x_pos+11;
            }else if(joystick.get_direction() == W){
                x_pos=x_pos-11;
            }else if(joystick.get_direction() == NE){
                y_pos=y_pos-9;
                x_pos=x_pos+11;
            }else if(joystick.get_direction() == NW){
                y_pos=y_pos-9;
                x_pos=x_pos-11;
            }else if(joystick.get_direction() == SE){
                y_pos=y_pos+9;
                x_pos=x_pos+10;
            }else if(joystick.get_direction() == SW){
                y_pos=y_pos+9;
                x_pos=x_pos-10;
                
            }boundary(x_pos, y_pos); //pass the new x,y position to function
}
           
void draw_interface(){
        lcd.clear();
        //buzzer.suspend();
        lcd.drawRect(0,0,78,37,FILL_TRANSPARENT);   // screen boarder
        lcd.drawRect(playline,  0, 3, 37,FILL_BLACK); //playline
        //horizental lines////////////////////////////
        lcd.drawRect(0, 9, 78, 1,FILL_BLACK);
        lcd.drawRect(0, 18, 78, 1,FILL_BLACK);
        lcd.drawRect(0, 27, 78, 1,FILL_BLACK);
        //vertical lines//////////////////////////////
        lcd.drawRect(11,  0, 12, 37,FILL_TRANSPARENT);
        lcd.drawRect(33,  0, 12, 37,FILL_TRANSPARENT);
        lcd.drawRect(44,  0, 12, 37,FILL_TRANSPARENT);
        lcd.drawRect(55,  0, 12, 37,FILL_TRANSPARENT);
        lcd.drawRect(66,  0, 12, 37,FILL_TRANSPARENT);

}

void draw_steps(){
            //1st row
                if(a1){
                    lcd.drawRect(1,1,10,9,FILL_BLACK); 
                }if(b1){
                    lcd.drawRect(12,1,10,9,FILL_BLACK); 
                }if(c1){
                    lcd.drawRect(23,1,10,9,FILL_BLACK); 
                }if(d1){
                    lcd.drawRect(34,1,10,9,FILL_BLACK); 
                }if(e1){
                    lcd.drawRect(45,1,10,9,FILL_BLACK); 
                }if(f1){
                    lcd.drawRect(56,1,10,9,FILL_BLACK); 
                }if(g1){
                    lcd.drawRect(67,1,10,9,FILL_BLACK); 
                }

            //2nd row

                if(a2){
                    lcd.drawRect(1,10,10,9,FILL_BLACK); 
                }

                if(b2){
                    lcd.drawRect(12,10,10,9,FILL_BLACK); 
                }
                if(c2){
                    lcd.drawRect(23,10,10,9,FILL_BLACK); 
                }

                if(d2){
                    lcd.drawRect(34,10,10,9,FILL_BLACK); 
                }
                if(e2){
                    lcd.drawRect(45,10,10,9,FILL_BLACK); 
                }

                if(f2){
                    lcd.drawRect(56,10,10,9,FILL_BLACK); 
                }
                if(g2){
                    lcd.drawRect(67,10,10,9,FILL_BLACK); 
                }



            //3rd row
                if(a3){
                    lcd.drawRect(1,19,10,9,FILL_BLACK); 
                }

                if(b3){
                    lcd.drawRect(12,19,10,9,FILL_BLACK); 
                }
                if(c3){
                    lcd.drawRect(23,19,10,9,FILL_BLACK); 
                }

                if(d3){
                    lcd.drawRect(34,19,10,9,FILL_BLACK); 
                }
                if(e3){
                    lcd.drawRect(45,19,10,9,FILL_BLACK); 
                }

                if(f3){
                    lcd.drawRect(56,19,10,9,FILL_BLACK); 
                }
                if(g3){
                    lcd.drawRect(67,19,10,9,FILL_BLACK); 
                }



            //4th row
                if(a4){
                    lcd.drawRect(1,28,10,9,FILL_BLACK); 
                }

                if(b4){
                    lcd.drawRect(12,28,10,9,FILL_BLACK); 
                }
                if(c4){
                    lcd.drawRect(23,28,10,9,FILL_BLACK); 
                }

                if(d4){
                    lcd.drawRect(34,28,10,9,FILL_BLACK); 
                }
                if(e4){
                    lcd.drawRect(45,28,10,9,FILL_BLACK); 
                }

                if(f4){
                    lcd.drawRect(56,28,10,9,FILL_BLACK); 
                }
                if(g4){
                    lcd.drawRect(67,28,10,9,FILL_BLACK); 
                }
}

void reset_steps(){
                a1=0;
                a2=0;
                a3=0;
                a4=0; 
                
                b1=0;
                b2=0;
                b3=0;
                b4=0;
                
                c1=0;
                c2=0;
                c3=0;
                c4=0;

                d1=0;
                d2=0;
                d3=0;
                d4=0;

                e1=0;
                e2=0;
                e3=0;
                e4=0;

                f1=0;
                f2=0;
                f3=0;
                f4=0;

                g1=0;
                g2=0;
                g3=0;
                g4=0;

}

void play_mode(){
    if(playline > 2 && playline < 10){            //play coloumnA
        if(columnA){
            if(a1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(a2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(a3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(a4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnA=0;
        }else{
        buzzer.suspend();
        }
    }
    else if (playline > 10 && playline < 20) {    //play coloumnB
               if(columnB){
            if(b1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(b2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(b3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(b4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnB=0;
        }else{
        buzzer.suspend();
        }
    }
    else if (playline > 20 && playline < 30) {   //play coloumnC
        if(columnC){
            if(c1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(c2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(c3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(c4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnC=0;
        }else{
        buzzer.suspend();
        }
    }
    else if (playline > 30 && playline < 40) {
        if(columnD){
            if(d1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(d2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(d3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(d4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnD=0;
        }else{
        buzzer.suspend();
        }
    }
    else if (playline > 40 && playline < 50) {//play coloumnE
        if(columnE){
            if(e1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(e2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(e3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(e4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnE=0;
        }else{
        buzzer.suspend();
        }
    }
    else if (playline > 50 && playline < 60) { //play coloumnF
        if(columnF){
            if(f1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(f2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(f3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(f4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnF=0;
        }else{
        buzzer.suspend();
        }
    }
    else if (playline > 60 && playline < 70) {//play coloumnG
        if(columnG){
            if(g1){
                buzzer.resume();
                play_note(NOTE_C4 + octave);
                buzzer.suspend();
            }if(g2){
                buzzer.resume();
                play_note(NOTE_E4 + octave);
                buzzer.suspend();
            }if(g3){
                buzzer.resume();
                play_note(NOTE_G4 + octave);
                buzzer.suspend();
            }if(g4){
                buzzer.resume();
                play_note(NOTE_B4 + octave);
                buzzer.suspend();
            }
        columnG=0;
        }else{
        buzzer.suspend();
        }
    }
    playline=playline+5;
}

void check_coloumn(){
    columnA = ( a1 || a2 || a3 || a4);
    columnB = ( b1 || b2 || b3 || b4);
    columnC = ( c1 || c2 || c3 || c4);
    columnD = ( d1 || d2 || d3 || d4 );
    columnE = ( e1 || e2 || e3 || e4 );
    columnF = ( f1 || f2 || f3 || f4 );
    columnG = ( g1 || g2 || g3 || g4 );
}

void check_pot(){
    octave = octave_pot.read_u16()/81.92;       //0 to 800 ocrave range   
    speed  = ((speed_pot.read_u16()/66.2)+10);    //10ms to 1000ms speed range
     
}

