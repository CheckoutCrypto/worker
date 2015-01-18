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
#include "daeobj.h"

daeObj::daeObj()
{
    size =0;
    MAX = DEFAULT;
    init(20);
}

daeObj::daeObj(string user, string pass, string server, string port, string coin, string walpas, string fee, string txFee, string rate, string mxConf, string enabled, bool unlock)
{
    size=0;
    setAll(user, pass, server, port, coin, walpas, fee, txFee, rate, mxConf, enabled, unlock);
}

void daeObj::init(int dSize){
     size=0;
     if(dSize < MAX){
         dSize = MAX;
     }
    dUser = new string[dSize];
    dPass = new string[dSize];
    dServer =new string[dSize];
    dPort = new string[dSize];
    dCoin = new string[dSize];
    dWalPas = new string[dSize];
    dTxFee = new string[dSize];
    dFee = new string[dSize];
    dRate = new string[dSize];
    dMxConf = new string[dSize];
    dEnabled = new string[dSize];
    dUnlock = new bool[dSize];

    for(int i=0; i<dSize; i++){
        setUser("-", i);
        setPass("-", i);
        setServer("-", i);
        setPort("-", i);
        setCoin("-", i);
        setWalPas("-", i);
        setTxFee("-", i);
        setFees("-", i);
        setRate("-", i);
        setMxConf("-", i);
        setEnabled("true", i);
        setUnlocked(false, i);
    }
}

/*
 * Reinit Array (scale)
 */
void daeObj::reinit(int scale){
    /// create initial blank COPY array
    int oldsize = 0;
    string *tempUser, *tempPass, *tempServer, *tempPort, *tempCoin, *tempWalPas, *tempTxFee, *tempFee, *tempRate, *tempMxConf, *tempEnabled;
    bool *tempUnlock;

    tempUser = new string[size];
    tempPass = new string[size];
    tempServer =new string[size];
    tempPort = new string[size];
    tempCoin = new string[size];
    tempWalPas = new string[size];
    tempTxFee = new string[size];
    tempFee = new string[size];
    tempRate = new string[size];
    tempMxConf = new string[size];
    tempEnabled = new string[size];
    tempUnlock = new bool[size];

    for(int i=0; i<size; i++){
        tempUser[i] = "-";
        tempPass[i] = "-";
        tempServer[i] = "-";
        tempPort[i] = "-";
        tempCoin[i] = "-";
        tempWalPas[i] = "-";
        tempTxFee[i] = "-";
        tempFee[i] = "-";
        tempRate[i] = "-";
        tempMxConf[i] = "-";
        tempEnabled[i] = "true";
        tempUnlock[i] = false;
    }

    /// copy original array
    for(int i=0; i<size; i++){
        tempUser[i] = dUser[i];
        tempPass[i] = dPass[i];
        tempServer[i] = dServer[i];
        tempPort[i] = dPort[i];
        tempCoin[i] = dCoin[i];
        tempWalPas[i] = dWalPas[i];
        tempTxFee[i] = dTxFee[i];
        tempFee[i] = dFee[i];
        tempRate[i] = dRate[i];
        tempMxConf[i] = dMxConf[i];
        tempEnabled[i] = dEnabled[i];
        tempUnlock[i] = dUnlock[i];
    }
    oldsize = size;
    /// delete original array
    delete [] dUser;
    delete [] dPass;
    delete [] dServer;
    delete [] dPort;
    delete [] dCoin;
    delete [] dWalPas;
    delete [] dTxFee;
    delete [] dFee;
    delete [] dRate;
    delete [] dMxConf;
    delete [] dEnabled;
    delete [] dUnlock;
    /// recreate original array
    init(scale);
    /// refill it
    for(int x=0; x<oldsize; x++){
        dUser[x] = tempUser[x];
        dPass[x] = tempPass[x];
        dServer[x] = tempServer[x];
        dPort[x] = tempPort[x];
        dCoin[x] = tempCoin[x];
        dWalPas[x] = tempWalPas[x];
        dTxFee[x] = tempTxFee[x];
        dFee[x] = tempFee[x];
        dRate[x] = tempRate[x];
        dMxConf[x] = tempMxConf[x];
        dEnabled[x] = tempEnabled[x];
        dUnlock[x] = tempUnlock[x];
    }
    size = oldsize;
}

daeObj::daeObj(const daeObj &src){

    init(src.size);
    for(int i=0; i< src.size; i++){
        //size =0;
        setAll(src.dUser[i], src.dPass[i], src.dServer[i], src.dPort[i], src.dCoin[i], src.dWalPas[i], src.dFee[i], src.dTxFee[i], src.dRate[i], src.dMxConf[i], src.dEnabled[i], src.dUnlock[i]);
    }
}

