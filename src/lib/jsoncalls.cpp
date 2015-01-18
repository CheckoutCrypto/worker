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

#include "jsoncalls.h"

JsonCalls::JsonCalls()
{
    callType = 0;
}
JsonCalls::~JsonCalls()
{
}

/*
 *  Get daemon HTTP url
 */
string JsonCalls::getHeaderUrl(string dUser, string dPass, string dServ, string dPort){
    stringstream myurl;
    myurl << "http://"<< dUser << ":" << dPass << "@" << dServ << ":" << dPort <<"/";
    return myurl.str();
}

/*
 *  Get daemon HTTP header
 */
QNetworkRequest JsonCalls::getRequest(string url){
    QNetworkRequest req;
    req.setUrl(QUrl(url.c_str()));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "Content-type: application/json");
    return req;
}

/*
 * create and fill unlock JSON
 */
void JsonCalls::unlockJson(QJsonObject &json, int id, string walpass, string time){
    qDebug()  << "method = " << "walletpassphrase " << true <<   "time: "<< time.c_str() << endl;
    //// unlock param
    QVariant dID(id),                                                   //RPC ID
            param(walpass.c_str()),           //RPC walletpassphrase param
            param2(time.c_str());                                   //RPC walletunlock timeout

    json = this->keyRefill(dID, param, param2, true);
}

/*
 * create and fill lock JSON
 */
void JsonCalls::lockJson(QJsonObject &json, int id, string walpass, string time){
    qDebug()  << "method = " << "walletlock" << false << endl;
    //// unlock param
    QVariant dID(id),                                                   //RPC ID
            param(walpass.c_str()),           //RPC walletpassphrase param
            param2(time.c_str());                                   //RPC walletunlock timeout

    json = this->keyRefill(dID, param, param2, false);
}

/*
 * create and fill regular JSON (ID+method 4 PARAM)
 */
void JsonCalls::regularJson(QJsonObject &json, int id, string action, string sender, string recipient, string fAmount, string confirms){

    qDebug()  << "method = " << action.c_str()  << "Param1 = " << sender.c_str()  << "Param2 = " << recipient.c_str() << "amount = " <<fAmount.c_str() << "id = " << id;

   QVariant dID(id);                                                          ///rpc request id
          QVariant name(QString(action.c_str())),                /// rpc request method
           param(QString(sender.c_str())),               /// rpc request method param1
           param2(QString(recipient.c_str())),           /// rpc request method param2
           param3(QString(confirms.c_str())),             /// rpc request confirmations
           amount(QString(fAmount.c_str()));              /// rpc request confirmations

  json = this->standardCall(dID, name, param, param2, amount, param3);
}


/*
 *  get/set RPC call action
 */
int JsonCalls::setRpcCall(string method){
    string msg = "";
    if(method == "getnewaddress"){
        callType = 2;
    }else if(method == "getreceivedbyaddress"){
        callType = 3;
    }else if(method == "sendfrom"){
        callType = 4;
    }else if(method == "getbalance"){
        callType = 5;
    }else if(method == "getaccount"){
        callType = 6;
    }else if(method == "getaccountaddress"){
        callType = 7;
    }else if(method == "getaccountaddress"){  // get fee wallet
        callType = 8;
    }else if(method == "settxfee"){  // get fee wallet
        callType = 9;
    }else if(method == "gettransaction"){
        callType = 10;
    }else if(method == "service_charge"){
        callType = 11;
    }else if(method == "move"){
        callType = 12;
    }else if(method == "getaccount"){
        callType = 13;
    }else if(method == "getbalance_service"){
        callType = 10004;
    }else{
        callType = 101010;
    }
    return callType;
}


/*
 *  Create Refill Key requests
 */
QJsonObject JsonCalls::keyRefill(QVariant id, QVariant param,  QVariant param2, bool withpass){
    QJsonObject json;
    QJsonArray all_params;
    string unlock = "";
    if(withpass){
        refillingKeys = true;
        unlock = "walletpassphrase";

    }else{
        refillingKeys = false;
        unlock = "walletlock";
    }
      QVariant name(QString(unlock.c_str()));
         json["method"] = name.toString();

         if(withpass){
             all_params.append(param.toString());
             all_params.append(param2.toInt());
         }
         json.insert("params", all_params);
         json["id"] = id.toInt();
         return json;
}

/*
 *  Create standard rpc request - id, method, param, param2, confirm
 */
QJsonObject JsonCalls::standardCall(QVariant id, QVariant name,  QVariant param, QVariant param2, QVariant amount, QVariant confirms){

    QJsonObject json;
    QJsonArray all_params;

    json["method"] = name.toString();
    if(callType == 2){    /// gen wallet - account
           all_params.append(param.toString());
    }
    else if(callType == 3){    /// confirm transaction  - wallet - confirm Num
        all_params.append(param.toString());
        all_params.append(param2.toInt());
    }else if(callType == 4 || callType == 11 || callType == 10001 || callType == 10002 || callType == 10003 || callType == 12 || callType == 10005){    /// send - walletA to walletB || withdrawal
        all_params.append(param.toString());
        all_params.append(param2.toString());
        all_params.append(amount.toDouble());
    }else if(callType == 5 || callType == 10004){  /// checkbalance
        all_params.append(param.toString());
        all_params.append(param2.toInt());
    }else if(callType == 6){    /// getaccount  // by address
         all_params.append(param.toString());
    }else if(callType == 7){    /// getaccount  // by address
        all_params.append(param.toString());
    }else if(callType == 8){    /// getaccount  // by address
        all_params.append(param.toString());
    }else if(callType == 9){    /// settx fee
        all_params.append(amount.toDouble());
    }else if(callType == 10){    /// gettransaction // by tranid
        all_params.append(param.toString());
    }else if(callType == 13){    /// gettransaction // by tranid
        all_params.append(param.toString());
    }

     json.insert("params", all_params);
     json["id"] = id.toInt();

     return json;
}
