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
#include "cache.h"

cache::cache()
{
   string DB = "/.cache/worker2/cache.db";
   DBlocation = getenv("HOME") + DB;
}
//// Open SQlite connection
void cache::OpenDB(){
   this->db = QSqlDatabase::addDatabase("QSQLITE", "test3");
   this->db.setDatabaseName(DBlocation.c_str());
}
//// Close SQlite connection
void cache::closeDB(){
    this->db = QSqlDatabase();
    QSqlDatabase::removeDatabase("test3");
}
/*
* Write Any string to database
*/
void cache::writeMe(string qry){
       QSqlQuery myQry;
       myQry.prepare(qry.c_str());
       myQry.exec();
}
/// controls inital functions at once
void cache::control(){
       createCache();
       setInitDB();
       createLocalDB();
}

/*
* Create Local Database Table
*/
void cache::createLocalDB() {
     OpenDB();
     string finalQry[12];
     finalQry[0] = "create table IF NOT EXISTS daemons (key INTEGER PRIMARY KEY, rpcUser TEXT, rpcPass TEXT, rpcIP TEXT, rpcPort TEXT, coin_code TEXT, walletpass TEXT)";
     finalQry[1] = "create table IF NOT EXISTS database (key INTEGER PRIMARY KEY, user TEXT, pass TEXT, ip_address TEXT, dbname TEXT)";
     finalQry[2] = "create table IF NOT EXISTS groups (key INTEGER PRIMARY KEY, grpid INTEGER, name TEXT, withdraw_fee TEXT, trade_fee TEXT, monthly_fee TEXT, exchange_fee TEXT)";
     finalQry[3] = "create table IF NOT EXISTS preferences (key INTEGER PRIMARY KEY, apikey TEXT)";

    if (this->db.open()) {
        for(int i=0; i<4; i++){
            QSqlQuery myQry(db);
            myQry.prepare(finalQry[i].c_str());
            myQry.exec();
        }
         this->db.close();
        closeDB();
    }

}

/*
 * Write a single Daemon
 */
void cache::writeDaemon(daeObj *src, int id){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));
   string str2;
   stringstream os;
   QString qUser = crypt.encryptToString(QString(src->getUser(id).c_str()));
   QString qPass = crypt.encryptToString(QString(src->getPass(id).c_str()));
   QString qHost = crypt.encryptToString(QString(src->getServer(id).c_str()));
   QString qPort = crypt.encryptToString(QString(src->getPort(id).c_str()));
   QString qCoin = QString(src->getCoin(id).c_str());  // exempt coin code for indexing
   QString qWallet = crypt.encryptToString(QString(src->getWalPas(id).c_str()));
   os << "INSERT INTO daemons (rpcUser, rpcPass, rpcIP, rpcPort, coin_code, walletpass) VALUES ('"
      << qUser.toStdString() << "','" << qPass.toStdString() << "','" << qHost.toStdString() << "','" << qPort.toStdString()  << "','" << qCoin.toStdString()  << "','" << qWallet.toStdString() << "')";
   OpenDB();

   if (this->db.open()) {
       str2 = os.str();
       QSqlQuery myQry(db);
       myQry.prepare(str2.c_str());
       myQry.exec();
       this->db.close();
      closeDB();
  }
}

/*
 * Write Database Config
 */
void cache::writeDB(dbObj *src){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));
   string str2;
   stringstream os;
   QString qUser = crypt.encryptToString(QString(src->getUser().c_str()));
   QString qPass = crypt.encryptToString(QString(src->getPass().c_str()));
   QString qHost = crypt.encryptToString(QString(src->getHost().c_str()));
   QString qTable = crypt.encryptToString(QString(src->getName().c_str()));

   os << "INSERT INTO database (user, pass, ip_address, dbname) VALUES ('"
      << qUser.toStdString() << "','" << qPass.toStdString() << "','" << qHost.toStdString() << "','" << qTable.toStdString() << "')";
   OpenDB();

   if (this->db.open()) {
       str2 = os.str();
       QSqlQuery myQry(db);
       myQry.prepare(str2.c_str());
       myQry.exec();
       this->db.close();
      closeDB();
  }
}

