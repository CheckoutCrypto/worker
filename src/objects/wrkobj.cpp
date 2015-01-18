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
#include "wrkobj.h"

wrkObj::wrkObj()
{
    size = 1;
    MAX = DEFAULT;
    oDblAmount = 0;
}
/*
 *  Initialize all work orders
 */
void wrkObj::init(int initsize){
     size = 1;
     MAX = initsize;
    ordId = new int[initsize];
    uid = new int[initsize];
    gid = new int[initsize];
    oAmount = new string[initsize];
    oCoin = new string[initsize];
    oSender = new string[initsize];
    oRecipient = new string[initsize];
    oAction = new string[initsize];
    oResult = new string[initsize];
    oStatus = new string[initsize];


    for(int i=0; i< initsize; i++){
            setOrderID(0, i);
            setUser(0, i);
            setGrpID(0, i);
            setAmount("0", i);
            setCoin("-", i);
            setSender("-", i);
            setRecipient("-", i);
            setAction("-", i);
            setStatus("0", i);
            setResult("0", i);
    }
}
/*
 *  REInitialize all work orders
 */
void wrkObj::reinit(int initsize){
    int oldsize = 0;
    int *tempuid, *tempgid, *tempuord;
    string *tempAmt, *tempCoin, *tempSender, *tempRecipient, *tempAction, *tempStatus, *tempResult;

    tempuord = new int[initsize];
    tempuid = new int[initsize];
    tempgid = new int[initsize];
    tempAmt = new string[initsize];
    tempCoin = new string[initsize];
    tempSender = new string[initsize];
    tempRecipient = new string[initsize];
    tempAction = new string[initsize];
    tempResult = new string[initsize];
    tempStatus = new string[initsize];

        /// fill temp array blan with old size
        for(int b= 0; b< size; b++){
            tempuord[b] = 0;
            tempuid[b] = 0;
            tempgid[b] = 0;
            tempAmt[b] = "0";
            tempCoin[b] = "-";
            tempSender[b] = "-";
            tempRecipient[b] = "-";
            tempAction[b] = "-";
            tempStatus[b] = "0";
            tempResult[b] = "0";
        }

        /// fill temp array with old orders
       for(int i=0; i< size; i++){
                tempuord[i] = ordId[i];
               tempuid[i] = uid[i];
               tempgid[i] = gid[i];
               tempAmt[i] = oAmount[i];
               tempCoin[i] = oCoin[i];
               tempSender[i] = oSender[i];
               tempRecipient[i] = oRecipient[i];
               tempAction[i] = oAction[i];
               tempStatus[i] = oStatus[i];
               tempResult[i] = oResult[i];
       }
        oldsize = size;


        delete [] ordId;
        delete [] uid;
        delete [] gid;
        delete [] oAmount;
        delete [] oCoin;
        delete [] oSender;
        delete [] oRecipient;
        delete [] oAction;
        delete [] oStatus;
        delete [] oResult;

        /// init old array to new size
        init(initsize);

        /// refill array
        for(int t=0; t< oldsize; t++){
            ordId[t] = tempuord[t];
           uid[t] =   tempuid[t];
           gid[t] = tempgid[t];
           oAmount[t] = tempAmt[t];
           oCoin[t] = tempCoin[t];
           oSender[t]= tempSender[t];
           oRecipient[t] = tempRecipient[t];
           oAction[t] = tempAction[t];
           oStatus[t] = tempStatus[t];
           oResult[t] = tempResult[t];
        }
        size = oldsize;

}

/*
 *  Set work order object
 */
void wrkObj::set(int id, int user, int group, string amount, string coin, string sender, string recipient, string action, string result, string status){
    if(size < MAX){
        setOrderID(id, size);
        setUser(user, size);
        setGrpID(group, size);
        setAmount(amount,size);
        setCoin(coin, size);
        setSender(sender,size);
        setRecipient(recipient, size);
        setAction(action, size);
        setStatus(status,size);
        setResult(result, size);
        size++;
    }else{
        MAX = MAX + DEFAULT;
        reinit(MAX);
        setOrderID(id, size);
        setUser(user, size);
        setGrpID(group, size);
        setAmount(amount,size);
        setCoin(coin, size);
        setSender(sender,size);
        setRecipient(recipient, size);
        setAction(action, size);
        setStatus(status,size);
        setResult(result, size);
        size++;
    }
}

/*
 *  Set work order object
 */
