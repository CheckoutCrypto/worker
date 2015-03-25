/* Copyright 2014-2015 Grant Hutchinson

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

One exception, this software is not allowed to be used by Simon Choucroun or any assosciate or affiliate individual/company
*/
#include "server.h"

Server::Server()
{
    apikey = "";
    count = 1;
    /// server socket status var
    MaxSockets = 20;
    socketCount = 0;
    wrkSocketCount = 0;
    tradeSocketCount = 0;
    cronSocketCount = 0;
    thread = "";
    /// thread status var
    CACHE_RUNNING = true;
    WORK_RUNNING = false;
    TRADE_RUNNING = false;
    CRON_RUNNING = false;
    API_RUNNING = true;
    SITE_RUNNING = true;
    MOBILE_RUNNING = true;
    MERCHANT_RUNNING = true;
    /// admin status var
    WORK_ENABLED = true;
    TRADE_ENABLED = true;
    CRON_ENABLED = true;
    API_ENABLED = true;
    SITE_ENABLED = true;
    MOBILE_ENABLED = true;
    MERCHANT_ENABLED = true;
    //// thread remote tables
    WORK_TABLE = "ccdev_work_orders";
    TRADE_TABLE = "ccdev_trade_orders";
    CRON_TABLE = "ccdev_cron_orders";
    MERCHANT_TABLE = "ccdev_merchant_orders";
    API_TABLE = "ccdev_api_orders";
    MOBILE_TABLE = "ccdev_mobile_orders";
    SITE_TABLE = "ccdev_site_orders";

    /// QUEUES
    WORK_QUEUE = false;
    TRADE_QUEUE = false;
    CRON_QUEUE = false;
}

void Server::start(){
    QHostAddress host;
    host.setAddress("127.0.0.1");  // localhost
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), SLOT(newConnection()));
    qDebug() << "CheckoutCrypto Server started!" << endl;
    qDebug() << "Address: " << host.toString().toStdString().c_str() << "Port: "<< 12311 << "Listening: " << server->listen(host, 12311);

    startNewThread(control, WORK_TABLE, "work");
    startNewThread(trade, TRADE_TABLE, "trade");
    startNewThread(cron, CRON_TABLE, "cron");

    /// immediately check orders
    WORK_RUNNING = true;
    TRADE_RUNNING = true;
    CACHE_RUNNING = true;

    control->fill(&grp, &db, &dae, WORK_TABLE);
    trade->fill(&grp, &db, &dae, TRADE_TABLE);
    cron->fill(&grp, &db, &dae, CRON_TABLE);

}

void Server::respondInvalid(){
    thread = "invalid";
    QByteArray response(thread.c_str(), thread.length());
  //  socket->write(response);
}

void Server::startNewThread(controlthread *&ctrl, string table, string ctrlthread){
    ctrl = new controlthread(ctrlthread);
    ctrl->start();
    QObject::connect( ctrl, SIGNAL( ordersDone(string) ),
    this, SLOT( setRunning(string) ) );
}

