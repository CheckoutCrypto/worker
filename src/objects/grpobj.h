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
#ifndef GRPOBJ_H
#define GRPOBJ_H

#include <QString>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

#define DEFAULT 100

using namespace std;

class grpobj
{
public:
    int size, MAX;
    grpobj();

    string *gName, *gWFee, *gTFee, *gXFee, *gMFee;   // withdraw, trade, exchange(conversion), monthly, fees
    int *gID;

    void set(int id, string name, string withdraw, string trade, string exchange, string monthly);
    void reinit(int initsize);
    int discoverIDPos(int id);

    void init(int initsize);
    void display();
    grpobj(const grpobj &src);
    grpobj& operator=(const grpobj& src);


    virtual ~grpobj();

    int getGID(int idx){
        return gID[idx];
    }
    string getName(int idx){
        return gName[idx];
    }
    string getWithdrawFee(int idx){
        return gWFee[idx];
    }
    string getTradeFee(int idx){
        return gTFee[idx];
    }
    string getExchangeFee(int idx){
        return gXFee[idx];
    }
    string getMonthlyFee(int idx){
        return gMFee[idx];
    }
private:


    void setGrpID(int id, int idx){
        gID[idx] = id;
    }

    void setName(string name, int idx){
            gName[idx] = name;
    }

    void setWithdrawFee(string desc, int idx){
        gWFee[idx] = desc;
    }

    void setTradeFee(string cost, int idx){
        gTFee[idx] = cost;
    }

    void setExchangeFee(string price, int idx){
        gXFee[idx] = price;
    }

    void setMonthlyFee(string price, int idx){
        gMFee[idx] = price;
    }
};

#endif // GRPOBJ_H