/*
 * Write a single Daemon
 */
void cache::writeGrp(grpobj *src, int id){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));
   string str2;
   stringstream os;
   QString qID = crypt.encryptToString(QString(src->getGID(id)));
   QString qName = crypt.encryptToString(QString(src->getName(id).c_str()));
   QString qWFee = crypt.encryptToString(QString(src->getWithdrawFee(id).c_str()));
   QString qTFee = crypt.encryptToString(QString(src->getTradeFee(id).c_str()));
   QString qXFee = crypt.encryptToString(QString(src->getExchangeFee(id).c_str()));
   QString qMFee =  crypt.encryptToString(QString(src->getMonthlyFee(id).c_str()));

   os << "INSERT INTO groups (grpid, name, withdraw_fee, trade_fee, monthly_fee, exchange_fee ) VALUES ('"
      << qID.toStdString() << "','" << qName.toStdString() << "','" << qWFee.toStdString() << "','" << qTFee.toStdString()  << "','" << qMFee.toStdString()  << "','" << qXFee.toStdString() << "')";
   OpenDB();

   if (this->db.open()) {
       str2 = os.str();
       QSqlQuery myQry(db);
       myQry.prepare(str2.c_str());
       myQry.exec();
       this->db.close();
      closeDB();
  }
}

/*
 *  Generate, Insert/Update API Key
 */
void cache::generateAPI(){

    SimpleCrypt crypt(atol(APICache));
   string str2, apikey = "";
   stringstream os;
   QString qAPI = crypt.encryptToString(QString(APIKey));

   /// Check if API KEY exists
   apikey = checkAPI();

    if(apikey == ""){
        os << "INSERT INTO preferences (apikey) VALUES ('" << qAPI.toStdString() << "')";
    }else{
        os << "UPDATE preferences SET apikey='"<< qAPI.toStdString() << "'";
    }

   OpenDB();

   if (this->db.open()) {
       str2 = os.str();
       QSqlQuery myQry(db);
       myQry.prepare(str2.c_str());
       myQry.exec();
       this->db.close();
      closeDB();
  }
  qDebug() << "Your new API KEY: " << qAPI.toStdString().c_str() << endl;
}

string cache::checkAPI(){
    SimpleCrypt crypt(atol(APICache));

    OpenDB();
    QString QVal1 = "";

    if(this->db.open()){
        QSqlQuery query(db);
        query.prepare("SELECT * FROM preferences");
        query.exec();
        while (query.next()){
            QVal1 = query.value(1).toString();
           // qDebug() << QVal1.toStdString().c_str() << endl;
       }
        this->db.close();
       closeDB();
    }
    return QVal1.toStdString();
}

/// read Database settings from local db
void cache::readDB(dbObj &src){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));

    OpenDB();

       if(this->db.open()){
           QSqlQuery query(db);
           query.prepare("SELECT * FROM database");
           query.exec();
           while (query.next()){
               QString QVal1 = crypt.decryptToString(query.value(1).toString());
               QString QVal2 = crypt.decryptToString(query.value(2).toString());
               QString QVal3 = crypt.decryptToString(query.value(3).toString());
               QString QVal4 = crypt.decryptToString(query.value(4).toString());
               src.set(QVal3.toStdString(),QVal4.toStdString(), QVal1.toStdString(),QVal2.toStdString());
           }
           this->db.close();
           closeDB();
       }
}

/// read Daemons settings from local db
void cache::readDae(daeObj &src){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));
    int count = 0;
    OpenDB();

       if(this->db.open()){
           QSqlQuery query(db);
           query.prepare("SELECT * FROM daemons");
           query.exec();
           while (query.next()){
               QString QVal1 = crypt.decryptToString(query.value(1).toString());
               QString QVal2 = crypt.decryptToString(query.value(2).toString());
               QString QVal3 = crypt.decryptToString(query.value(3).toString());
               QString QVal4 = crypt.decryptToString(query.value(4).toString());
               QString QVal5 = query.value(5).toString();
               QString QVal6 = crypt.decryptToString(query.value(6).toString());

               src.set(QVal1.toStdString(),QVal2.toStdString(), QVal3.toStdString(),QVal4.toStdString(), QVal5.toStdString(),QVal6.toStdString());
           }
           this->db.close();
           closeDB();
       }
}