daeObj& daeObj::operator=(const daeObj& src){

    if(this != &src){
        init(src.size);
        for(int i=0; i< src.size; i++){
           // size =0;
            setAll(src.dUser[i], src.dPass[i], src.dServer[i], src.dPort[i], src.dCoin[i], src.dWalPas[i], src.dFee[i], src.dTxFee[i], src.dRate[i], src.dMxConf[i], src.dEnabled[i], src.dUnlock[i]);
        }
    }
    return *this;
}
/*
 *  Set standard cached daemon obj
 */
void daeObj::set(string user, string pass, string server, string port, string coin, string walpas){
    if(size < MAX){
        setUser(user, size);
        setPass(pass, size);
        setServer(server, size);
        setPort(port, size);
        setCoin(coin, size);
        setWalPas(walpas, size);
        size++;
    }else{
        MAX = MAX + DEFAULT;
        reinit(MAX);
        setUser(user, size);
        setPass(pass, size);
        setServer(server, size);
        setPort(port, size);
        setCoin(coin, size);
        setWalPas(walpas, size);
        size++;
    }
}
/*
 *  Modify standard cached daemon obj by its id
 */
void daeObj::setSpec(int id, string user, string pass, string server, string port, string coin, string walpas){
    setUser(user, id);
    setPass(pass, id);
    setServer(server, id);
    setPort(port, id);
    setCoin(coin, id);
    setWalPas(walpas, id);
}
/*
 *  Set complete daemon obj with fees etc
 */
void daeObj::setAll(string user, string pass, string server, string port, string coin, string walpas, string fee, string txFee, string rate, string maxConf, string enabled, bool unlock){
    setUser(user, size);
    setPass(pass, size);
    setServer(server, size);
    setPort(port, size);
    setCoin(coin, size);
    setWalPas(walpas, size);
    setTxFee(txFee, size);
    setFees(fee, size);
    setRate(rate, size);
    setMxConf(maxConf, size);
    setEnabled(enabled, size);
    setUnlocked(unlock, size);

    size++;
}
/*
 *  Set the ccdev_coin var grabbed from server db
 */
void daeObj::setFee(string coin, string rate, string fee, string txFee, string maxConf, string enabled){
    for(int i=0; i<size; i++){
        if(getCoin(i) == coin){
            setTxFee(txFee, i);
            setFees(fee, i);
            setRate(rate, i);
            setMxConf(maxConf, i);
            setEnabled(enabled, i);
        }
    }
}


void daeObj::display(int coinid){
    cout << "printing " << dUser[coinid].c_str() << " " << getPass(coinid).c_str() << endl;
}
void daeObj::displayAll(){
    int count = 1;
    qDebug() << "(1)User " <<endl << "(2)Pass" <<endl << "(3)Server" <<endl << "(4)Port" <<endl << "(5)Coin" <<endl
             << "(6)WalPass" <<endl << "(7)OurCoinFee" <<endl << "(8)TxFee(Miners Fee)" <<endl << "(9)Rate" <<endl
             << "(10)MaxConfirm" <<endl << "(11)Enabled/Disabled" << endl << "(12)Unlock" << endl;
    for(int coinid=0; coinid<size; coinid++){
        count=1;
        cout << "COIN #" <<coinid << " ";
        cout << count << ") " << dUser[coinid].c_str() << " ";
        count++;
        cout << count<< ") "  << dPass[coinid].c_str() << " ";
        count++;
        cout << count<< ") " << dServer[coinid].c_str() << " ";
        count++;
        cout << count << ") " <<  dPort[coinid].c_str()  << " ";
        count++;
        cout << count << ") " << dCoin[coinid].c_str() << " ";
        count++;
        cout << count << ") "  << dWalPas[coinid].c_str() << " ";
        count++;
        cout << count << ") " << dFee[coinid].c_str() << " ";
        count++;
        cout << count << ") " << dTxFee[coinid].c_str() << " ";
        count++;
        cout << count << ") " << dRate[coinid].c_str() << " ";
        count++;
        cout  << count << ") " << dMxConf[coinid].c_str() << " ";
        count++;
        cout  << count << ") " << dEnabled[coinid].c_str() << endl;
        count++;
        cout  << count << ") " << dUnlock[coinid] << endl;
    }
}

int daeObj::getCoinIndex(string coin){
 for(int x=0; x<= size; x++){
     if(getCoin(x) == coin){
         return x;
     }
 }
}

daeObj::~daeObj(){

    delete [] dUser;
    delete [] dPass;
    delete [] dServer;
    delete [] dPort;
    delete [] dCoin;
    delete [] dWalPas;
    delete [] dFee;
    delete [] dTxFee;
    delete [] dRate;
    delete [] dMxConf;
    delete [] dEnabled;
    delete [] dUnlock;
}
