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
#ifndef DAEOBJ_H
#define DAEOBJ_H

#include <QString>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#define DEFAULT 20

using namespace std;

class daeObj
{
public:
    int size, MAX;
    string *dUser, *dPass, *dServer, *dPort, *dCoin, *dWalPas, *dFee, *dTxFee, *dRate, *dMxConf, *dEnabled;
    bool *dUnlock;

    daeObj();
    virtual ~daeObj();
    void display(int coinid);
    void displayAll();
    void init(int size);
    void reinit(int size);
    void setAll(string user, string pass, string server, string port, string coin, string walpas, string fee, string txFee, string rate, string mxconf, string enabled, bool unlock);
    void set(string user, string pass, string server, string port, string coin, string walpas);
    void setSpec(int id, string user, string pass, string server, string port, string coin, string walpas);
    void setFee( string code, string fee, string txFee, string rate, string mxconf, string enabled);

    int getCoinIndex(string coin);

   daeObj(string user, string pass, string server, string port, string coin, string walpas, string fee, string txFee, string rate, string mxconf, string enabled, bool unlock);
   daeObj(const daeObj &src);
    daeObj& operator=(const daeObj& src);

    string getUser(int coinid){
        return dUser[coinid];
    }
    string getPass(int coinid){
        return dPass[coinid];
    }
    string getServer(int coinid){
        return dServer[coinid];
    }
    string getPort(int coinid){
        return dPort[coinid];
    }
    string getCoin(int coinid){
        return dCoin[coinid];
    }
    string getWalPas(int coinid){
        return dWalPas[coinid];
    }
    string getFee(int coinid){
        return dFee[coinid];
    }
    string getTxFee(int coinid){
        return dTxFee[coinid];
    }
    string getRate(int coinid){
        return dRate[coinid];
    }
    string getMxConf(int coinid){
        return dMxConf[coinid];
    }
    string getEnabled(int coinid){
        return dEnabled[coinid];
    }
    bool getUnlocked(int coinid){
        return dUnlock[coinid];
    }

private:

    void setUser(string user, int coinid){
        dUser[coinid] = user;
    }
    void setPass(string pass, int coinid){
        dPass[coinid] = pass;
    }
    void setServer(string server, int coinid){
        dServer[coinid] = server;
    }
    void setPort(string port, int coinid){
        dPort[coinid] = port;
    }
    void setCoin(string coin, int coinid){
        dCoin[coinid] = coin;
    }
    void setWalPas(string walpas, int coinid){
        dWalPas[coinid] = walpas;
    }
    void setFees(string fee, int coinid){
        dFee[coinid] = fee;
    }
    void setTxFee(string txfee, int coinid){
        dTxFee[coinid] = txfee;
    }
    void setRate(string rate, int coinid){
        dRate[coinid] = rate;
    }
    void setMxConf(string conf, int coinid){
        dMxConf[coinid] = conf;
    }
    void setEnabled(string enabled, int coinid){
        dEnabled[coinid] = enabled;
    }
    void setUnlocked(bool lock, int coinid){
        dUnlock[coinid] = lock;
    }

};

#endif // DAEOBJ_H
