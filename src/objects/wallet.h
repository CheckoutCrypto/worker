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
#ifndef WALLET_H
#define WALLET_H


#include <QString>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>


using namespace std;

class wallet
{
public:

    int wUID, wConfirm;
    string wAccount, wAddress, wCoin, wStrAmount, wFlag;
    double wAmount;
    string wFee;

    wallet();
    virtual ~wallet();
    void display();
    void init();
    void set(int uid, string account, string address, double amount, string coin, int confirms, string fee, string flag);
    void set(int uid, string account, string address, string amount, string coin, int confirm,  string fee, string flag);
   wallet(const wallet &src);
    wallet& operator=(const wallet& src);

    int getUser(){
        return wUID;
    }
    string getAccount(){
        return wAccount;
    }
    string getAddress(){
        return wAddress;
    }
    double getAmount(){
        return wAmount;
    }
    string getStrAmount(){
        return wStrAmount;
    }
    string getCoin(){
        return wCoin;
    }
    string getFee(){
        return wFee;
    }
    string getFlag(){
        return wFlag;
    }

    int getConfirm(){
        return wConfirm;
    }
    void setAddress(string address){
        wAddress = address;
    }
    void setAccount(string account){
        wAccount = account;
    }
    void setStrAmount(string amount){
        wStrAmount = amount;
        bool ok = false;
        wAmount = QString(amount.c_str()).toDouble(&ok);
    }
    void setAmount(double amount){
        wAmount = amount;
       wStrAmount  = QString::number(amount).toStdString();
    }
    void setConfirm(int confirm){
        wConfirm = confirm;

    }
    void setFee(string fee){
        wFee = fee;
    }

    void setFlag(string flag){
        wFlag = flag;
    }

private:

    void setCoin(string coin){
        wCoin = coin;
    }
    void setUser(int userid){
        wUID = userid;
    }

};

#endif // WALLET_H
