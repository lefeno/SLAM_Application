#include"ServoController.h"
#include<sys/time.h>
#include<iostream>
#include"json/json.h"

using namespace std;

static int cnt = 0;

void cnt_timer(){
    ++cnt;  
}

void reset_timer(){
    cnt = 0;
}

int main(int argc, char **argv)
{
        cout << "Hello I'm RO_TO" << endl;

    enum State{INIT, RUN, GET_FEEDBACK, STOP};
    enum State state;

    ServoController a;
    state = INIT;
    int num_fb = 2;
    int out = 0;
    while(1){
        switch(state){
            case INIT:
                a.initialize(0);
                state = RUN;
                break;
            case RUN:
                a.moveForward(3000,50);
                state = GET_FEEDBACK;
                break;
            case GET_FEEDBACK:
                if(cnt == 10){
                    a.getOdometer();
                    reset_timer();
                    num_fb -= 1;
                    if(num_fb == 0){
                        state = STOP;
                    }
                }
                break;
            case STOP:
                cout << "Stop robot" << endl;
                a.stopRobot();
                out = 1;
                break;
            default:
                break;                          
        }
        usleep(100000);
        cnt_timer();
        if(out){
            break;
        }
    }
    return 0;
}

