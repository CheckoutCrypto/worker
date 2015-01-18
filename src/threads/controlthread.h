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
#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QDebug>
#include <QThread>
#include <QMutexLocker>
#include <QMutex>
#include <QWaitCondition>
#include <QNetworkAccessManager>
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
#include "src/threads/orderthread.h"

class controlthread : public QThread
{
       Q_OBJECT
public:
    wrkObj wrk;
    DataConnect dbconnect;
    JsonCalls rpcVar;
    parseWrite parse;
    rpcCntrl rpcCn;
    rpcCall rCall;
    orderthread oThread;
    int coinIndex, order, callType;
    bool ORDERS_RECEIVED, RPC_COMPLETE, ORDER_COMPLETE, JUST_UNLOCKED, UNLOCK_COMPLETE, ALL_COMPLETE, RUNNING;
    bool QueuedOrders;
    QMutex mutex;
    QWaitCondition wCond;
    QNetworkRequest req;
    QJsonObject rpcResp;

    QThread rpc, prs;
    string thread;

    explicit controlthread(string thread);
    virtual ~controlthread();

    void grabCache(dbObj *db, daeObj &dae, grpobj &grp);
    void grabWork(dbObj *db, string table);

    void fill(grpobj *grp, dbObj *db, daeObj *dae, string table);
    void startThread(grpobj *grp2, dbObj *db2, daeObj *dae2, string table);

    void initializeOrder();
    void initTxFees(daeObj *dae);

    void checkActionForUnlock();
    void checkUnlockParse();

    void makeCall(dbObj *db, daeObj *dae);
    void parseRPC();

    void QueueOrder(){
        QueuedOrders = true;
    }

public slots:
    //// Send rpc json reply to parse thread
    void parseRPC(QJsonObject *json){
        emit parseOrder(json, oThread.order);
    }
    //// Send rpc json error reply to parse thread
    void parseRPCError(QJsonObject *json){
        emit parseOrderError(json, oThread.order);
    }

    void allDone(){
        wrk.~wrkObj();
            emit ordersDone(thread);
    }

signals:
    void setCache(grpobj *, dbObj *, daeObj *);    /// reinit all caches remote -> local
    void ordersDone(string);  // exit
    void parseOrder(QJsonObject * json, int);  // parse rpc response
    void parseOrderError(QJsonObject * json, int);  // parse rpc response error
    void recheckOrders(); ///  redouble check and completed work orders.
};

#endif // CONTROLTHREAD_H
