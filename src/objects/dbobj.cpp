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
#include "dbobj.h"

dbObj::dbObj()
{
    dHost = "";
    dName = "";
    dUser = "";
    dPass = "";
}

dbObj::~dbObj(){

}

dbObj::dbObj(string host, string name, string user, string pass){
    set( host,  name,  user,  pass);
}


void dbObj::set(string host, string name, string user, string pass){
    setHost(host);
    setName(name);
    setUser(user);
    setPass(pass);
}

void dbObj::display(){

    qDebug() << " host: " << getHost().c_str()
             << " name: " << getName().c_str()
             << " user: " << getUser().c_str()
             << " pass: " << getPass().c_str() << endl;
}

/*
 * work order copy constructor
 */
dbObj::dbObj(const dbObj &src){
    set(src.dHost, src.dName, src.dUser, src.dPass);
}


/*
 * work order copy constructor
 */
dbObj& dbObj::operator=(const dbObj& src){

    if(this != &src){
              set(src.dHost, src.dName, src.dUser, src.dPass);
    }
    return *this;
}
