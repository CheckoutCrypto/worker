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
#ifndef ORDERTHREAD_H
#define ORDERTHREAD_H

#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QWaitCondition>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "src/objects/daeobj.h"
#include "src/objects/dbobj.h"
#include "src/objects/wrkobj.h"
#include "src/objects/grpobj.h"

#include "src/db/dataconnect.h"
#include "src/lib/jsoncalls.h"
#include "src/lib/rpccntrl.h"
#include "src/lib/rpccall.h"
#include "src/threads/parsewrite.h"

class orderthread : public QThread
{
       Q_OBJECT
public:
    int order, callType, coinIndex;
    bool BUSY;
    string myurl, thread, table;

    QNetworkAccessManager nam;
    QNetworkRequest req;
    QWaitCondition unlockWait;
    QMutex mutex;

    daeObj *dae;
    dbObj *db;
    grpobj *grp;
    wrkObj *wrk;
    JsonCalls rpcVar;
    rpcCntrl rpcCn;
    bool WALLET_UNLOCKED, ALREADY_UNLOCKED, ACCOUNT_FOUND;

    explicit orderthread();
    virtual ~orderthread();

    void init(string thread2, string table2, dbObj *db2, daeObj *dae2, wrkObj *wrk2, grpobj *grpob);

    void begin();       // loop orders
    void validateOrder();  // check if unlock/fee needed
    void initOrder();   // set Index for coin and call type
    void submitOrder();  // submit rpc call

    /* Exceptions */
    void unlockWallet();
    void addFee();
    void balance_check(string account);
    void account_check();

    void close();

public slots:

    /// Recheck work orders, in case we missed any during work
    void recheckWork(){
        BUSY = false;
        begin();
    }


    /// Account found (for modified getbalance)
    void accountCheck(string foundAccount){
        ACCOUNT_FOUND = true;
        balance_check(foundAccount);
    }

    /// additional balance check - getreceivedby maxconfirm
    void balanceCheck(){
        account_check();
    }

    /// unlock wallet exception from parse
    void unlockException(){
        ALREADY_UNLOCKED = false;
        unlockWallet();
    }

    /// coingateway fee Exception from parse - move fees after withdraw
    void withdrawException(){
        addFee();
    }

    /// unlock just occured, parse response happened
    void unlocked(){
        WALLET_UNLOCKED = false;
        ALREADY_UNLOCKED = true;
        BUSY = false;
        begin();
    }
    /// Order Completed, Continue the order thread.
    void nextOrder(){
        order++;
        BUSY = false;
        begin();
    }
signals:
    void makeCall(QNetworkAccessManager*, QNetworkRequest, QJsonObject*, int, int, int); //init rpc call
    void initParse(daeObj *, int);  // daemon + coin index for parse
    void ordersDone(); // all work orders completed

};

#endif // ORDERTHREAD_H
