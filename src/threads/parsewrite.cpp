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
#include "parsewrite.h"

parseWrite::parseWrite()
{
    table = "", thread = "";
    rpcID = 0;
    coinIndex = 0;
    defaultFee = 1;
}


void parseWrite::close(){
}

bool parseWrite::getOrders(wrkObj &src, string table){
    dbConnect.fillOrders(src, table);
    return true;
}

bool parseWrite::setWorkOrderStatus(string table, int orderid, int status){
    dbConnect.updateWorkOrderStatus(table, orderid, status);
    dbConnect.updateWorkerStatus();
}

/*
 *   Parse the data read from daemon
 *  Types:
 *  0) update parseWrite order test
 *  1) getinfo
 *  2) getnewaddress
 *  3) getreceivedbyaddress
 *  4) sendfrom
 *
 */
bool parseWrite::parseAndWrite(QJsonObject *doc, int wrkNum, int type){
    string gAddress = "", ParseAccount = "", ParseAddress = "";
    wallet wall;
    QString test = "", qWallConfirm;
    double wallConfirm = 0;
    string tranID = "", balance = "", fee = "";
    bool confirm = false, feeCalc = false, walletExists = false, ReceiveConfirm = false;
    int rpcid = 0;
    double dAmount = 0;
    bool ok = false, checkbalance = false;
    switch(type){
    case 0:
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        return true;
        break;
    case 9999:
        qDebug() << "Wallet Unlocked" << endl;
        emit unlocked();
        break;
    case 2:  // get new address
        gAddress = parseNewAddress(doc);
        wall.set(wrk->getUser(wrkNum), wrk->getSender(wrkNum) ,gAddress, wrk->getAmount(wrkNum), wrk->getCoin(wrkNum), 0, "0", "-");
        confirm = 1;
        wrk->setResult("1", wrkNum);
        dbConnect.writeGeneratedWallet(&wall, wrk, wrkNum );

        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        emit parseDone();
        break;
    case 3:  // get receieved by address
        walConfirm.init();
        wallConfirm = parseConfirmBalance(doc);
        test = QString(wrk->oAmount[wrkNum].c_str());
        qWallConfirm =  QString("%1").arg(wallConfirm,0,'g',8);

        wall.set(wrk->getUser(wrkNum), wrk->getSender(wrkNum) , wrk->getSender(wrkNum), qWallConfirm.toStdString(), wrk->getCoin(wrkNum), rpcVar.str2int(wrk->getRecipient(wrkNum)), "0", "-");
        wall.setStrAmount(qWallConfirm.toStdString());

        if(rpcVar.str2int(wrk->getRecipient(wrkNum)) < rpcVar.str2int(dae->getMxConf(coinIndex))){

            /// get previous pending total
            walConfirm.setAddress(wrk->getSender(wrkNum));
            walletExists = dbConnect.getWalletPending(walConfirm);
            wall.setFlag(walConfirm.getFlag());

            if(walConfirm.getAmount() <= 0.000001 &&  wallConfirm < 0.000001){
                wrk->setResult("0", wrkNum);
                wall.setStrAmount(qWallConfirm.toStdString());
                wall.setConfirm(0);
                dbConnect.writePendingBalance(&wall, false);
                dbConnect.writeConfirmWallet(&wall, wrk->getOrderID(wrkNum), false, false, walletExists); /// update pending_total
            }else{
                if(wallConfirm >= walConfirm.getAmount()){
                    wrk->setResult("1", wrkNum);
                    wallConfirm =  wallConfirm - walConfirm.getAmount();
                    qWallConfirm =  QString("%1").arg(wallConfirm,0,'g',8);
                    wall.setStrAmount(qWallConfirm.toStdString());
                    wall.setFee("0");

                    dbConnect.writePendingBalance(&wall, false);
                    dbConnect.writeConfirmWallet(&wall, wrk->getOrderID(wrkNum), false, true, walletExists); /// update pending_total
                    dbConnect.updateTransCount(wrk->getUser(wrkNum));
                }else{
                    if(wallConfirm > 0){
                        dbConnect.writeConfirmWallet(&wall, wrk->getOrderID(wrkNum), false, true, walletExists); /// update pending_total
                    }
                    wrk->setResult("0", wrkNum);
                    qDebug() << "error the user did not deposit enough coin"   << endl;
                }
                ReceiveConfirm = true;
            }

        }else if(rpcVar.str2int(wrk->getRecipient(wrkNum)) == rpcVar.str2int(dae->getMxConf(coinIndex))){
            /// get previous pending total
            walConfirm.setAddress(wrk->getSender(wrkNum));
            walletExists = dbConnect.getWalletPending(walConfirm);
            wall.setFlag(walConfirm.getFlag());
            if(wallConfirm == 0.0 || wallConfirm == 0 || qWallConfirm == "0"){
                wrk->setResult("0", wrkNum);
            }else{
                if(wallConfirm >= walConfirm.getAmount()){
                    wrk->setResult("1", wrkNum);
                    qWallConfirm =  QString("%1").arg(wallConfirm,0,'g',8);
                    wall.setStrAmount(qWallConfirm.toStdString());
                    wall.setFee("0");
                    dbConnect.writeConfirmWallet(&wall, wrk->getOrderID(wrkNum), true, true, walletExists);   /// update balance_total + no more flag
                    dbConnect.writeConfirmedBalance(&wall, false);
                    checkbalance = true;
                }else{
                    if(wallConfirm > 0.0000001){
                        qWallConfirm =  QString("%1").arg(wallConfirm,0,'g',8);
                        wall.setStrAmount(qWallConfirm.toStdString());
                        wall.setFee("0");
                        dbConnect.writeConfirmWallet(&wall, wrk->getOrderID(wrkNum), true, true, walletExists);   /// update balance_total
                        dbConnect.writeConfirmedBalance(&wall, false);
                        checkbalance = true;
                    }
                    wrk->setResult("0", wrkNum);
                    qDebug() << "error the user did not deposit enough coin"   << endl;
                }
                ReceiveConfirm = true;
            }
        }
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        if(!checkbalance){
            emit parseDone();
        }else{
            emit parseDoneBalance();
        }
        break;
    case 4:   // send from
        tranID = parseTransaction(doc);

        /// calc the upcoming fee
        dAmount = QString(wrk->getAmount(wrkNum).c_str()).toDouble(&ok);

        fee = getStrFee(wallConfirm, rpcid, wrkNum);
        if(fee != "0"){   // valid fee amount
            confirm =true;
        }else{
            confirm = false;
        }
        wall.setFee(fee);
        wrk->setResult("1", wrkNum);

        /// add transaction to history
        dbConnect.writeTransaction(wrk, wrkNum, tranID, fee);
        wall.set(wrk->getUser(wrkNum), wrk->getSender(wrkNum), "", wrk->getAmount(wrkNum), wrk->getCoin(wrkNum), 0, fee, "-");

        /// update ccbalance - for withdrawal
        dbConnect.writePendingBalance(&wall, true);
        dbConnect.updateTransCount(wrk->getUser(wrkNum));
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        emit parseDoneAddFee();
        break;
    case 5:   // getbalance
        balance = parseBalance(doc);
        dbConnect.writeUpdatedBalance(wrk->getUser(wrkNum), balance, wrk->getCoin(wrkNum));
        wrk->setResult("1", wrkNum);
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        emit parseDone();
        break;
    case 6:    /// getaccount
        /// determine if we're doing fee for withdrawal or confirmation
        ParseAccount = parseNewAddress(doc);
        walConfirm.setAccount(ParseAccount);
        emit SenderFound(ParseAccount);
        qDebug() << "The address we're accessing fee from is owned by :" << ParseAccount.c_str() << endl;
        break;
    case 7:    /// getaccountaddress
        /// determine if we're doing fee for withdrawal or confirmation
        ParseAccount = parseNewAddress(doc);
        walConfirm.setAddress(ParseAccount);
        emit parseDone();
        qDebug() << "The address we're accessing is owned by :" << ParseAccount.c_str() << endl;
        break;
    case 8:    /// getaccountaddress // fee wallet
        ParseAddress = parseNewAddress(doc);
        walConfirm.setAddress(ParseAddress);
        emit parseDone();
        qDebug() << "The account we're accessing fee has an adress of:" << ParseAddress.c_str() << endl;
        break;
    case 9:   // settxfee
        qDebug() << "txfee for: "<< dae->getCoin(coinIndex).c_str() <<" added successfully!" << endl;
        emit parseDone();
        break;
    case 10:  // gettransaction
        ///get transaction ID
        parseTransactionByID(wall, doc, wrkNum);
        dAmount = QString(wrk->getAmount(wrkNum).c_str()).toDouble(&ok);
        fee = getStrFee(wallConfirm, rpcid, wrkNum);
        wall.setFee(fee);
        dbConnect.updateTransaction(&wall, wrkNum, wrk->getSender(wrkNum).c_str());

        /// check if We've reached the max confirms
        if(wall.getConfirm() >= rpcVar.str2int(dae->getMxConf(coinIndex))){
            dbConnect.confirmWithdrawal(&wall, wrkNum);\
        }
        wrk->setStatus("1", wrkNum);
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        emit parseDone();
        break;

    case 12:  ///  move
         qDebug() << "move complete" << endl;
        emit parseDone();

    case 13:    /// getaccount  <address>
        qDebug() << "The account we're accessing from is owned by :" << ParseAccount.c_str() << endl;
        ParseAccount = parseNewAddress(doc);
        emit parseDoneAccount(ParseAccount);
        break;

    case 10001:   //fees
        qDebug() << "test" << endl;
        /// add transaction to history
        walConfirm.setAmount(walConfirm.getAmount() +  rpcVar.str2dbl(dae->getTxFee(coinIndex)));
        wall.set(wrk->getUser(wrkNum), wrk->getSender(wrkNum), wrk->getSender(wrkNum), walConfirm.getStrAmount(), wrk->getCoin(wrkNum), rpcVar.str2int(dae->getMxConf(coinIndex)), "0", "-");
        emit parseDone();
        return true;
        break;
    case 10002:   //service charges
        qDebug() << "service charge complete" << endl;
        /// add transaction to history
        fee = controlFee(wrkNum);
        wall.setFee(fee);
        wall.set(wrk->getUser(wrkNum), wrk->getSender(wrkNum), wrk->getSender(wrkNum), walConfirm.getStrAmount(), wrk->getCoin(wrkNum), rpcVar.str2int(dae->getMxConf(coinIndex)), "0", "-");
        dbConnect.writeBillingSummary(&wall, fee, wrk->getGrpID(wrkNum));  //// CHANGEME
        dbConnect.writeBill(&wall, wrk->getGrpID(wrkNum), grp->getName(grp->discoverIDPos(wrk->getGrpID(wrkNum))));
        wrk->setResult("1", wrkNum);
        wrk->setStatus("1", wrkNum);
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        emit parseDone();
        return true;
        break;
    case 10004:  // getbalance modified
        qDebug() << "balance modified call after getreceived" << endl;
        balance = parseBalance(doc);
        dbConnect.writeModifiedBalance(wrk->getUser(wrkNum), balance, wrk->getCoin(wrkNum));
        wrk->setResult("1", wrkNum);
        dbConnect.updateWorkOrders(wrk, table, wrkNum, confirm);
        emit parseDone();
        break;
    case 10005:  /// move fees
        qDebug() << "fees complete" << endl;
        dbConnect.writeFeeBalance(wrk->getUser(wrkNum), wrk->getAmount(wrkNum), wrk->getCoin(wrkNum), false);  // remove from sender's balance
        dbConnect.writeFeeBalance(wrk->getUser(wrkNum), wrk->getAmount(wrkNum), wrk->getCoin(wrkNum), true);   // add to recipient's balance

        emit parseDone();
        break;
    }
    return false;
}

