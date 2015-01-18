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
#include "control.h"

control::control()
{
    dae.init(20);
    API = "";
}

void control::initializeServer(){
    /// start menu and server
    serv.moveToThread(&sThread);
    QObject::connect( & mThread, SIGNAL( startServer() ),
       & serv, SLOT( begin() ) );
    QObject::connect( this, SIGNAL( startServer() ),
       & serv, SLOT( begin() ) );

    //// Initialize server daemon and db
   serv.setParamObjects(grp, db, dae, API);
   grp.~grpobj();
   db.~dbObj();
   dae.~daeObj();

    sThread.start();
    sThread.moveToThread(&mThread);
    mThread.start();
}

control::~control()
{
    sThread.exit(1);
    mThread.exit(1);
}

string control::initCache(string mnPass){
    menu mnu;
    int oldsize = 0;
    /// Ask, then set, cache password for decryption
    if(mnPass == ""){
        mnPass = mnu.getPassword(cah);
        cah.setDecrypt(mnPass);
    }
    /// Create Tables if we haven't already
    cah.control();
    // Read DB/DAE/GRP objects
    cah.readDB(db);
    cah.readDae(dae);
    cah.readGrp(grp);

    // refresh data from remote database
    if(db.dHost != ""){
        dbconnect.setSQL(&db);
        oldsize = dae.size;
        dbconnect.fillCoin(dae);
        // Insert synced objects
        int key = 0, id =0;
        if(oldsize > 0){  // update daemons
            for(int x=0; x < dae.size; x++){
                key = cah.getKeyByCoin(dae.getCoin(x));
                cah.updateDae(&dae, key, x);
            }
        }
        oldsize = grp.size;
        dbconnect.fillGroups(grp);

        if(oldsize > 1){  // update groups
            for(int y=1; y< grp.size; y++){
                key = cah.getKeyByGrp(grp.getName(y));
                cah.updateGrp(&grp, key, y);
            }
        }else{   /// Insert new group
            qDebug() << "inserting groups" << endl;
            for(int y=1; y< grp.size; y++){
                cah.writeGrp(&grp, y);
            }
        }
    }
    /// select API from local cache
    API = cah.checkAPI();

    return mnPass;
}

void control::printCache(){
    qDebug() << "CACHE............" <<endl;
    dae.displayAll();
    db.display();
    qDebug() << "API: "<< API.c_str() << endl;
}

void control::main(string mnPass){
    mnPass = initCache(mnPass);
    menu mnu;
    mnu.main(cah, mnPass);
    initializeServer();
}
