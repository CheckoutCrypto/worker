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
#ifndef JSONCALLS_H
#define JSONCALLS_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QDebug>
#include <sstream>
#include <stdlib.h>


using namespace std;
class JsonCalls
{
public:
    bool refillingKeys;
    int callType;
    JsonCalls();
    virtual ~JsonCalls();


    void unlockJson(QJsonObject &json, int id, string walpass, string time);
    void lockJson(QJsonObject &json, int id, string walpass, string time);
    void feeJson(QJsonObject &json, int id, string sender, string recipient, string amount, string confirms);

    void regularJson(QJsonObject &json, int id, string action, string sender, string recipient, string fAmount, string confirms);

    int setRpcCall(string method);
    QJsonObject keyRefill(QVariant id, QVariant param,  QVariant param2, bool withpass);
    QJsonObject standardCall(QVariant id, QVariant name,  QVariant param, QVariant param2, QVariant amount, QVariant confirms);

    string getHeaderUrl(string dUser, string dPass, string dServ, string dPort);
    QNetworkRequest getRequest(string url);

    string int2str(int number)
    {
       stringstream ss;//create a stringstream
       ss << number;//add number to the stream
       return ss.str();//return a string with the contents of the stream
    }

    double str2dbl (const string &str) {
      double num = 0.0;
      num = atof(str.c_str());
      return num;
    }

    string dbl2str (double dbl){
        QString qResult = "";
        qResult = QString("%1").arg(dbl,0,'g',8);
        return qResult.toStdString();
    }


    int str2int (const string &str) {
      stringstream ss(str);
      int num = 0;
      if((ss >> num).fail())
      {
          //ERROR
      }
      return num;
    }
};

#endif // JSONCALLS_H
