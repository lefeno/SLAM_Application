/*
 * Mediator.cpp
 *
 *  Created on: Feb 22, 2017
 *      Author: trucndt
 */

#include "Mediator.h"

boost::condition_variable data_ready_cond;
boost::mutex data_ready_mutex;
bool data_ready = false;

Mediator::Mediator()
{
	mServPort = gPROG_ARGUMENT["port"].as<int>();
    mServoController = new ServoController();
    mRPlidar = new RPLidarController();
    
    mGridMap = new OccuGridMap();

    mMotion = new MotionModel();
    mMesurement = new MeasurementModel();
    mData = NULL;

    mPre = mCurr = Vector3f(0,0,0);
    //Temporary
    mPassword = "pass1234";
}

Mediator::~Mediator()
{
	// TODO Auto-generated destructor stub
	delete mRPlidar;
    mRPlidar = NULL;

    delete mGridMap;
    mGridMap = NULL;

    mData = NULL;
}

void Mediator::start()
{
	int servSock;					/* Socket descriptor for server */
	int clntSock;					/* Socket descriptor for client */
	struct sockaddr_in servAddr; 	/* Local address */
	struct sockaddr_in clntAddr; 	/* Client address */
	unsigned int clntLen;			/* Length of client address data structure */

    /* Initialize ServoController and RPLidar*/
    mServoController->initialize(0);
    mRPlidar->initialize("/dev/ttyUSB0",115200,1);
	mRPlidar->startMotor();

	/*Configure parameters*/
	int mapSize = 600;
	float mapResolution = 1;	//cm
	int mapSizeX = mapSize;
	int mapSizeY = mapSize;
	mNodeCount = 360;

	float l_occ = 10;
	float l_free = 100;
	float p_i1 = 0.5;
	float l0 = log(p_i1/(1-p_i1)) + 255;
	Vector3f pose;
	pose(0) = mapSizeX*mapResolution/2.0; pose(1) = mapSizeY*mapResolution/2.0; pose(2) = 0;
	float robotSize = 20;
	float alpha = 1;	// thickness of obstacles

	// create an empty map
	mGridMap->createMap(mapResolution,
		mapSizeX,
		mapSizeY,
		alpha,
		l0,
		l_occ,
		l_free,
		robotSize);

	/* Create socket for incoming connections */
	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		dieWithError("socket() failed\n");
	}

	/* Construct local address structure */
	memset(&servAddr, 0, sizeof(servAddr));		/* Zero out structure */
	servAddr.sin_family = AF_INET;				/* Internet address family */
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	servAddr.sin_port = htons(mServPort);	  	/* Local port */

	/* Bind to the local address */
	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
	{
		dieWithError("Aggregator bind() failed");
	}

	/* Mark the socket so it will listen for incoming connections */
	if (listen(servSock, MAXPENDING) < 0)
	{
		dieWithError("listen() failed");
	}

	struct ifaddrs *addrs, *tmp;
	getifaddrs(&addrs);
	tmp = addrs;

	while (tmp) 
	{
	    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
	    {
	        struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
	        if(strcmp(tmp->ifa_name,"wlan0") == 0){
	        	mIP = string(inet_ntoa(pAddr->sin_addr));
	        	break;
	        }
	    }

	    tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);


	boost::thread mapThread(&Mediator::mapHdl, this);

	while (1)
	{
		/* Set the size of the in-out parameter */
		clntLen = sizeof(clntAddr);

		/* Wait for a client to connect */
		cout << "Waiting for client ..." << endl;
		if ((clntSock = accept(servSock, (struct sockaddr *) &clntAddr,
			&clntLen)) < 0)
		{
			cout << "accept() failed" << endl;
			continue;
		}

		// ClntSock is connected to a client! 
		string ClientAddr = inet_ntoa(clntAddr.sin_addr);
		uint16_t ClientPort = ntohs(clntAddr.sin_port);

		mClients.push_back(clntSock);

		cout << "Handling client " << ClientAddr << endl;
		/* Start new thread */
  		Employee *clnt = new Employee(this, mServoController, mRPlidar, clntSock);
		clnt->start();

		{
	        boost::unique_lock<boost::mutex> lock(data_ready_mutex);
	        data_ready = true;
	    }
	    data_ready_cond.notify_all();
    }
}

bool Mediator::comparePassword(const string &aPass)
{
    //TODO: need mutex???
    return (aPass == mPassword);
}

void Mediator::mapHdl(){
	printf("Create map handle thread\n");
	boost::unique_lock<boost::mutex> lock(data_ready_mutex);

	Vector3f tmpCurr = Vector3f(mGridMap->getMapSizeX()/2.0,mGridMap->getMapSizeY()/2.0,0);
	std::vector<Vector2f> tmpArr;	
	while(1){
		if(!data_ready || mClients.size() == 0){
			//Doing something
			cout << "Waiting ..." << endl;
			data_ready_cond.wait(lock);			
		}
		boost::chrono::milliseconds sleepDuration(1000);
	    boost::this_thread::sleep_for(sleepDuration);
	    mCurr = mServoController->getPose();
	    if(mPre != mCurr){
	    	printf("Get scan data...\n");

      		mData = mRPlidar->getScanArray(mNodeCount);	
      		printf("Clear map ...\n");
        	mGridMap->clean();
        	printf("Update map ...\n");
        	mGridMap->updateMap(tmpCurr,mData);
        	// mGridMap->updateMap(mCurr,mData);
        	tmpArr = mGridMap->getOccGrids();
        	tmpArr.insert(tmpArr.begin(),Vector2f(tmpCurr[0],tmpCurr[1]));
        	cout << tmpArr[0] << endl;
        	sendToClient(mapJson(tmpArr));
	    }
	    mPre = mCurr;
	}
}

int Mediator::sendToClient(const Json::Value &aRoot)
{
    string outMsg = aRoot.toStyledString();

    cout << "outMsg's size = " << outMsg.size() << endl;
    // cout << "outMsg = " << outMsg << endl;

    /* rcsApp uses readline() */
    Json::FastWriter fWriter;
    outMsg = fWriter.write(aRoot);

    for(int i = 0; i < mClients.size(); ++ i){
    	if (send(mClients[i], outMsg.c_str(), outMsg.size(), 0) != outMsg.size())
	    {
	        cout << "send() failed from: " << __FILE__ << ":" << __LINE__ << endl;
	        continue;
	    }	
    }
 
    return 0;
}

Json::Value Mediator::mapJson(vector<Vector2f> coordinates){
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