bool parseWrite::invalidDebug(QJsonObject *jsonArr, int wrkNum, int invalidType){
    stringstream Msg;

    if(invalidType == 1){  /// json ID didn't match!
        Msg << " ERROR INCORRECT RESPONSE ID FROM RPC when it should be " << wrkNum;
        qDebug() << Msg.str().c_str() << endl;
    }
    else if(invalidType == 2){   /// json reply gave error!
        double sErrorCode = 0;
        sErrorCode = parseErrorCode(jsonArr);
        qDebug() << jsonArr;  // test json output

        if(sErrorCode == -12){     ///for refill key
            Msg <<" REFILLING KEYS --------------------------" ;
            qDebug() <<  Msg.str().c_str() << endl;
            emit unlockWallet();
        }
        else if(sErrorCode == -15){
            Msg << " unlocking wallet --------------------------" ;
            qDebug() <<  Msg.str().c_str() << endl;
        }
        else if(sErrorCode == -13){
            Msg <<" Wallet needs unlock " << " ..unlocking wallet --------------------------" ;
            qDebug() <<  Msg.str().c_str() << endl;
            emit unlockWallet();
        }
        else if(sErrorCode == -17){
            Msg << "wallet already unlocked code : " << sErrorCode;
            qDebug() <<  Msg.str().c_str() << endl;
            emit unlocked();
        }
        else if(sErrorCode == -1){
            Msg << "wallet is locked, can't withdraw ! ";
            qDebug() <<  Msg.str().c_str() << endl;
            emit unlockWallet();
        }
        else if(sErrorCode == 0){
            Msg <<  "error code : " << sErrorCode;
            qDebug() <<  Msg.str().c_str() << endl;
            dbConnect.updateWorkOrderStatus(table, wrk->getOrderID(wrkNum), 2);
            emit parseDone();

        }else if(sErrorCode == -6){
            Msg << "WARNING empty wallet  : " << sErrorCode;
            qDebug() <<  Msg.str().c_str() << endl;
            wrk->setStatus("1", wrkNum);
            wrk->setResult("0", wrkNum);
            qDebug() << "UPDATING parseWrite ORDERS -----------" << endl;
            dbConnect.updateWorkOrderStatus(table, wrk->getOrderID(wrkNum), 2);
            emit parseDone();

        }else if(sErrorCode == -3){
            Msg << "WARNING Send Amount to small!!  : " << sErrorCode;
            qDebug() <<  Msg.str().c_str() << endl;
            wrk->setStatus("1", wrkNum);
            wrk->setResult("0", wrkNum);
            qDebug() << "UPDATING parseWrite ORDERS -----------" << endl;
            dbConnect.updateWorkOrderStatus(table, wrk->getOrderID(wrkNum), 2);
            emit parseDone();

        } else{
            Msg << " the daemon appears to be offline or there are no more work orders";
            qDebug() <<  Msg.str().c_str() << endl;
            dbConnect.updateWorkOrderStatus(table, wrk->getOrderID(wrkNum), 2);
            emit parseDone();
        }

    }

}


