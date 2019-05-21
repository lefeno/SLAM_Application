/*
 * Employee.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: trucndt
 */

#include "Employee.h"

#define STOP_VALUE 400
#define MAX_VALUE 6000
#define TURN_VALUE 3000
#define FORWARD 30000
#define CONVERT_VALUE 70
#define MIN_V 20
#define DIST 170    //Distance between 2 wheels (mm)
Employee::Employee(Mediator* aMediator, ServoController *aServoController, RPLidarController * aRPlidar, int aSock)
{
    mMediator = aMediator;
    mSock = aSock;
    mServoController = aServoController;
    mRPLidar = aRPlidar;
}

void Employee::start()
{
    boost::thread clntThread(&Employee::clientHdl, this);
}

Employee::~Employee()
{
}

void Employee::clientHdl()
{
    char recvBuffer[RCV_BUFFER_SIZE];
    int recvMsgSize;

    /* Connection establishment */
    if (sessionInitiation() < 0) //failed to create a session
    {
        cout << "Cannot initiate session\n";
        close(mSock);
        delete this;
        return;
    }

    bool timerTrigger = false;
    int timer = 0;
    Vector3f control(0,0,0);
    /* Commands sequence */
    while (true)
    {
        if ((recvMsgSize = recv(mSock, recvBuffer, RCV_BUFFER_SIZE, 0)) <= 0)
        {
            cout << "recv() failed from: " << __FILE__ << ":" << __LINE__ << endl;
            cout << "rcvMsgSize = " << recvMsgSize << endl;

            std::vector<int>::iterator position = std::find((mMediator->mClients).begin(),(mMediator->mClients).end(),mSock);
            if(position != (mMediator->mClients).end()){
                (mMediator->mClients).erase(position);
            }

            close(mSock);
            delete this;
            return;
        }


        if (timerTrigger){
            ++timer;
            if(timer > 1000){
                timer = 0;
                mServoController->moveXY(control(0),control(1),control(2));
                cout << "Turn\n" ;
            }
        }

        recvBuffer[recvMsgSize] = NULL;
        cout << "Received " << recvMsgSize << ": " << recvBuffer << endl;

        receivedCmdHandler(recvBuffer, recvMsgSize, timerTrigger, timer, control);
    }
    return;
}

int Employee::sendToClient(const Json::Value &aRoot)
{
    string outMsg = aRoot.toStyledString();

    cout << "outMsg's size = " << outMsg.size() << endl;
    cout << "outMsg = " << outMsg << endl;

    /* rcsApp uses readline() */
    Json::FastWriter fWriter;
    outMsg = fWriter.write(aRoot);

    if (send(mSock, outMsg.c_str(), outMsg.size(), 0) != outMsg.size())
    {
        cout << "send() failed from: " << __FILE__ << ":" << __LINE__ << endl;
        return -1;
    }

    return 0;
}

int Employee::receivedCmdHandler(const char *aRcvMsg, int aRcvMsgSize, bool &trigger, int & timer, Vector3f & control)
{
    Json::Reader reader;
    Json::Value root;

    if (reader.parse(aRcvMsg, aRcvMsg + aRcvMsgSize, root) == 0)
    {
        cout << "Error: Parse JSON failed.\n";
    }

    if (root["action"] == "move")
    {
        moveServo(root, trigger, timer, control);
        // mCurr = mServoController->getPose();
        // mCurr = Vector3f(mGridMap->getMapSizeX()/2.0,mGridMap->getMapSizeY()/2.0,0);
        // mData = mRPlidar->getScanArray(mMediator->getCountNode());
        // mGridMap->clean();
        // mGridMap->updateMap(mCurr,mData);
        // sendToClient(mapJson(mGridMap->getOccGrids()));
        //test Json data
        // exportToFile(mapJson(mGridMap->getOccGrids()));
    }
    else if (root["action"] == "getRobotInfo")
    {
        getRobotInfo(root);
    }
    else if (root["action"] == "getServoInfo")
    {
        getServoInfo(root);
    }
    else if (root["action"] == "stopRPLidar")
    {
        mRPLidar->stopMotor();
        mRPLidar->clean();
    }
    else if (root["action"] == "stream")
    {
        getStreamingInfo(root);
    }
    else
    {
        cout << "ERR: Invalid command: " << aRcvMsg << endl;
        cmdNotSupported(root["action"].asString());
    }

}

