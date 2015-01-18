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

#include "menu.h"

menu::menu()
{

}

menu::~menu()
{

}
int menu::main(cache cah, string mnPass){
    int choice = 0, SecChoice = 0;
    //// Get Decryption
    if(mnPass == ""){
        mnPass = getPassword(cah);
        cah.setDecrypt(mnPass);
    }
    choice = menuSelection();
    if(choice == 1){   // Add/Edit Database rows
        dbObj db;
        while(SecChoice <= 2){
            db = dbObj();
            SecChoice = DBMenuSelection();
            if(SecChoice == 1){   /// insert new db row
              getDBInput(db);
              db.display();
              cah.writeDB(&db);
            }else if(SecChoice == 2){ /// Edit DB row
               cah.readDB(db);
               db.display();
               getDBInputEdit(db);
               db.display();
               cah.updateDB(&db);
            }else{                   /// display DB
                cah.readDB(db);
                db.display();
            }
        }
    }else if(choice == 2){  // Add/Edit Daemons
        daeObj dae;
        while(SecChoice <= 2){
            dae.init(20);
            SecChoice = DaeMenuSelection();
            if(SecChoice == 1){
                getDaeInput(dae);
                dae.displayAll();
                cah.writeDaemon(&dae, dae.size-1);
            }else if(SecChoice == 2){
                cah.readDae(dae);
                getDaeInputEdit(dae, cah); /// display Daemon edit menu, return edited coin index
            }else if(SecChoice == 3){
                cah.readDae(dae);
                getDaeRemove(&dae, cah);
            }else if(SecChoice == 4){
                cah.readDae(dae);
                dae.displayAll();
            }else{

            }
        }
    }else if(choice == 3){  // Generate API key
        cah.generateAPI();
    }else if(choice == 4){  // Run server
        return 0;
    }else{
        return 0;
    }
    return 0;
}
/*
 * Main Worker Menu
 */
int menu::menuSelection(){
    int choice =0;
    cout << "Welcome to the Worker " << endl;
    cout << "********************************" << endl;
    cout << "================================" << endl;
    cout << "=======  CheckoutCrypto Worker  =========" << endl;
    cout << "================================" << endl;
    cout << "*********************************" << endl;
    cout << "1) Add/Edit Remote Database" << endl;
    cout << "2) Add/Edit Coin Daemon" << endl;
    cout << "3) Add/Edit API Key" << endl;
    cout << "4) Run Server" << endl;
    cout << "Enter Choice(0-4): ";
    cin >> choice;
    return choice;
}

/*
 * Get Decrypt Password
 */
string menu::getPassword(cache cah){
    int pExit = 0;
    int fCount = 0;
    string pass = "", mnPass = "";
    while(pExit != 1){
        pass ="";
        mnPass= "";
        cout << "Worker is password protected, enter pass: ";
        cin >> pass;
        mnPass = cah.setDecrypt(pass);
        if(mnPass != ""){
            cout << "Success!" << endl;
            pExit =1;
        }else{
           cout << "Incorrect password entry" << endl;
           fCount++;
           if(fCount > 5){
               cout << "Max retries" << endl;
               pExit = 0;
           }
        }
    }
    return mnPass;
}

/*
 * Select/Edit/Add Daemon
 */
int menu::DBMenuSelection(){
    int choice =0;
    cout << "DB MENU" << endl;
    cout << "1) Add DB" << endl;
    cout << "2) Edit DB" << endl;
    cout << "3) Display DB" << endl;
    cin >> choice;
    return choice;
}

/*
 * Select/Edit/Add Daemon
 */
int menu::DaeMenuSelection(){
    int choice =0;
    cout << "DAEMON MENU" << endl;
    cout << "1) Add DAEMON" << endl;
    cout << "2) Edit DAEMON" << endl;
    cout << "3) Remove DAEMON" << endl;
    cout << "4) List DAEMONS" << endl;
    cin >> choice;
    return choice;
}

/*
 * Final Coin selection from coin list
 */