void parseWrite::parseInfo(QJsonObject &doc){

    foreach (const QJsonValue & valAll, doc)
    {
        QJsonObject dataArr = valAll.toObject();

    }
}
string parseWrite::parseNewAddress(QJsonObject *doc){
    QJsonValue val = doc->value("result");
    string testing = "";
    testing = val.toString().toStdString();
    return testing;
}

string parseWrite::parseBalance(QJsonObject *doc){
    QJsonValue val = doc->value("result");
    QString final = "";
    double testing = 0;
    testing =val.toDouble();
    final =  QString("%1").arg(testing,0,'g',13);
    return final.toStdString();
}

double parseWrite::parseConfirmBalance(QJsonObject *doc){
    QJsonValue val = doc->value("result");
    string testing = "";
    double wallConfirm = 0;
    QString final = "";
    wallConfirm = val.toDouble();
    testing = val.toString().toStdString();
    final =  QString("%1").arg(wallConfirm,0,'g',8);
    return wallConfirm;
}

int parseWrite::parseID(QJsonObject *doc){
    QJsonValue val = doc->value(QString("id"));
    int rpcid = 0;
    rpcid = (int)(val.toDouble());
    return rpcid;
}

bool parseWrite::parseResult(QJsonObject *doc){
    QJsonValue val = doc->value("result");
    if(val.isNull()) {
        //     qDebug() << " RESULT IS NULL " << endl;
        return false;
    }else{
        //     qDebug() << " RESULT ISnt NULL " << endl;
        return true;
    }
}
double parseWrite::parseErrorCode(QJsonObject *reply){

    double sErrorCode = 0;

    QJsonObject testObj = reply->value(QString("error")).toObject();


    foreach(const QJsonValue & val, testObj){
        if(val.isDouble()){
            qDebug() << "ERROR CODE IS " << val.toDouble() << endl;
            sErrorCode = val.toDouble();
        }

    }

    return sErrorCode;
}

