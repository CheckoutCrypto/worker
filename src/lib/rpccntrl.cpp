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
#include "rpccntrl.h"

rpcCntrl::rpcCntrl()
{
    FEEWITHDRAW = false;
    FEE = false;
    FEE_SENDER = false;
    FEE_RECEIVE = false;
    FEE_ADMIN_WALLET = false;
    VALIDATED = false;
    CHECKBALANCE = false;
    MAXCONFIRM = false;
    callType = 0;
    coinIndex = 0;
    Completed = false;
    FEEWALLET="fee";
    UNLOCKTIME = "100";
}

rpcCntrl::~rpcCntrl(){


}



/*
 *  Initialize a work order's parameters
 */
void rpcCntrl::initCoinAndAction(int index, daeObj *daemon, wrkObj *dOrder){
    LOCK = true;


    /// get specific coin daemon index
    /// if getreceivedby call, check if its a maxconfirm (for fees and other confirmations)
    /// get fee for the order user
}

/*
 *  Validate the sendfrom order is greater than that coin's txfee (miner's fee)
 */
bool rpcCntrl::validateOrder(grpobj *grp, wrkObj *wrk, daeObj *daemon){
    if(callType == 4){
           switch(callType){
            case 4:         /// check sendfrom isn't small than coin's txfee
               if(jCall.str2dbl(wrk->getAmount(count)) > jCall.str2dbl(daemon->getTxFee(coinIndex))){
                   VALIDATED = true;
               }else if(jCall.str2dbl(wrk->getAmount(count)) <= jCall.str2dbl(daemon->getTxFee(coinIndex))){
                   VALIDATED = false;
               }
           break;
         }
    }else{
        VALIDATED = true;
    }
   return VALIDATED;
}


void rpcCntrl::JsonData(QJsonObject &json, int type, daeObj *daemon, wrkObj *dOrder, int count){
    string sender = "";
    callType = type;
    jCall.callType = type;

    switch(type){
    case 9999:  /// unlock  <walletpass> <timeunlocked>
        jCall.unlockJson(json, 9999, daemon->getWalPas(coinIndex), UNLOCKTIME);
    break;
    case 1:  /// lock
        jCall.lockJson(json, 9999, daemon->getWalPas(coinIndex), UNLOCKTIME);
    break;
    case 2:  /// getnewaddress(ID) <account>
        jCall.regularJson(json, type, dOrder->getAction(count), dOrder->getSender(count), dOrder->getRecipient(count), dOrder->getAmount(count), dOrder->getStatus(count));
    break;
    case 3:   /// getreceivedbyaddress(ID) <address> <confirms>
        jCall.regularJson(json, type, dOrder->getAction(count), dOrder->getSender(count), dOrder->getRecipient(count), dOrder->getAmount(count), dOrder->getStatus(count));
    break;
    case 4: /// sendfrom(ID)  <account> <address> <amount>
        jCall.regularJson(json, type, dOrder->getAction(count), dOrder->getSender(count), dOrder->getRecipient(count), dOrder->getAmount(count), "");
        break;
    case 5:  /// getbalance(ID) <account> <confirms>
        jCall.regularJson(json, type, "getbalance", dOrder->getSender(count), dOrder->getRecipient(count), "", "");
    break;
    case 6:  /// getaccount(ID) <address>
        jCall.regularJson(json, type, "getaccount", dOrder->getSender(prevCount), "", "", "");
    break;
    case 7: /// getaddressbyaccount(ID) <account>
        jCall.regularJson(json, type, "getaccountaddress", dOrder->getSender(count), "", "", "");
    break;
    case 8: /// getaccountaddress adminaccount
        jCall.regularJson(json, type, "getaccountaddress", FEEWALLET, "", "", "");
    break;
    case 9: /// set tx fee
        jCall.regularJson(json, type, "settxfee", "", "", daemon->getTxFee(coinIndex), "");
    break;
    case 10: /// gettransaction by tranid
        jCall.regularJson(json, type, "gettransaction", dOrder->getSender(count), "", "", "");
    break;
    case 12: /// move  <account> <address> <amount>
        jCall.regularJson(json, type, dOrder->getAction(count), dOrder->getSender(count), dOrder->getRecipient(count), dOrder->getAmount(count), "");
        break;
    case 13: /// getaccount <address>
        jCall.regularJson(json, type, "getaccount", dOrder->getSender(count), "", "", "");
    break;
    case 10001: /// FEE getreceivedby
        jCall.regularJson(json, 10001, "sendfrom", confirmWallet.getAccount(), confirmWallet.getAddress(), confirmWallet.getStrAmount(), "");
        //emit setParseFee(confirmWallet.getStrAmount());
    break;
    case 10002: /// FEE Sendfrom
        jCall.regularJson(json, 10002, "sendfrom", confirmWallet.getAccount(), confirmWallet.getAddress(), confirmWallet.getStrAmount(), "");
       // emit setParseFee(confirmWallet.getStrAmount());
    break;
    case 10003: /// FEE Sendfrom
        jCall.regularJson(json, 10003, "sendfrom", dOrder->getSender(count), confirmWallet.getAddress(), confirmWallet.getStrAmount(), "");
       // emit setParseFee(confirmWallet.getStrAmount());
    break;
    case 10004: /// Modified CheckBalance (runs after getreceived confirms, to confirm balance)
        jCall.regularJson(json, 10004, "getbalance", dOrder->getSender(count), dOrder->getRecipient(count), "", "");
    break;
    case 10005: /// fees utilizing move from account to account
        jCall.regularJson(json, 10005, "move", dOrder->getSender(count), "fee", dOrder->getAmount(count), "");
    }
}

QJsonObject rpcCntrl::unlock(wrkObj *wrk, daeObj *daemon){
    qDebug() << "unlocking ----" <<endl;
    QJsonObject json;
    prevCount = count;
    coinIndex = getCoinIndex(count, daemon, wrk);
    callType = 9999;
    JsonData(json, callType, daemon, wrk, count);
    qDebug() << "json unlock ready ----" <<endl;
    Completed = false;
    return json;
}
void rpcCntrl::initFees(){
    FEE_SENDER = true;
    FEE_RECEIVE = false;
    FEE_ADMIN_WALLET = false;
}

string rpcCntrl::calcFee(string amount ,string grp_fee){

    QString finalFee = "";
    double dFee = 0, dAmount = 0, dCalc = 0;
    bool ok = false;
    dAmount = QString(amount.c_str()).toDouble(&ok);
    dFee = QString(grp_fee.c_str()).toDouble(&ok);

     dCalc = (dAmount * dFee) / 100;
     finalFee =  QString::number(dCalc, 'g', 8);
  confirmWallet.setStrAmount(finalFee.toStdString());

  //confirmWallet.setAmount(dFee);
    qDebug() << "performing FEE CALCULATION ---------" <<endl;
    qDebug() << "the original amount: " <<amount.c_str() <<endl;
    qDebug() << "our fee percent: " << dFee <<endl;
    qDebug() << "our total fee: " << dCalc <<endl;
    qDebug() << "our total String fee: " << finalFee <<endl;

    return finalFee.toStdString();
}


/*
 *  Match current Order Coin with index of Coin in list of daemons
 */
int rpcCntrl::getCoinIndex(int orderNum, daeObj *daemon, wrkObj *dOrder){
        for(int i=0; i<daemon->size; i++){
            if( daemon->getCoin(i) == dOrder->getCoin(orderNum) && daemon->getEnabled(i) == "true"){
                coinIndex =  i;
                qDebug() << coinIndex << endl;
                return i;
            }else{
                /// coin not found in index
            }
        }
}