/// read Daemons settings from local db
void cache::readGrp(grpobj &src){
    src.init(10);
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));

    OpenDB();
       if(this->db.open()){
           QSqlQuery query(db);
           query.prepare("SELECT * FROM groups");
           query.exec();
           while (query.next()){
               QString QVal1 = crypt.decryptToString(query.value(1).toString());
               QString QVal2 = crypt.decryptToString(query.value(2).toString());
               QString QVal3 = crypt.decryptToString(query.value(3).toString());
               QString QVal4 = crypt.decryptToString(query.value(4).toString());
               QString QVal5 = crypt.decryptToString(query.value(5).toString());
               QString QVal6 = crypt.decryptToString(query.value(6).toString());

               src.set(QVal1.toInt(),QVal2.toStdString(), QVal3.toStdString(),QVal4.toStdString(), QVal5.toStdString(),QVal6.toStdString());
           }
           this->db.close();
           closeDB();
       }
}

/// Update Database settings in local db
void cache::updateDB(dbObj *src){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));

    string str2;
    stringstream os;

    QString qUser = crypt.encryptToString(QString(src->getUser().c_str()));
    QString qPass = crypt.encryptToString(QString(src->getPass().c_str()));
    QString qHost = crypt.encryptToString(QString(src->getHost().c_str()));
    QString qTable = crypt.encryptToString(QString(src->getName().c_str()));

    os << "UPDATE database SET user='"<< qUser.toStdString()  <<"', pass='"<< qPass.toStdString() <<"',ip_address='"<< qHost.toStdString() << "', dbname='"<< qTable.toStdString() << "' WHERE key=1";
    OpenDB();
    if (this->db.open()) {
        str2 = os.str();
        QSqlQuery myQry(db);
        myQry.prepare(str2.c_str());
        myQry.exec();
        this->db.close();
       closeDB();
   }
}

/// Update Daemon settings in local db
void cache::updateDae(daeObj *src, int key, int id){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));
    QString qUser = crypt.encryptToString(QString(src->getUser(id).c_str()));
    QString qPass = crypt.encryptToString(QString(src->getPass(id).c_str()));
    QString qHost = crypt.encryptToString(QString(src->getServer(id).c_str()));
    QString qPort = crypt.encryptToString(QString(src->getPort(id).c_str()));
    QString qCoin = QString(src->getCoin(id).c_str());
    QString qWallet = crypt.encryptToString(QString(src->getWalPas(id).c_str()));

    string str2;
    stringstream os;
    os << "UPDATE daemons SET rpcUser='"<< qUser.toStdString()  <<"', rpcPass='"<< qPass.toStdString()
       << "', rpcIP='"<< qHost.toStdString() << "', rpcPort='"<< qPort.toStdString() << "', coin_code='"
      << qCoin.toStdString() << "', walletpass='"<< qWallet.toStdString() << "' WHERE key=" << key;

    OpenDB();

    if (this->db.open()) {
        str2 = os.str();
        QSqlQuery myQry(db);
        myQry.prepare(str2.c_str());
        myQry.exec();
        this->db.close();
       closeDB();
   }
}

/// Update Group settings in local db
void cache::updateGrp(grpobj *src, int key, int id){
    bool ok;
    SimpleCrypt crypt(pDecrypt.toLong(&ok, 10));
    QString qName = crypt.encryptToString(QString(src->getName(id).c_str()));
    QString qWFee = crypt.encryptToString(QString(src->getWithdrawFee(id).c_str()));
    QString qTFee = crypt.encryptToString(QString(src->getTradeFee(id).c_str()));
    QString qXFee = crypt.encryptToString(QString(src->getExchangeFee(id).c_str()));
    QString qMFee = crypt.encryptToString(QString(src->getMonthlyFee(id).c_str()));

    string str2;
    stringstream os;
    os << "UPDATE groups SET name='"<< qName.toStdString() << "', withdraw_fee='"<< qWFee.toStdString()
       << "', trade_fee='"<< qTFee.toStdString() << "', exchange_fee='"  << qXFee.toStdString()
       << "', monthly_fee='"<< qMFee.toStdString() << "' WHERE key=" << key;

    OpenDB();

    if (this->db.open()) {
        str2 = os.str();
        QSqlQuery myQry(db);
        myQry.prepare(str2.c_str());
        myQry.exec();
        this->db.close();
       closeDB();
   }
}

