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
#include "orderthread.h"

orderthread::orderthread()
{
    thread = "", table = "", myurl = "";
    order = 1;
    callType = 0;
    BUSY = false;
    coinIndex = 0;
    WALLET_UNLOCKED = false;
    ALREADY_UNLOCKED = false;
    ACCOUNT_FOUND = false;
}
/*
 *  Initialize order objects and param
 */
void orderthread::init(string thread2, string table2, dbObj *db2, daeObj *dae2, wrkObj *wrk2, grpobj *grp2){
    thread = thread2;
    table = table2;
    db = db2;
    dae = dae2;
    wrk = wrk2;
    grp = grp2;
    thread = "", table = "", myurl = "";
    order = 1;
    callType = 0;
    BUSY = false;
    coinIndex = 0;
    WALLET_UNLOCKED = false;
    ALREADY_UNLOCKED = false;
    ACCOUNT_FOUND = false;
}
/*
 *  Order thread process
 */
void orderthread::begin()
{
    if(wrk->size > 1){
        while(order < wrk->size && !BUSY){
            ///initalize all param needed from order
            initOrder();
            /// validate order, check if unlock needed
            validateOrder();
            /// arrange JsonObject for regular RPC CALL, make call
            if(!WALLET_UNLOCKED){
                submitOrder();
            }
            /// exceptions unlock etc
        }
    }else{
        /// empty work orders
        qDebug() << "completed"<< endl;
        emit ordersDone();
    }
    if(order == wrk->size){
        qDebug() << "completed"<< endl;
        emit ordersDone();
    }
}

/*
 *  Order validation
 */
void orderthread::validateOrder(){
    if(wrk->getAction(order) == "sendfrom" && !ALREADY_UNLOCKED){
        WALLET_UNLOCKED = true;
        unlockWallet();
    }
}
/*
 *  Daemon init
 */
void orderthread::initOrder(){
    /// get/set CallType, CoinIndex
    coinIndex = (rpcCn.getCoinIndex(order, dae, wrk));
    callType = rpcVar.setRpcCall(wrk->getAction(order));  // set call type based on action
    /// get request header
    myurl = rpcVar.getHeaderUrl(dae->getUser(coinIndex), dae->getPass(coinIndex), dae->getServer(coinIndex), dae->getPort(coinIndex));
}
/*
 *  regular RPC call
 */
void orderthread::submitOrder(){
    req = rpcVar.getRequest(myurl);
    /// get request param
    QJsonObject json;
    rpcCn.JsonData(json, callType, dae, wrk, order);
    BUSY = true;

    emit initParse(dae, coinIndex);
    emit makeCall(&nam, req, &json, order, callType, wrk->getOrderID(order));
}
/*
 *  Withdrawal Fee
 */
void orderthread::addFee(){
    QJsonObject json;
    string amount;
    callType = 10005;

    req = rpcVar.getRequest(myurl);
    coinIndex = (rpcCn.getCoinIndex(order, dae, wrk));

    qDebug() << "Adding fee ----" <<endl;

    if(thread == "trade"){
        amount = rpcCn.calcFee(wrk->getAmount(order),grp->getTradeFee(grp->discoverIDPos(wrk->getGrpID(order))));
    }else{
        amount = rpcCn.calcFee(wrk->getAmount(order), grp->getWithdrawFee(grp->discoverIDPos(wrk->getGrpID(order))));  /// send order total amount + grp_price
    }
    wrk->oAmount[order] = amount;

    rpcCn.JsonData(json, callType, dae, wrk, order);
    BUSY = true;
    emit initParse(dae, coinIndex);
    emit makeCall(&nam, req, &json, order, callType, wrk->getOrderID(order));
}

/*
 * Account Check, get wallet account by address
 */
void orderthread::account_check(){
    QJsonObject json;
    callType = 13;

    req = rpcVar.getRequest(myurl);
    coinIndex = (rpcCn.getCoinIndex(order, dae, wrk));

    qDebug() << "Locating an account ----" << endl;
    rpcCn.JsonData(json, callType, dae, wrk, order);
    BUSY = true;
    emit initParse(dae, coinIndex);
    emit makeCall(&nam, req, &json, order, callType, wrk->getOrderID(order));
}



/*
 *  Modified Check Balance Call
 */
void orderthread::balance_check(string account){
    if(ACCOUNT_FOUND){
        QJsonObject json;
        string amount;
        callType = 10004;

        req = rpcVar.getRequest(myurl);
        coinIndex = (rpcCn.getCoinIndex(order, dae, wrk));

        qDebug() << "Double checking balance ----" <<endl;
        wrk->oSender[order] = account;
        rpcCn.JsonData(json, callType, dae, wrk, order);
        BUSY = true;
        emit initParse(dae, coinIndex);
        emit makeCall(&nam, req, &json, order, callType, wrk->getOrderID(order));
    }
}

/*
 *  Prepare and make call to unlock all wallets
 */
void orderthread::unlockWallet(){
    req = rpcVar.getRequest(myurl);
    QJsonObject json;
    coinIndex = (rpcCn.getCoinIndex(order, dae, wrk));
    qDebug() << "unlocking ----" <<endl;
    callType = 9999;
    rpcCn.JsonData(json, callType, dae, wrk, order);
    BUSY = true;
    emit initParse(dae, coinIndex);
    emit makeCall(&nam, req, &json, order, callType, wrk->getOrderID(order));
}

orderthread::~orderthread()
{
}

void orderthread::close(){
    nam.clearAccessCache();
    this->exit(0);
}