int Employee::moveServo(const Json::Value &aRoot, bool &trigger, int & timer, Vector3f & control)
{
    Json::Value response, argument;
    int8_t ret = 0;

    response["result"] = "SUCCESS";
    response["description"] = "Move successfully";

    /* Check for errors */
    if (aRoot.isMember("leftMotor") == false || aRoot.isMember("rightMotor") == false)
    {
        ret = -1;
    }
    else
    {
        short right = aRoot["rightMotor"].asInt();
        short left = aRoot["leftMotor"].asInt();
        cout << "R: " << right << " L: " << left << endl;

        // mServoController->setRpm(right, left);
        //stop robot
        if(right >= 0 && right <= STOP_VALUE && left >= -STOP_VALUE && left <= 0){
            cout << "Stop robot" << endl;
            mServoController->setRpm(right, left);

            trigger = false;
            timer = 0;
        } //move forward
        else if (right == -left && right > 0){
            mServoController->moveXY(FORWARD, 0, rpmToV(right));
            cout << "F" << endl;
        } //rotate 180
        else if (-right == left && left > 0){
            // mServoController->moveXY(-FORWARD, 0, rpmToV(left));
            mServoController->turn(3.14,100);
            // usleep(300);
            cout << "B" << endl;
        } //rotate robot to the left: left wheel = 0//rotate robot to the left: left wheel = 0
        else if (left >= -STOP_VALUE && left <= 0){
            mServoController->turn(-TURN_VALUE,rpmToV(right));
            cout << "L" << endl;
        } //rotate robot to the right: right wheel = 0//rotate robot to the right: right wheel = 0
        else if (right >= 0 && right <= STOP_VALUE){
            mServoController->turn(TURN_VALUE,rpmToV(-left));
            cout << "R" << endl;
        } else{
            // cout << "Nothing ... " << endl;
            trigger = true;
            timer ++;
            float rc = 0, x = 0, y = 0;

            if (left < 0){ //forward, convert -400->-6000 2800->6000
                left = (-left - 400)/2.0 + 2800;
            } else{ //backward, convert 400->6000 2800->0
                left = 2800 - (left - 400)/2.0;
            }

            if (right > 0){ //forward, convert 400->6000 2800->6000
                right = (right - 400)/2.0 + 2800;
            } else{ //backward, convert -400->-6000 2800->0
                right = 2800 - (-right - 400)/2.0;
            }

            rc = left*DIST/right;

            if(left > right){ //turn right
                x = rc + DIST*0.5 + (rc + DIST*0.5)*cos(0.1);
                y = (rc + DIST*0.5)*sin(0.1);
                cout << "Turn left: x: " << x << " y: " << y << endl;
            } else { //turn left
                x = - (rc + DIST*0.5 + (rc + DIST*0.5)*cos(0.1));
                y = (rc + DIST*0.5)*sin(0.1);
                cout << "Turn right: x: " << x << " y: " << y << endl;
            }
            control(0) = y;
            control(1) = x;
            control(2) = 25; //rpmToV((left + right)/2);
            mServoController->moveXY(control(0),control(1),control(2));
        }
    }

    if (ret == -1)
    {
        response["result"] = "FAILED";
        response["description"] = "Missing required parameters";
    }

//    Not needed
//    sendToClient(response);

    return ret;
}

void Employee::cmdNotSupported(const string& aCmd)
{
    Json::Value response;

    response["result"] = "FAILED";
    response["description"] = string("The command " + aCmd + " is not supported");

    sendToClient(response);
}

int Employee::sessionInitiation()
{
    char rcvBuffer[RCV_BUFFER_SIZE];
    int rcvMsgSize, ret;
    Json::Reader reader;
    Json::Value root, response;

    if ((rcvMsgSize = recv(mSock, rcvBuffer, RCV_BUFFER_SIZE, 0)) <= 0)
    {
        cout << "recv() failed from: " << __FILE__ << ":" << __LINE__ << endl;
        return -1;
    }
    rcvBuffer[rcvMsgSize] = 0;
    cout << "Received " << rcvMsgSize << ": " << rcvBuffer << endl;

    if (reader.parse(rcvBuffer, rcvBuffer + rcvMsgSize, root) == 0)
    {
        cout << "Error: Parse JSON failed.\n";
        return -1;
    }

    response["result"] = "SUCCESS";
    response["description"] = "Session Initiated";
    ret = 0;

    /* Check for errors */
    if (root.isMember("action") == false || root.isMember("pass") == false)
    {
        ret = -1;
    }
    else if (root["action"].asString() != "SessionInitiation")
    {
        ret = -2;
    }
    else{
        if (mMediator->comparePassword(root["pass"].asString()) == false)
        {
            ret = -3;
        }
    } 

    /* Generate response */
    if (ret < 0)
    {
        response["result"] = "FAILED";
        if (ret == -1) response["description"] = "Missing required parameters";
        else if (ret == -2) response["description"] = "You must initiate a session first";
        else if (ret == -3) response["description"] = "Wrong password";
    }

    sendToClient(response);

    return ret;
}

double Employee::rpmToV(short rpm){
    double tmp = (rpm - STOP_VALUE)/CONVERT_VALUE + MIN_V;
    return tmp;
}

int Employee::getRobotInfo(const Json::Value &aRoot)
{
    Json::Value response;

    response["result"] = "SUCCESS";
    response["description"] = "Get Robot info";
    response["value"] = mServoController->getOdometer();

    sendToClient(response);

    return 0;
}

int Employee::getServoInfo(const Json::Value &aRoot)
{
    Json::Value response;

    response["result"] = "SUCCESS";
    response["description"] = "Get Servo info";
    response["value"] = mServoController->getServoInfo();

    sendToClient(response);

    return 0;
}

int Employee::getStreamingInfo(const Json::Value &aRoot)
{
    int8_t ret = 0;
    Json::Value response;

    response["result"] = "SUCCESS";
    response["description"] = "Streaming";

    // Json::Value requestIP;
    // requestIP["action"] = "requestIP";
    // Json::FastWriter writer;
    // string outMsg = writer.write(requestIP);
    // char buf[1024];

    // int byteRcv = mSpokesman->send(writer.write(requestIP).c_str(), buf);
    // requestIP.clear();

    // Json::Reader reader;
    // reader.parse(buf, buf + byteRcv, requestIP, false);

    response["IP"] = mMediator->getIP();
    response["port"] = mMediator->getServerPort();

    sendToClient(response);
    // cout << response << endl;

    return ret;
}