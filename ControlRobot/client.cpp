#include "client.h"

using namespace std;

Client::Client(QObject * parent): QObject(parent){
    mSocket = 0;
    mValRead = -1;
//    parent = 0;
}

int Client::connect2Server(QString host, QString port){
    if((mSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
//        printf( ("Socket creation error \n");
        return -1;
    }
    memset(&mServ_addr, '0', sizeof(mServ_addr));
    mServ_addr.sin_family = AF_INET;

    const char *host2 = QString2Char(host);
    mServ_addr.sin_port = htons(port.toInt());

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, host2, &mServ_addr.sin_addr)<=0)
    {
//        printf ("Invalid address/ Address not supported \n");
        return -1;
    }
    if (::connect(mSocket, (struct sockaddr *)&mServ_addr, sizeof(mServ_addr)) < 0)
    {
//        printf ("Connection Failed \n");
        return -3;
    }
    return 0;
}

const char * Client::QString2Char(QString str){
    QByteArray tmp = str.toLocal8Bit();
    const char *s = tmp.data();
    return s;
}

int Client::sendData(QJsonObject &json){
//    Convert QJson to QString
    QJsonDocument doc(json);
    QByteArray docByteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson = QLatin1String(docByteArray);
//      Convert QString to char
    const char * str = QString2Char(strJson);
    send(mSocket , str , strlen(str) , 0 );
    return 0;
}

int Client::sessionInit(QString pass){
    QJsonObject json;
    json["action"] = "SessionInitiation";
    json["pass"] = pass;
    sendData(json);
    return 0;
}