void Server::dataRec(QByteArray inMsg) {

    QString ApiMsg = QString(inMsg);
    bool valid = false;
    int MsgSize = 0, ApiSize = 0;
    string MsgKey = "", MsgParam = "";
    QString resp = "";
  //  qDebug() << ApiMsg << endl;
    ApiSize = apikey.length();
    MsgSize = ApiMsg.length();
    MsgKey = ApiMsg.toStdString().substr(0, MsgSize);
    MsgParam = MsgKey.substr(ApiSize, MsgSize);
    ApiMsg = ApiMsg.toStdString().substr(0, ApiSize).c_str();

   if(ApiMsg.toStdString() == apikey){      /// do order list
           if(MsgParam == "-work"){
               if(WORK_ENABLED && !WORK_RUNNING){
                   thread = "work";
                   WORK_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
                //   socket->write(response);
                   control->fill(&grp, &db, &dae, WORK_TABLE);
               }else{
                   WORK_QUEUE = true;
                    respondInvalid();
               }
           }else if(MsgParam == "-trade"){
               if(TRADE_ENABLED && !TRADE_RUNNING){
                   thread = "trade";
                   TRADE_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
               //    socket->write(response);
                   trade->fill(&grp, &db, &dae, TRADE_TABLE);
               }else{
                    TRADE_QUEUE = true;
                    respondInvalid();
               }
           }else if(MsgParam == "-cron"){
               if(CRON_ENABLED && !CRON_RUNNING){
                   thread = "cron";
                   CRON_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
                 //  socket->write(response);
                   cron->fill(&grp, &db, &dae, CRON_TABLE);
               }else{
                   CRON_QUEUE = true;
                    respondInvalid();
               }
           }else if(MsgParam == "-merchant"){
               if(MERCHANT_ENABLED && !MERCHANT_RUNNING){
                   thread = "merchant";
                   MERCHANT_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
                   socket->write(response);
                   startNewThread(merchant, MERCHANT_TABLE, thread);
               }else{
                    respondInvalid();
               }
           }else if(MsgParam == "-site"){
               if(SITE_ENABLED && !SITE_RUNNING){
                   thread = "site";
                   SITE_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
                   socket->write(response);
                   startNewThread(site, SITE_TABLE, thread);
               }else{
                    respondInvalid();
               }
           }else if(MsgParam == "-api"){
               if(API_ENABLED && !API_RUNNING){
                   thread = "api";
                   API_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
                   socket->write(response);
                   startNewThread(api, API_TABLE, thread);
               }else{
                    respondInvalid();
               }
           }else if(MsgParam == "-mobile"){
               if(MOBILE_ENABLED && !MOBILE_RUNNING){
                   thread = "mobile";
                   MOBILE_RUNNING = true;
                   QByteArray response(thread.c_str(), thread.length());
                   socket->write(response);
                   startNewThread(mobile, MOBILE_TABLE, thread);
               }else{
                    respondInvalid();
               }
           }else if(MsgParam == "cache"){
             //  cache = new controlthread("thread");
             /*  QObject::connect( cache, SIGNAL( ordersDone(string) ),
               this, SLOT( setRunning(string) ) );
                 cache->grabCache(&db, dae, grp); */
           }else{
               respondInvalid();
           }
   }else{
       respondInvalid();
   }
}

Server::~Server(){
    delete server;
}

void Server::newConnection()
{
    bool AVAIL = false;
    while(server->hasPendingConnections())
    {
         AVAIL = false;
        socket = server->nextPendingConnection();
        qDebug() << "connect" <<endl;
        if(!TRADE_RUNNING || !WORK_RUNNING || !CRON_RUNNING){
             if(socketCount < MaxSockets){
                    socketCount++;
                    connectSocket("cron");
                    AVAIL = true;
            }else{
                 socket->deleteLater();
            }
        }else{
            socket->deleteLater();
        }
    }
}

void Server::connectSocket(string sThread){
    connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
     connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
     QByteArray *buffer = new QByteArray();
     qint32 *s = new qint32;
     *s = 0;
     buffers.insert(socket, buffer);
     sizes.insert(socket, s);
     threads.insert(socket, sThread);

}

void Server::disconnected()
{
    socket = static_cast<QTcpSocket*>(sender());
    socketCount--;
    socket->deleteLater();
}

void Server::readyRead()
{
    connect(this, SIGNAL(dataReceived(QByteArray)),this ,SLOT(dataRec(QByteArray)), Qt::UniqueConnection);
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray data;
    int bytes = socket->bytesAvailable();
    int len;
    data = socket->readAll();
    len = qstrlen(data);

     if(len == bytes) {
         emit dataReceived(data);
     } else {
         qDebug() << "Couldn't read all data!";
     }
}

qint32 Server::ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadOnly);
    data >> temp;
    return temp;
}
