

#ifndef DATACONNECT_H
#define DATACONNECT_H

#include "src/objects/wrkobj.h"
#include "src/objects/wallet.h"
#include "src/objects/dbobj.h"
#include "src/objects/daeobj.h"
#include "src/objects/grpobj.h"

#include <QtSql>

#include <QString>
#include <QDebug>
#include <QObject>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class DataConnect : public QThread
{
      Q_OBJECT
public:

    QSqlDatabase db;
    dbObj *data;

    explicit DataConnect();
    virtual ~DataConnect();
    void close();

    void openDB();
    void closeDB();
    bool getWalletPending(wallet &wall);

    void writeTransaction(wrkObj *src, int wrkNum, string tranID, string fee);
    void updateTransaction(wallet *src, int wrkNum, string tranID);

    bool fillOrders(wrkObj &src, string table);
    bool fillCoin(daeObj &src);
    bool fillGroups(grpobj &src);


    bool writeGeneratedWallet(wallet *wall,wrkObj *src, int workNum);
    bool writeConfirmWallet(wallet *src, int orderid, bool final, bool confirmed, bool walExists);
   bool writeConfirmedBalance(wallet *src, bool withdrawal);
   bool writeUpdatedBalance(int userID, string balance, string coin_code);
    bool writeModifiedBalance(int userID, string balance, string coin_code);

   void confirmWithdrawal(wallet *src, int wrkNum);

   bool writePendingBalance(wallet *src, bool withdrawal);
   bool updateFee(wallet *src, int orderid);

    void writeBillingSummary(wallet *wall, string fee, int grpid);
    void writeBill(wallet *wall, int grpid, string grpname);

   bool writeGeneratedAccount(wrkObj *src, int workNum);
    bool updateWorkOrders(wrkObj *src, string table, int numComplete, int confirmed);
    void writeFee(wallet *wall);
    bool writeFeeBalance(int userID, string amount, string coin_code, bool addremove);

    bool updateWorkOrderStatus(string table, int ordernum, int status);
    bool updateErrorWorkStatus(wrkObj *wrk, int orderid);
    bool updateTransCount(int userid);
    bool updateWorkerStatus();

    void setSQL(dbObj *db2){
        data = db2;
    }

signals:

    void ordersReceived(void);

};

#endif // DATACONNECT_H
