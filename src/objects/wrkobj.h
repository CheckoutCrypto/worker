
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
#ifndef WRKOBJ_H
#define WRKOBJ_H

#include <QString>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#define DEFAULT 100

using namespace std;
class wrkObj
{
public:

    int *ordId, *uid, *gid, size, MAX;
    string *oAmount, *oCoin, *oSender, *oRecipient, *oAction, *oStatus, *oResult;

    double oDblAmount;

    wrkObj();
    virtual ~wrkObj();

    void init(int size);
    void reinit(int initsize);
    void set(int id, int user, int group, string amount, string coin, string sender, string recipient, string action, string result, string status);
    void insert(int pos, int id, int user, int group, string amount, string coin, string sender, string recipient, string action, string result, string status);

    void display(int orderid);
    void displayAll();
    wrkObj(const wrkObj &src);
    wrkObj& operator=(const wrkObj& src);

    int getOrderID(int idx){
        return ordId[idx];
    }

    int getUser(int idx){
        return uid[idx];
    }
    int getGrpID(int idx){
        return gid[idx];
    }

    string getAmount(int idx){
        return oAmount[idx];
    }
    double getDblAmount(int idx){
        bool ok = false;
        oDblAmount = QString(oAmount[idx].c_str()).toDouble(&ok);
        return oDblAmount;
    }
    string getCoin(int idx){
        return oCoin[idx];
    }
    string getSender(int idx){
        return oSender[idx];
    }
    string getRecipient(int idx){
        return oRecipient[idx];
    }
    string getAction(int idx){
        return oAction[idx];
    }
    string getStatus(int idx){
        return oStatus[idx];
    }
    string getResult(int idx){
        return oResult[idx];
    }

    void setInvalid(int idx){
        setStatus(0, idx);
        setResult(0, idx);
    }

    void setStatus(string status, int idx){
        oStatus[idx] = status;
    }
    void setResult(string result, int idx){
        oResult[idx] = result;
    }

private:
    void setOrderID(int order_id, int idx){
        ordId[idx] = order_id;
    }

    void setUser(int user, int idx){
        uid[idx] = user;
    }
    void setGrpID(int group, int idx){
        gid[idx] = group;
    }

    void setAmount(string amount, int idx){
        oAmount[idx] = amount;
    }
    void setCoin(string coin, int idx){
        oCoin[idx] = coin;
    }
    void setSender(string sender, int idx){
        oSender[idx] = sender;
    }
    void setRecipient(string recipient, int idx){
        oRecipient[idx] = recipient;
    }
    void setAction(string action, int idx){
        oAction[idx] = action;
    }


};

#endif // WRKOBJ_H
