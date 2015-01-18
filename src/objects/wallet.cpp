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
#include "wallet.h"

wallet::wallet()
{
    init();
}

void wallet::init(){
    wAddress = "";
    wAccount = "";
    wCoin = "";
    wAmount = 0;
    wUID = 0;
    wConfirm = 0;
    wFee = "";
    wFlag = "";
}

wallet::~wallet(){

}
void wallet::set(int uid, string account, string address, string amount, string coin, int confirm, string fee, string flag){
    setAccount(account);
    setUser(uid);
    setAddress(address);
    setStrAmount(amount);
    setCoin(coin);
    setConfirm(confirm);
    setFlag(flag);
}

void wallet::set(int uid, string account, string address, double amount, string coin, int confirm, string fee, string flag){
    setAccount(account);
    setUser(uid);
    setAddress(address);
    setAmount(amount);
    setCoin(coin);
    setConfirm(confirm);
    setFee(fee);
    setFlag(flag);
}


void wallet::display(){

    qDebug() << " uid: " << getUser()
             << " account: " << getAccount().c_str()
             << " wallet: " << getAddress().c_str()
             << " amount: " << getAmount()
             << " coin: " << getCoin().c_str()
             << " confirm: " << getConfirm()
            << " flag: " << getFlag().c_str() << endl;
}

/*
 * work order copy constructor
 */
wallet::wallet(const wallet &src){
    set(src.wUID, src.wAccount, src.wAddress, src.wAmount, src.wCoin, src.wConfirm, src.wFee, src.wFlag);
}


/*
 * work order copy constructor
 */
wallet& wallet::operator=(const wallet& src){

    if(this != &src){
              set(src.wUID, src.wAccount, src.wAddress, src.wAmount, src.wCoin, src.wConfirm, src.wFee, src.wFlag);
    }
    return *this;
}
