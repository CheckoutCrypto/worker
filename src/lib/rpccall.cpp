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
#include "rpccall.h"

rpcCall::rpcCall()
{
}

rpcCall::~rpcCall()
{
}

void rpcCall::close(){
}

/*
 *  Get Daemon Request Param - take json data and convert to QByteArray
 */
QByteArray rpcCall::getRequestParam(QJsonObject *json){
    QJsonDocument doc(*json);
   QString result(doc.toJson());
   QByteArray postData;
    postData.append(result);
    return postData;
}

/*
 *   Make Coin Daemon RPC Call
 */
void rpcCall::makeCall(QNetworkAccessManager *nam, QNetworkRequest req, QJsonObject *json, int ID, int type, int orderid){
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(DaemonReply(QNetworkReply*)),Qt::AutoConnection);
    QByteArray postData = getRequestParam(json);

    QNetworkReply * reply = nam->post(req,postData);
}

/*
 *   Respond to Coin Daemon RPC Reply
 */
void rpcCall::DaemonReply(QNetworkReply *src){
         QString strReply = (QString)src->readAll();
         QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
         QJsonObject jsonArr = jsonResponse.object();
    int rpcid = 0;
    string sRpcID = "";
    QJsonValue val = jsonArr.value(QString("id"));
    rpcid = (int)(val.toDouble());
   // qDebug() << jsonArr << endl;  // test json output

    if(src->error() == QNetworkReply::NoError) { ////  <----- VALID REPLY

             if(rpcid != 0){
                 qDebug() << jsonArr << endl;  // test json output (remove in PROD)
                 emit parseAndWrite(&jsonArr);
            }
         }else{
            /* if(rpcid == 9999){
                 emit unlocked();
             } */
             if(rpcid != 0){
                 qDebug() << jsonArr << endl;  // test json output (remove in PROD)
                emit parseError(&jsonArr);
             }
         }
    src->deleteLater();
}