int menu::CoinSelection(){
    int choice =0;
    cout << "Select a Coin to edit/remove" << endl;
    /// list coins
    cin >> choice;
    return choice;
}

/*
 * DB Input (insert)
 */
void menu::getDBInput(dbObj &src){
    string userSel = "", passSel = "", tableSel = "", IPSel = "";

    cout << "Enter in a new database configuration" << endl;
    cout << "MySQL User: ";
    cin >> userSel;
    cout << "MySQL Pass: ";
    cin >> passSel;
    cout << "MySQL Table: ";
    cin >> tableSel;
    cout << "MySQL Server(IP): " ;
    cin >> IPSel;

    src.set(IPSel, tableSel, userSel, passSel);
}

/*
 * DB Input (insert)
 */
void menu::getDaeInput(daeObj &src){
    string userSel = "", passSel = "",  IPSel = "",
            portSel = "", wallPassSel = "", coinSel = "";

    cout << "Enter in a new database configuration" << endl;
    cout << "RPC User: ";
    cin >> userSel;
    cout << "RPC Pass: ";
    cin >> passSel;
    cout << "RPC Port: ";
    cin >> portSel;
    cout << "RPC Server(IP): " ;
    cin >> IPSel;
    cout << "Wallet Decryption Password: " ;
    cin >> wallPassSel;
    cout << "Daemon Coin Acronym: " ;
    cin >> coinSel;

    src.set(userSel, passSel, IPSel, portSel, coinSel, wallPassSel);
}

/*
 * DB Input/Modify (insert/update)
 */
void menu::getDBInputEdit(dbObj &src){
    string editSel = "", userSel = "", passSel = "", tableSel = "", IPSel = "";

    cout << "Modify current database configuration" << endl;
    cout << "1) MySQL User: "<< src.getUser() << " 2) MySQL Pass: " << src.getPass() << " 3) MySQL Table: " << src.getName() << " 4)MySQL Server(IP): " << src.getHost() << endl;

    cout << "Which do you want to modify? (verbose 0)" << endl;
    cin >> editSel;


    if(editSel == "1"){
        cout << "MySQL User Before:" << src.dUser;
        cout << " Now? :";
        cin >> userSel;
        src.dUser = userSel;
    }else if(editSel == "2"){
        cout << "MySQL Pass Before:" << src.dPass;
        cout << " Now? :";
        cin >> passSel;
        src.dPass = passSel;
    }else if(editSel == "3"){
        cout << "MySQL Table Before:" << src.dName;
        cout << " Now? :";
        cin >> tableSel;
        src.dName = tableSel;
    }else if(editSel == "4"){
        cout << "MySQL Server Before:" << src.dHost;
        cout << " Now? :";
        cin >> IPSel;
        src.dHost = IPSel;
    }else if(editSel == "0"){
        cout << "MySQL User Before:" << src.dUser <<endl;
        cout << "MySQL Pass Before:" << src.dPass<<endl;
        cout << "MySQL Table Before:" << src.dName<<endl;
        cout << "MySQL Server Before:" << src.dHost<<endl;

        cout << "MySQL User :";
        cin >> userSel;
        cout << "MySQL Pass :";
        cin >> passSel;
        cout << "MySQL Table :";
        cin >> tableSel;
        cout << "MySQL Server :";
        cin >> IPSel;

        src.set(IPSel, tableSel, userSel, passSel);
    }else{
        cout << "exiting database modify menu" << endl;
    }

}

/*
 * DAE Input/Modify (insert)
 */
