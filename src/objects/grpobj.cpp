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
#include "grpobj.h"

grpobj::grpobj()
{
    size = 1;
    MAX = DEFAULT;
}

void grpobj::init(int initsize){
    size = 1;
    MAX = initsize;
    gID = new int[initsize];
    gName = new string[initsize];
    gWFee = new string[initsize];
    gTFee = new string[initsize];
    gXFee = new string[initsize];
    gMFee = new string[initsize];

    for(int i=0; i<initsize; i++){
        setGrpID(0, i);
        setName("-", i);
        setWithdrawFee("-", i);
        setTradeFee("-", i);
        setExchangeFee("-", i);
        setMonthlyFee("-", i);
    }

}

void grpobj::set(int id, string name, string wfee, string tfee, string xfee, string mfee){

    if(size < MAX){
        setGrpID(id, size);
        setName(name, size);
        setWithdrawFee(wfee, size);
        setTradeFee(tfee, size);
        setExchangeFee(xfee, size);
        setMonthlyFee(mfee,size);
        size++;
    }else{
        MAX = MAX + DEFAULT;
        reinit(MAX);
        setGrpID(id, size);
        setName(name, size);
        setWithdrawFee(wfee, size);
        setTradeFee(tfee, size);
        setExchangeFee(xfee, size);
        setMonthlyFee(mfee,size);
        size++;
    }

}


void grpobj::reinit(int initsize){

    int oldsize = 0;
    int *tempgid;
    string *tempName, *tempWFee, *tempTFee, *tempXFee, *tempMFee;

    tempgid = new int[initsize];
    tempName = new string[initsize];
    tempWFee = new string[initsize];
    tempTFee = new string[initsize];
    tempXFee = new string[initsize];
    tempMFee = new string[initsize];

    /// fill temp array blan with old size
    for(int b= 0; b< size; b++){
        tempgid[b] = 0;
        tempName[b] = '-';
        tempWFee[b] = '-';
        tempTFee[b] = '-';
        tempXFee[b] = '-';
        tempMFee[b] = '-';
    }

    /// fill temp array with old orders
    for(int i=0; i< size; i++){
        tempgid[i] = gID[i];
        tempName[i] = gName[i];
        tempWFee[i] = gWFee[i];
        tempTFee[i] = gTFee[i];
        tempXFee[i] = gXFee[i];
        tempMFee[i] = gMFee[i];
    }
    oldsize = size;


    delete [] gID;
    delete [] gName;
    delete [] gWFee;
    delete [] gTFee;
    delete [] gXFee;
    delete [] gMFee;

    /// init old array to new size
    init(initsize);

    /// refill array
    for(int t=0; t< oldsize; t++){
        gID[t] = tempgid[t];
        gName[t] =tempName[t];
        gWFee[t] = tempWFee[t];
        gTFee[t] = tempTFee[t];
        gXFee[t] = tempXFee[t];
        gMFee[t] = tempMFee[t];
    }
    size = oldsize;
}

int grpobj::discoverIDPos(int id){
    for(int i=1; i <size; i++ ){
        if(gID[i] == id){
            return i;
        }
    }
}


void grpobj::display(){
    for(int i=1; i<size; i++){
        qDebug() << " " << gID[i] << " " << gName[i].c_str() <<
                    " " << gWFee[i].c_str() << " " << gTFee[i].c_str() << " " << gXFee[i].c_str() << " " << gMFee[i].c_str();
    }
}

/*
 * work order copy constructor
 */
grpobj::grpobj(const grpobj &src){

    init(src.size);
    for(int i=0; i< src.size; i++){
        set(src.gID[i], src.gName[i], src.gWFee[i], src.gTFee[i], src.gXFee[i], src.gMFee[i]);
    }
}

/*
 * work order copy constructor
 */
grpobj& grpobj::operator=(const grpobj& src){

    if(this != &src){
        init(src.size);
        for(int i=0; i< src.size; i++){
            set(src.gID[i], src.gName[i], src.gWFee[i], src.gTFee[i], src.gXFee[i], src.gMFee[i]);
        }
    }
    return *this;
}

grpobj::~grpobj(){
    delete [] gID;
    delete [] gName;
    delete [] gWFee;
    delete [] gTFee;
    delete [] gXFee;
    delete [] gMFee;
}
