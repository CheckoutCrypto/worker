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
#ifndef PARSEWRITE_H
#define PARSEWRITE_H

#include "src/db/dataconnect.h"
#include "src/objects/wrkobj.h"
#include "src/objects/grpobj.h"
#include "src/objects/daeobj.h"
#include "src/objects/dbobj.h"
#include "src/objects/wallet.h"
#include "src/lib/jsoncalls.h"

#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtNetwork>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <iomanip>      // setprecision
#include <iostream>     // std::cout, std::fixed

class parseWrite : public QThread
{
    Q_OBJECT
public:
    dbObj *db;
    daeObj *dae;
    wrkObj *wrk;
    grpobj *grp;
    string thread, table;
    JsonCalls rpcVar;
    DataConnect dbConnect;
    wallet walConfirm;
    int rpcID, coinIndex;  // id returned from daemon reply
    double defaultFee;

    parseWrite();
    virtual ~parseWrite();
    void close();

    bool parseAndWrite(QJsonObject *doc, int wrkNum, int type);

    void init(string thread2, string table2, grpobj *group, dbObj *DB, wrkObj *work){
        table = table2;
        thread = thread2;
        db = DB;
        wrk = work;
        grp = group;
        dbConnect.setSQL(db);
    }

    bool getOrders(wrkObj &src, string table);
    bool invalidDebug(QJsonObject *doc,int wrkNum, int type);
    int parseID(QJsonObject *doc);
    void parseInfo(QJsonObject &doc);
    double parseErrorCode(QJsonObject *reply);
    bool parseResult(QJsonObject *doc);
    string parseNewAddress(QJsonObject *doc);
    string parseBalance(QJsonObject *doc);
    double parseConfirmBalance(QJsonObject *doc);
    void parseAccountAddress(QJsonObject &doc);
    string parseTransaction(QJsonObject *doc);
    void parseTransactionByID(wallet &wall, QJsonObject *doc, int wrkNum);
    void parseListAccounts(QJsonObject &doc);
    void setConfirmWallet(wallet wall);
    string controlFee(int wrkNum);
    string calcFee(string amount);
    string getStrFee(double amount, int calltype, int wrkNum);

public slots:


    void setErrorWorkStatus(int orderid, int wrkNum){
       dbConnect.updateWorkOrderStatus(table, orderid, wrkNum);
       dbConnect.updateWorkerStatus();
    }


    bool setWorkOrderStatus(string table, int orderid, int status);

    void parseMe(QJsonObject *doc, int wrkNum){

        int type =0;
        int rpcid = 0;
        QJsonValue val = doc->value(QString("id"));
        rpcid = (int)(val.toDouble());

        if(parseAndWrite(doc, wrkNum, rpcid)){
            qDebug() << "successfully parsed and written" << endl;
        }


    }
     void parseError(QJsonObject *doc, int wrkNum){
         int rpcid = 0;
         QJsonValue val = doc->value(QString("id"));
         rpcid = (int)val.toDouble();
         qDebug() << "parsing the id: "<< rpcid << endl;
           if(invalidDebug(doc, wrkNum, 2)){
               qDebug() << "error logged" << endl;
           }
     }
     void setRpcInfo(daeObj *daemon, int index){
         dae = daemon;
         coinIndex = index;
     }
     void setFeeInfo(string fee){
         walConfirm.setStrAmount(fee);
     }

signals:
    void parseDone();
    void parseDoneAddFee();  /// add fee to finished order
    void parseDoneBalance(); /// add getbalance check to finished order

    void parseDoneAccount(string account); /// return the order's account(for balance check)

     void unlocked();
     void alreadyUnlocked();


     /// order completed
     void ConfirmAmount(bool, bool);
    ///fees
    void SenderFound(string);
    void ReceiveFound(string);
    void FeeCompleted();
    void CheckBalance();

    /// error debug
    void ErrorCompleted(string, bool);
    void unlockWallet();   // exception, unlock required, begin unlock
};

#endif // PARSEWRITE_H