void menu::getDaeInputEdit(daeObj &src, cache cah){
    string editSel = "", userSel = "", passSel = "", PortSel = "", IPSel = "", CoinSel = "", WalletSel = "";
    string coinSel = "";
    int key = 0, id = 0;
    cout << "Modify current database configuration" << endl;

    for(int id=0; id< src.size; id++){
    cout << "1) RPC User: "<< src.getUser(id) << " 2) RPC Pass: " << src.getPass(id) << " 3) RPC Server: " << src.getServer(id) << " 4)RPC Port: " << src.getPort(id)
         << " 5) Coin: " << src.getCoin(id) << " 6) Wallet Pass: " << src.getWalPas(id) << endl;
    }

    cout << "Which Coin do you want to modify? Enter Coin Code e.g. BTC" << endl;
    cin >> coinSel;

    key = cah.getKeyByCoin(coinSel);
    id = src.getCoinIndex(coinSel);

    if(id != 0){
        cout << "1) RPC User: "<< src.getUser(id) << " 2) RPC Pass: " << src.getPass(id) << " 3) RPC Server: " << src.getServer(id) << " 4)RPC Port: " << src.getPort(id)
         << " 5) Coin: " << src.getCoin(id) << " 6) Wallet Pass: " << src.getWalPas(id) << endl;
    }else{
        cout << "Incorrect entry" << endl;
    }

    cout << "Which setting do you want to modify?" << endl;
    cin >> editSel;

    if(editSel == "1"){
        cout << "RPC User Before: " << src.getUser(id);
        cout << " Now: ";
        cin >> userSel;
        src.setSpec(id, userSel, src.getPass(id), src.getServer(id), src.getPort(id), src.getCoin(id), src.getWalPas(id));
    }else if(editSel == "2"){
        cout << "RPC Pass Before: " << src.getPass(id);
        cout << " Now: ";
        cin >> passSel;
        src.setSpec(id, src.getUser(id), passSel, src.getServer(id), src.getPort(id), src.getCoin(id), src.getWalPas(id));
    }else if(editSel == "3"){
        cout << "RPC Server Before: " << src.getServer(id);
        cout << " Now: ";
        cin >> IPSel;
        src.setSpec(id, src.getUser(id), src.getPass(id), IPSel, src.getPort(id), src.getCoin(id), src.getWalPas(id));
    }else if(editSel == "4"){
        cout << "RPC Post Before: " << src.getPort(id);
        cout << " Now: ";
        cin >> PortSel;
        src.setSpec(id, src.getUser(id), src.getPass(id), src.getServer(id), PortSel, src.getCoin(id), src.getWalPas(id));
    }else if(editSel == "5"){
        cout << "Coin Code Before: " << src.getCoin(id);
        cout << " Now: ";
        cin >> CoinSel;
        src.setSpec(id, src.getUser(id), src.getPass(id), src.getServer(id), src.getPort(id), CoinSel, src.getWalPas(id));
    }else if(editSel == "6"){
        cout << "Wallet Pass Before: " << src.getWalPas(id);
        cout << " Now: ";
        cin >> WalletSel;
        src.setSpec(id, src.getUser(id), src.getPass(id), src.getServer(id), src.getPort(id), src.getCoin(id), WalletSel);
    }else if(editSel == "0"){
        cout << "RPC User Before:" << src.getUser(id) << endl;
        cout << "RPC Pass Before:" << src.getPass(id) << endl;
        cout << "RPC Server Before:" << src.getServer(id) << endl;
        cout << "RPC Port Before:" << src.getPort(id) << endl;
        cout << "Coin Code Before:" << src.getCoin(id) << endl;
        cout << "Wallet Pass Before:" << src.getWalPas(id) << endl;

        cout << "RPC User :";
        cin >> userSel;
        cout << "RPC Pass :";
        cin >> passSel;
        cout << "RPC Server :";
        cin >> IPSel;
        cout << "RPC Port :";
        cin >> PortSel;
        cout << "Coin Code :";
        cin >> CoinSel;
        cout << "Wallet Pass :";
        cin >> WalletSel;
        src.setSpec(id, userSel, passSel, IPSel, PortSel, CoinSel, WalletSel);


    }else{
        cout << "exiting database modify menu" << endl;
    }
    qDebug() << id << endl;
    cah.updateDae(&src, key, id);

}

void menu::getDaeRemove(daeObj *src, cache cah){
    src->displayAll();
    string coinSel = "";

    cout << "which coin do you want to remove? Enter Coin Code: ";
    cin >> coinSel;

    cah.removeDae(coinSel);
}
