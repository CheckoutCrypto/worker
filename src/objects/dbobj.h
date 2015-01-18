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
#ifndef DBOBJ_H
#define DBOBJ_H


#include <QString>
#include <QDebug>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class dbObj
{
public:

    string dHost, dName, dUser, dPass;

    dbObj();
    virtual ~dbObj();

    void set(string host, string name, string user, string pass);

    void display();

    dbObj(string host, string name, string user, string pass);
    dbObj(const dbObj &src);
    dbObj& operator=(const dbObj& src);

    string getHost(){
        return dHost;
    }
    string getName(){
        return dName;
    }
    string getUser(){
        return dUser;
    }
    string getPass(){
        return dPass;
    }


private:

    void setHost(string host){
        dHost = host;
    }
    void setName(string name){
        dName = name;
    }
    void setUser(string user){
        dUser = user;
    }
    void setPass(string pass){
        dPass = pass;
    }
};

#endif // DBOBJ_H
