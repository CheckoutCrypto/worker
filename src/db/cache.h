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
#ifndef cache_H
#define cache_H

#include <QtSql>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "src/objects/daeobj.h"
#include "src/objects/dbobj.h"
#include "src/objects/wrkobj.h"
#include "src/objects/grpobj.h"

#include "src/lib/simplecrypt.h"

#define TEMPCACHEFOLD "/.cache/worker2"
#define TEMPCACHE "/.cache/worker2/dbcache.txt"
#define TEMPCACHEDB "/.cache/worker2/cache.db"
#define APICache "11231231231"
#define APIKey "workerserver"

using namespace std;
class cache
{
public:
    string DBlocation;
    QString pDecrypt;
    QSqlDatabase db;

    cache();
    cache(const cache& src);
    cache& operator=(const cache& src);
    virtual ~cache();

    void control();
    void createLocalDB();
    void createCache();
    void setInitDB();

    void OpenDB();
    bool initDB();

    void readDB(dbObj &src);
    void readDae(daeObj &src);
    void readGrp(grpobj &src);

    int getKeyByCoin(string coin);
    int getKeyByGrp(string grp);

    void updateDB(dbObj *src);
    void updateDae(daeObj *src, int key, int id);
    void updateGrp(grpobj *src, int key, int id);

    void removeDae(string coin);

    void writeDB();

    void writeDaemon(daeObj *src, int);
    void writeDB(dbObj *src);
    void writeGrp(grpobj *src, int);

    void writeMe(string qry);

    void generateAPI();
    string checkAPI();
    void closeDB();


    void dropTable(int mode);



    string setDecrypt(string decrypt){

       pDecrypt = QString(decrypt.c_str());
       return decrypt;
    }
    void setSQL(string strLocate){
       DBlocation = strLocate;
    }
    string getSQL(){
        return getenv("HOME") + DBlocation;
    }


};

#endif // cache_H
