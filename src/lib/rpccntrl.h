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
#ifndef RPCCNTRL_H
#define RPCCNTRL_H

#include "src/lib/jsoncalls.h"
#include "src/objects/wrkobj.h"
#include "src/objects/daeobj.h"
#include "src/objects/wallet.h"
#include "src/objects/grpobj.h"
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QMutexLocker>

class rpcCntrl : public QObject
{
    Q_OBJECT
public:
    rpcCntrl();
    virtual ~rpcCntrl();
    int count;
    bool LOCK, FEEWITHDRAW, FEE, FEE_SENDER, FEE_RECEIVE, FEE_ADMIN_WALLET, VALIDATED,
    Completed, DONE, MAXCONFIRM, BUSY, ERROR, COINFOUND, CHECKBALANCE;
    int MAXORDERS, MAXRETRIES;
    int callType, coinIndex, prevCount, typeBeforeFee, retryCount;
    string FEEWALLET, UNLOCKTIME;
    JsonCalls jCall;
    QMutex mutex;

    wallet confirmWallet, feeWall;

    void initCoinAndAction(int index, daeObj *dae, wrkObj *dOrder);
    void JsonData(QJsonObject &json, int type, daeObj *dae, wrkObj *dOrder, int count);
    int getCoinIndex(int orderNum, daeObj *dae, wrkObj *dOrder);
    void regularJson(QJsonObject &json, int id, string *action, string *sender, string *recipient, string *fAmount, string *confirms);
    string calcFee(string amount, string fee);
    QJsonObject unlock(wrkObj *wrk, daeObj *daemon);
    void initFees();
    QJsonObject CntrlFees(int wrkIndex, grpobj *grp, wrkObj *wrk, daeObj *daemon);
    bool validateOrder(grpobj *gro, wrkObj *wrk, daeObj *daemon);
};

#endif // RPCCNTRL_H
