/*
 * Employee.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: trucndt
 */

#include "Employee.h"

Employee::Employee(Mediator* aMediator, int aSock)
{
    mMediator = aMediator;
    mSock = aSock;
    mSignal = false;
}

void Employee::start()
{
    boost::thread clntThread(&Employee::clientHdl, this);
}

Employee::~Employee()
{
    // mData = NULL;
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

    /* Commands sequence */
    while (true)
    {
        if ((recvMsgSize = recv(mSock, recvBuffer, RCV_BUFFER_SIZE, 0)) <= 0)
        {
            cout << "recv() failed from: " << __FILE__ << ":" << __LINE__ << endl;
            cout << "rcvMsgSize = " << recvMsgSize << endl;

            close(mSock);
            delete this;
            return;
        }
        recvBuffer[recvMsgSize] = NULL;
        cout << "Received " << recvMsgSize << ": " << recvBuffer << endl;

        receivedCmdHandler(recvBuffer, recvMsgSize);
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

int Employee::receivedCmdHandler(const char *aRcvMsg, int aRcvMsgSize)
{
    Json::Reader reader;
    Json::Value root;

    if (reader.parse(aRcvMsg, aRcvMsg + aRcvMsgSize, root) == 0)
    {
        cout << "Error: Parse JSON failed.\n";
    }

    if (root["action"] == "move")
    {
        
        moveServo(root);
        // mCurr = mServoController->getPose();
        // mCurr = Vector3f(mGridMap->getMapSizeX()/2.0,mGridMap->getMapSizeY()/2.0,0);
        // mData = mRPlidar->getScanArray(mMediator->getCountNode());
        // mGridMap->clean();
        // mGridMap->updateMap(mCurr,mData);
        
        // example to send to Client
        // std::vector<Vector2f> v;
        // v.push_back(Vector2f(0,1));
        // v.push_back(Vector2f(0,2));
        // sendToClient(mapJson(v));

    }
    else if (root["action"] == "getRobotInfo")
    {
        getRobotInfo(root);
    }
    else if (root["action"] == "getServoInfo")
    {
        getServoInfo(root);
    }
    else if (root["action"] == "stop")
    {
    }
    else if (root["action"] == "stopRPLidar")
    {
        // mRPlidar->stopMotor();
        // mRPlidar->clean();
    }
    else
    {
        cout << "ERR: Invalid command: " << aRcvMsg << endl;
        cmdNotSupported(root["action"].asString());
    }

}

Json::Value Employee::mapJson(vector<Vector2f> coordinates){
    Json::Value res, tmp;
    Json::Value tmp_arr;
    for(int i = 0; i < coordinates.size(); ++i){
        tmp["x"] = coordinates[i][0];
        tmp["y"] = coordinates[i][1];
        tmp_arr[i] = tmp;
    }
    res["data"] = tmp_arr;
    return res;
}

int Employee::moveServo(const Json::Value &aRoot)
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

        // mServoController->setRpm(right, left);
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
    else
    {
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

int Employee::getRobotInfo(const Json::Value &aRoot)
{
    Json::Value response;

    response["result"] = "SUCCESS";
    response["description"] = "Get Robot info";
    // response["value"] = mServoController->getOdometer();

    sendToClient(response);

    return 0;
}

int Employee::getServoInfo(const Json::Value &aRoot)
{
    Json::Value response;

    response["result"] = "SUCCESS";
    response["description"] = "Get Servo info";
    // response["value"] = mServoController->getServoInfo();

    sendToClient(response);

    return 0;
}