void wrkObj::insert(int pos, int id, int user, int group, string amount, string coin, string sender, string recipient, string action, string result, string status){

        //// if position is not the last in the array
        if(pos < size){
            bool cpyPosFound = false;
            int initsize = 0;
            int oldsize = 0;
            int *tempuid, *tempgid, *tempuord;
            string *tempAmt, *tempCoin, *tempSender, *tempRecipient, *tempAction, *tempStatus, *tempResult;

            initsize = size;
            tempuord = new int[initsize];
            tempuid = new int[initsize];
            tempgid = new int[initsize];
            tempAmt = new string[initsize];
            tempCoin = new string[initsize];
            tempSender = new string[initsize];
            tempRecipient = new string[initsize];
            tempAction = new string[initsize];
            tempResult = new string[initsize];
            tempStatus = new string[initsize];

                    /// fill temp array blan with old size
                    for(int b= 0; b< size; b++){
                        tempuord[b] = 0;
                        tempuid[b] = 0;
                        tempgid[b] = 0;
                        tempAmt[b] = "0";
                        tempCoin[b] = "-";
                        tempSender[b] = "-";
                        tempRecipient[b] = "-";
                        tempAction[b] = "-";
                        tempStatus[b] = "0";
                        tempResult[b] = "0";
             }


             /// fill temp array with old orders
              for(int i=0; i< size; i++){
                            tempuord[i] = ordId[i];
                           tempuid[i] = uid[i];
                           tempgid[i] = gid[i];
                           tempAmt[i] = oAmount[i];
                           tempCoin[i] = oCoin[i];
                           tempSender[i] = oSender[i];
                           tempRecipient[i] = oRecipient[i];
                           tempAction[i] = oAction[i];
                           tempStatus[i] = oStatus[i];
                           tempResult[i] = oResult[i];
             }
             oldsize = size;

             delete [] ordId;
             delete [] uid;
             delete [] gid;
             delete [] oAmount;
             delete [] oCoin;
             delete [] oSender;
             delete [] oRecipient;
             delete [] oAction;
             delete [] oStatus;
             delete [] oResult;
             /// init old array to new size
             init(initsize);

             /// refill array
             for(int t=0; t< oldsize; t++){
                 if(t != pos){
                     if(!cpyPosFound){
                     ordId[t] = tempuord[t];
                    uid[t] =   tempuid[t];
                    gid[t] = tempgid[t];
                    oAmount[t] = tempAmt[t];
                    oCoin[t] = tempCoin[t];
                    oSender[t]= tempSender[t];
                    oRecipient[t] = tempRecipient[t];
                    oAction[t] = tempAction[t];
                    oStatus[t] = tempStatus[t];
                    oResult[t] = tempResult[t];
                     }else{
                         ordId[t] = tempuord[t];
                        uid[t] =   tempuid[t];
                        gid[t] = tempgid[t];
                        oAmount[t] = tempAmt[t];
                        oCoin[t] = tempCoin[t];
                        oSender[t]= tempSender[t-1];
                        oRecipient[t] = tempRecipient[t];
                        oAction[t] = tempAction[t];
                        oStatus[t] = tempStatus[t];
                        oResult[t] = tempResult[t];
                     }
                 }else{
                    cpyPosFound = true;
                    ordId[t] = id;
                    uid[t] =   user;
                    gid[t] = group;
                    oAmount[t] = amount;
                    oCoin[t] = coin;
                    oSender[t]= sender;
                    oRecipient[t] = recipient;
                    oAction[t] = action;
                    oStatus[t] = result;
                    oResult[t] = status;
                 }
             }
             size = initsize;
        }else{
            set(id, user, group, amount, coin, sender, recipient, action, result, status);
        }

}


/*
 *  Output Display one specific work.c_str() order
 */
void wrkObj::display(int orderid){
    qDebug() << ordId[orderid] << " "
     << uid[orderid] << " "
     << gid[orderid] << " "
     << oAmount[orderid].c_str() << " "
     << oCoin[orderid].c_str() << " "
     << oSender[orderid].c_str() << " "
     << oRecipient[orderid].c_str() << " "
     << oAction[orderid].c_str() << " "
     << oResult[orderid].c_str() << " "
     << oStatus[orderid].c_str() << " " <<endl;
}

/*
 *  Output Display all work order
 */
void wrkObj::displayAll(){
    for(int i=1; i< size; i++){
        qDebug()  << ordId[i] << " "
          << uid[i] << " "
          << gid[i] << " "
         << oAmount[i].c_str() << " "
         << oCoin[i].c_str() << " "
         << oSender[i].c_str() << " "
         << oRecipient[i].c_str() << " "
         << oAction[i].c_str() << " "
         << oResult[i].c_str() << " "
         << oStatus[i].c_str() << " " <<endl;
    }
}

/*
 * work order copy constructor
 */
wrkObj::wrkObj(const wrkObj &src){

    init(src.size);
    for(int i=0; i< src.size; i++){
        set(src.ordId[i], src.uid[i], src.gid[i], src.oAmount[i], src.oCoin[i], src.oSender[i], src.oRecipient[i], src.oAction[i], src.oResult[i], src.oStatus[i]);
    }
}

/*
 * work order copy constructor
 */
wrkObj& wrkObj::operator=(const wrkObj& src){

    if(this != &src){
        init(src.size);
        for(int i=0; i< src.size; i++){
            set(src.ordId[i], src.uid[i], src.gid[i], src.oAmount[i], src.oCoin[i], src.oSender[i], src.oRecipient[i], src.oAction[i], src.oResult[i], src.oStatus[i]);
        }
    }
    return *this;
}

wrkObj::~wrkObj()
{
    delete [] ordId;
    delete [] oAmount;
    delete [] oCoin;
     delete [] oSender;
     delete [] oRecipient;
     delete [] oAction;
     delete [] oStatus;
     delete [] oResult;
}
