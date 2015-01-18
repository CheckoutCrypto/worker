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
#include <QtCore>
#include <QtNetwork>
#include <iostream>
#include <string>
#include <QNetworkAccessManager>
#include "src/threads/controlthread.h"
#include "src/objects/daeobj.h"
#include "src/objects/dbobj.h"

using namespace std;

class Server : public QObject
{
    Q_OBJECT
public:
    bool WORK_RUNNING, TRADE_RUNNING, CRON_RUNNING, SITE_RUNNING, MERCHANT_RUNNING, API_RUNNING, MOBILE_RUNNING;
    bool WORK_ENABLED, TRADE_ENABLED, CRON_ENABLED, SITE_ENABLED, MERCHANT_ENABLED, API_ENABLED, MOBILE_ENABLED;
    bool CACHE_RUNNING, WORK_QUEUE, TRADE_QUEUE, CRON_QUEUE;
    string WORK_TABLE, TRADE_TABLE, CRON_TABLE, SITE_TABLE, MERCHANT_TABLE, API_TABLE, MOBILE_TABLE;

    int count, socketCount, MaxSockets, wrkSocketCount, tradeSocketCount, cronSocketCount;
    string thread, apikey;
    controlthread *cache, *control, *trade, *cron, *merchant, *site, *mobile, *api, *fee;
    dbObj db;
    daeObj dae;
    grpobj grp;

    explicit Server();
    virtual ~Server();
    void start();
    void respondInvalid();
    void connectSocket(string thread);
    void startNewThread(controlthread *&ctrl, string table, string ctrlthread);
    void setParamObjects(grpobj &grp2, dbObj &db2, daeObj &dae2, string key){
        apikey = key;
        dae = dae2;
        db = db2;
        grp = grp2;
    }
    void initFees(){
        fee = new controlthread("fee");
        fee->start();
        QObject::connect( fee, SIGNAL( ordersDone(string) ),
        this, SLOT( setRunning(string) ) );
        fee->fill(&grp, &db, &dae, "txfee");
    }

signals:
    void dataReceived(QByteArray);
public slots:
    void begin(){
        initFees();
    }

    void reInit(string type){
        if(type == "work"){
           control->fill(&grp, &db, &dae, WORK_TABLE);
        }else if(type == "trade"){;
            trade->fill(&grp, &db, &dae, TRADE_TABLE);
        }else if(type == "cron"){;
            cron->fill(&grp, &db, &dae, CRON_TABLE);
        }else if(type == "merchant"){;
            merchant->exit(1);
            this->MERCHANT_RUNNING = false;
        }else if(type == "mobile"){;
            mobile->exit(1);
            this->MOBILE_RUNNING = false;
        }else if(type == "site"){;
            site->exit(1);
            this->SITE_RUNNING = false;
        }else if(type == "api"){;
            api->exit(1);
            this->API_RUNNING = false;
        }else if(type == "fee"){
            fee->exit(1);
            start();
        }
    }

    void setRunning(string type){
        if(type == "work"){
           if(WORK_QUEUE){
               WORK_QUEUE = false;
                control->fill(&grp, &db, &dae, WORK_TABLE);
           }else{
                this->WORK_RUNNING = false;
           }
        }else if(type == "trade"){;
            if(TRADE_QUEUE){
                 TRADE_QUEUE = false;
                 trade->fill(&grp, &db, &dae, TRADE_TABLE);
            }else{
                this->TRADE_RUNNING = false;
            }
        }else if(type == "cron"){;
            if(CRON_QUEUE){
                CRON_QUEUE = false;
                 cron->fill(&grp, &db, &dae, CRON_TABLE);
            }else{
                this->CRON_RUNNING = false;
            }
        }else if(type == "merchant"){;
            merchant->exit(1);
            this->MERCHANT_RUNNING = false;
        }else if(type == "mobile"){;
            mobile->exit(1);
            this->MOBILE_RUNNING = false;
        }else if(type == "site"){;
            site->exit(1);
            this->SITE_RUNNING = false;
        }else if(type == "api"){;
            api->exit(1);
            this->API_RUNNING = false;
        }else if(type == "fee"){
            fee->exit(1);
            start();
        }
        socketCount--;
    }

private slots:
    void newConnection();
    void disconnected();
    void readyRead();
    void dataRec(QByteArray var);
    qint32 ArrayToInt(QByteArray source);

private:
    QTcpSocket *socket;
    QTcpServer *server;
    QHash<QTcpSocket*, string> threads; //We need a buffer to store which thread socket is used for
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};
