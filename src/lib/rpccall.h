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
#ifndef RPCCALL_H
#define RPCCALL_H

#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "src/db/dataconnect.h"
#include "src/objects/dbobj.h"

class rpcCall : public QThread
{
    Q_OBJECT
public:
    QNetworkReply * reply;
    explicit rpcCall();
    virtual ~rpcCall();
    QByteArray getRequestParam(QJsonObject *json);
    void close();
public slots:
    void makeCall(QNetworkAccessManager*, QNetworkRequest, QJsonObject*, int, int, int);
    void DaemonReply(QNetworkReply *);
signals:
    void updateWorkOrderStatus(int, int);
    void completed();
    void setID(int id, int type);
    void parseAndWrite(QJsonObject *);
    void parseError(QJsonObject *);
    void unlocked();

};

#endif // RPCCALL_H
