#include <stdio.h> 
#include <unistd.h>
#include <iostream>
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <string>
#include <ncurses.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "utils.h"
#include "json/json.h"
#include <bits/stdc++.h>

#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#define PORT 59998
#define STOP 0

using namespace std;

static const int RCV_BUFFER_SIZE = 100000;

void resetCounter(int &counter);
string moveRobot(int key);
void listener(short socket);

int main()
{
    // struct sockaddr_in address; 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 

    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "10.42.0.1", &serv_addr.sin_addr)<=0) 
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 


    char* initSession = "{\"action\": \"SessionInitiation\",\"pass\" : \"pass1234\"}";
    char* start = "{\"action\": \"stream\"}";
    char* stopLidar = "{\"action\": \"stopRPLidar\"}";
    send(sock , initSession , strlen(initSession) , 0 );
    printf("Sent SessionInitiation\n"); 

    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 

    // send(sock , start , strlen(start) , 0 );
    printf("Sent Streaming\n"); 

    valread = read( sock , buffer, 1024); 
    printf("%s\n",buffer ); 

    //create a thread to listen incoming data
    boost::thread listenServer(listener,sock);
    int ch; 
    // /* Curses Initialisations */

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    printw("Welcome - Use Arrow to Control Robot\n");

    bool press = false, sendStop = false;
    char move[512];
    while(ch = getch()) 
    { 
        switch(ch)
        {
            case KEY_UP:          
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                if(!press){
                    memset(move, 0, sizeof(move));
                    strcpy(move,moveRobot(ch).c_str());
                    send(sock , move , strlen(move) , 0 );
                    press = true;
                    sendStop = false;
                }
                break;
            case '0':
                send(sock , stopLidar , strlen(stopLidar) , 0 );
            case '#':
                refresh();
                getch();
                endwin();
                return 0;  
            default:
                if(press && !sendStop){
                    press = false;
                    memset(move, 0, sizeof(move));
                    strcpy(move,moveRobot(STOP).c_str());
                    send(sock , move , strlen(move) , 0 );
                    sendStop = true;
                }
                break;            
        }
    }
    listenServer.join();
    return 0;
}


void listener(short socket){
    char recvBuffer[RCV_BUFFER_SIZE];
    int recvMsgSize;

    while(true){
        if ((recvMsgSize = recv(socket, recvBuffer, RCV_BUFFER_SIZE, 0)) <= 0)
        {
            cout << "recv() failed from: " << __FILE__ << ":" << __LINE__ << endl;
            cout << "rcvMsgSize = " << recvMsgSize << endl;

            close(socket);
            // delete this;
            // return;
        }
        recvBuffer[recvMsgSize] = NULL;
        cout << "Received " << recvMsgSize << ": " << recvBuffer << endl;    
    }
}


string moveRobot(int key){
    string tmp = "{\"action\":\"move\",";
    switch(key){
        case KEY_UP: 
            tmp += "\"leftMotor\":-800";
            tmp += ",";
            tmp += "\"rightMotor\":800";
            break;
        case KEY_DOWN:
            tmp += "\"leftMotor\":800";
            tmp += ",";
            tmp += "\"rightMotor\":-800";
            break;
        case KEY_LEFT:
            tmp += "\"leftMotor\":600";
            tmp += ",";
            tmp += "\"rightMotor\":500";
            // tmp += "\"leftMotor\":-400";
            // tmp += ",";
            // tmp += "\"rightMotor\":500";
        
            break;
        case KEY_RIGHT:
            tmp += "\"leftMotor\":-500";
            tmp += ",";
            tmp += "\"rightMotor\":-600";
            // tmp += "\"leftMotor\":-500";
            // tmp += ",";
            // tmp += "\"rightMotor\":400";
            break;
        case STOP: 
            // return "{\"action\":\"stop\""};

            tmp += "\"leftMotor\":-400";
            tmp += ",";
            tmp += "\"rightMotor\":400";
            break;
        default:
            break;
    }
    tmp += "}";
    return tmp;
}