void parseWrite::parseTransactionByID(wallet &wall, QJsonObject *doc, int wrkNum){
    string testing = "";

    double amount = 0;
    int confirms = 0, counter = 0, iConfirm = 0;
    string sender = "", receiver = "", account = "";
    QString sAmount = "", qConfirms = "";
    /// get transaction confirms
    /// doc.
    QJsonObject jResult = doc->value("result").toObject();
    confirms = jResult.value("confirmations").toDouble();
    qConfirms =  QString("%1").arg(confirms,0,'g',8);

    QJsonArray jDoc = jResult.value("details").toArray();
    foreach (const QJsonValue & valAll, jDoc)
    {
        qDebug() << " IN transaction Result" << endl;
        counter++;
        QJsonObject dataArr = valAll.toObject();
        if(counter == 1 && dataArr.contains("amount")){
            amount = (dataArr["amount"].toDouble() * -1);
            sAmount =  QString("%1").arg(amount,0,'g',8);
        }
        if(dataArr.contains("account")){
            account = dataArr["account"].toString().toStdString();
        }
        if(dataArr.contains("address")){
            receiver = dataArr["address"].toString().toStdString();
        }
    }
    iConfirm = rpcVar.str2int(qConfirms.toStdString());
    wall.set(wrk->getUser(wrkNum), account, receiver, amount, wrk->getCoin(wrkNum), iConfirm , "0", "-");
    wall.setStrAmount(sAmount.toStdString());
}