/// Get KEY by Coin Code
int cache::getKeyByCoin(string coin){
    QString QVal0 = "";
    int finalID = 0;

    OpenDB();

       if(this->db.open()){
           QSqlQuery query(db);
           stringstream os;
           os << "SELECT * FROM daemons WHERE coin_code ='" << coin<< "'";
           query.prepare(os.str().c_str());
           query.exec();
           while (query.next()){
               finalID = query.value(0).toInt();
           }
           this->db.close();
           closeDB();
       }
       return finalID;
}

/// Get KEY by Group Name
int cache::getKeyByGrp(string grp){
    QString QVal0 = "";
    int finalID = 0;

    OpenDB();

       if(this->db.open()){
           QSqlQuery query(db);
           stringstream os;
           os << "SELECT * FROM groups WHERE name ='" << grp<< "'";
           query.prepare(os.str().c_str());
           query.exec();
           while (query.next()){
               QVal0 = query.value(0).toString();
           }
           this->db.close();
           closeDB();
       }
       finalID = QVal0.toInt();
       return finalID;
}

void cache::removeDae(string coin){
     OpenDB();

       if(this->db.open()){
           QSqlQuery query(db);
           stringstream os;
           os << "DELETE FROM daemons WHERE coin_code ='" << coin<< "'";
           query.prepare(os.str().c_str());
           query.exec();
           this->db.close();
           closeDB();
       }

}

/// get initial db file from text cache
bool cache::initDB(){
   bool found = false;
   char PrefIn[100];
   FILE* fp;
   string tempcache = TEMPCACHE;
   string Cache = getenv("HOME") + tempcache;
   fp = fopen(Cache.c_str(), "r");   /// open cached db location
   if(fp != NULL){
       cout << "cache found " << endl;
       rewind(fp);
       while(!feof(fp)){
           fscanf(fp, "%s", &PrefIn);
           found = true;
       }
   }
   else{
       return false;
   }

   if(found){   // if the db (listed in this text cache) exists
       setSQL(PrefIn);  /// set SQL location
       fclose(fp);
       return true;
   }
   else{
       fclose(fp);
       return false;
   }
}
/// set initial db file in text cache
void cache::setInitDB(){
   string tempcache = TEMPCACHE;
   ofstream myfile;
   string cache = getenv("HOME") + tempcache;
   myfile.open (cache.c_str());
   myfile << DBlocation.c_str();
   myfile.close();
}

/// create initial cache folders
void cache::createCache(){
   string main = "/.cache/worker2";
   string u_home = getenv("HOME");
   string finMain = u_home + main;
   QString q_main = finMain.c_str();
   QDir(q_main).mkdir(q_main);
}

cache::cache(const cache& src){
   DBlocation = src.DBlocation;
}
cache& cache::operator=(const cache& src){
   if(this != &src)
   {
       setSQL(src.DBlocation);
   }
   return *this;
}
/*
 * Delete Database tables
 */
void cache::dropTable(int mode){
   string finalQry[5];
   int itemCount = 0;

   if( mode == 1)  // drop local audio/video files
   {
       finalQry[0] = "drop table daemons";
       finalQry[1] = "drop table database";
       finalQry[2] = "drop table groups";
       finalQry[3] = "drop table preferences";
       itemCount = 4;
   }
   OpenDB();
/*
   for(int i=0; i<itemCount; i++){
       writeMe(finalQry[i]);
   } */
}
cache::~cache(){
}
