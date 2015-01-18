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
#include "controlthread.h"


controlthread::controlthread(string type){
    thread = type;
    order=0, coinIndex=0, callType = 0;
    ORDERS_RECEIVED = false;
    ALL_COMPLETE = false;
    RUNNING = false;
    QueuedOrders = false;
}
/*
 * Update local cache from remote, coins + groups
 */
void controlthread::grabCache(dbObj *db, daeObj &dae, grpobj &grp){
    dbconnect.fillCoin(dae);
    dbconnect.fillGroups(grp);
}

/*
 * Initialize and SELECT work orders from remote db
 */
void controlthread::grabWork(dbObj *db, string table){
    dbconnect.setSQL(db);
   dbconnect.fillOrders(wrk, table);   /// mem hog
}

/*
 * Initialize TXFees for each daemon
 */
void controlthread::initTxFees(daeObj *dae){
    wrk.init(dae->size+1);
    for(int x=1; x<= dae->size; x++){
        wrk.set(0, 0, 0, dae->getTxFee(x-1), dae->getCoin(x-1), "-", "-","settxfee","0", "0" );
    }
}

/*
 *  Control the worker run process
 */
void controlthread::fill(grpobj *grp2, dbObj *db2, daeObj *dae2, string table){
    if(table == "txfee"){
        initTxFees(dae2);
    }else{
        grabWork(db2,table);
    }


    if(wrk.size <= 1){
        parse.close();
        emit ordersDone(thread);
    }else{
        if(!RUNNING){
            RUNNING = true;
            startThread(grp2, db2, dae2, table);
        }else{
            parse.init(thread, table, grp2, db2, &wrk);
            oThread.init(thread, table, db2, dae2, &wrk, grp2);
             oThread.begin();
        }
    }
}

void controlthread::startThread(grpobj *grp2, dbObj *db2, daeObj *dae2, string table){
    /* MAIN SIGNALS FOR WORKER PROCESS */
    /// connect signal FROM ORDERS when RPC call is prepared, and needs to be called. Signal RPC THREAD
    QObject::connect( & oThread, SIGNAL( makeCall(QNetworkAccessManager*, QNetworkRequest,QJsonObject*,int,int,int)),
      & rCall, SLOT( makeCall(QNetworkAccessManager*, QNetworkRequest,QJsonObject*,int,int,int) ), Qt::AutoConnection);
    /// connect signal TO MAIN when RPC call is completed, and needs to be parsed. Signal MAIN
    QObject::connect( & rCall, SIGNAL( parseAndWrite(QJsonObject*) ),
      this, SLOT( parseRPC(QJsonObject*) ));
    /// connect signal FROM MAIN when RPC parse is needed (with the exact order index) Signal PARSE
    QObject::connect( this, SIGNAL( parseOrder(QJsonObject*,int) ),
      & parse, SLOT( parseMe(QJsonObject*,int)));
    /// connect signal FROM PARSE when RPC parse is completed, start the next order  Signal ORDER
    QObject::connect( & parse, SIGNAL( parseDone() ),
      & oThread, SLOT( nextOrder()));
    /// connect signal FROM ORDERS when order loop is completed, allow a new socket connection.  Signal Main
    QObject::connect( & oThread, SIGNAL( ordersDone() ),
      this, SLOT( allDone()));   /// Exits all work orders
    /* //---------------------------------------// */

    /* INITIALIZE parse thread with Daemon, Order, coinindex */
    QObject::connect( & oThread, SIGNAL( initParse(daeObj*, int) ),
    & parse, SLOT( setRpcInfo(daeObj*,int) ) );

    /* UNLOCK */
    /// connect PARSE unlock exception (unlock required)
    QObject::connect( & parse, SIGNAL( unlocked() ),
    & oThread, SLOT( unlocked() ) );

    /* EXCEPTION (RPC ERROR) */
    /// connect PARSE unlock exception (unlock required)
    QObject::connect( & parse, SIGNAL( unlockWallet() ),
    & oThread, SLOT( unlockException() ) );
    /// connect RPC ANY ERROR exception to main (so we can add additional parameters)
    QObject::connect( & rCall, SIGNAL( parseError(QJsonObject*) ),
    this, SLOT( parseRPCError(QJsonObject*) ) );
    /// connect MAIN to PARSE any ERROR exception
    QObject::connect( this, SIGNAL( parseOrderError(QJsonObject*, int) ),
    &parse, SLOT( parseError(QJsonObject*,int)) );
    /* EXCEPTION PARSE (FEE, modified balance check) */
    /// connect from PARSE to order thread, to add fee to any ERROR exception
    QObject::connect( & parse, SIGNAL( parseDoneAddFee() ),
      & oThread, SLOT( withdrawException()));
    /// connect from PARSE to order thread, to add balance check to any order
    QObject::connect( & parse, SIGNAL( parseDoneBalance() ),
      & oThread, SLOT( balanceCheck()));
    /// connect from PARSE to order thread, to add account check response
    QObject::connect( & parse, SIGNAL( parseDoneAccount(string) ),
      & oThread, SLOT( accountCheck(string)));


    rCall.start();
    parse.start();

    parse.init(thread, table, grp2, db2, &wrk);
    oThread.init(thread, table, db2, dae2, &wrk, grp2);

    oThread.begin();
}

controlthread::~controlthread(){
    rCall.close();
    rCall.quit();
    parse.quit();
    oThread.quit();
}