string parseWrite::parseTransaction(QJsonObject *doc){
    QJsonValue val = doc->value("result");
    string testing = "";
    testing = val.toString().toStdString();
    return testing;
}

string parseWrite::controlFee(int wrkIndex){

    string fee = "";

    /// calculate fee
    if(grp->getName(grp->discoverIDPos(wrk->getGrpID(wrkIndex))) != "default"){
        fee = rpcVar.dbl2str((rpcVar.str2dbl(grp->getWithdrawFee(grp->discoverIDPos(wrk->getGrpID(wrkIndex))))));
        fee = rpcVar.dbl2str((rpcVar.str2dbl(fee) / rpcVar.str2dbl(wrk->getAmount(wrkIndex)))+rpcVar.str2dbl(dae->getTxFee(coinIndex)) );
        walConfirm.setStrAmount(fee);
    }
    return fee;
}

string parseWrite::calcFee(string amount){

    QString finalFee = "";
    double dFee = 0;
    double dAmount = 0;
    bool ok = false;
    dAmount = QString(amount.c_str()).toDouble(&ok);
    dFee = (dAmount * defaultFee) / 100;
    finalFee =  QString::number(dFee, 'g', 8);

    return finalFee.toStdString();
}


string parseWrite::getStrFee(double amount, int calltype, int wrkIndex){

    double dFee = 0.00000000;
    if(calltype == 4){
        dFee = (((amount * rpcVar.str2dbl(grp->getWithdrawFee(grp->discoverIDPos(wrk->getGrpID(wrkIndex))))) / 100) + ((rpcVar.str2dbl(dae->getTxFee(coinIndex))) * 2));
    }
    if(dFee <= (rpcVar.str2dbl(dae->getTxFee(coinIndex)))){
        dFee = rpcVar.str2dbl("0");
    }

    return rpcVar.dbl2str(dFee);
}

parseWrite::~parseWrite(){

    delete  db;
    delete  dae;
    delete grp;
    delete   wrk;
}
