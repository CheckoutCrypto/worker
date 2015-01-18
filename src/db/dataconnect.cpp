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
#include "dataconnect.h"

DataConnect::DataConnect()
{

}

DataConnect::~DataConnect(){
    closeDB();
    delete data;
}

void DataConnect::close(){
    closeDB();
    delete data;
}

/*
  * open mysql database from global preferences
  */
void DataConnect::openDB(){
     this->db = QSqlDatabase::addDatabase("QMYSQL", "test");
     this->db.setHostName(QString(data->getHost().c_str()));
     this->db.setDatabaseName(QString(data->getName().c_str()));
     this->db.setUserName(QString(data->getUser().c_str()));
     this->db.setPassword(QString(data->getPass().c_str()));

}

void DataConnect::closeDB(){
    this->db = QSqlDatabase();
    QSqlDatabase::removeDatabase("test");
}


/*
  * connect objects with mysql
  */
bool DataConnect::fillOrders(wrkObj &src, string table){
    openDB();
    src.init(10);
    stringstream qry;
    qry << "SELECT * FROM "<< table << " WHERE status = 0 ";
    int counter = 1;
       if(this->db.open()){
            QSqlQuery * query = new QSqlQuery(this->db);
                query->exec(qry.str().c_str());
            while(query->next()){
                QString QVal = query->value(0).toString();
                QString QVal1 = query->value(2).toString();
                QString QVal2 = query->value(3).toString();
                QString QVal3 = query->value(4).toString();
                QString QVal4 = query->value(5).toString();
                QString QVal5 = query->value(6).toString();
                QString QVal6 = query->value(7).toString();
                QString QVal7 = query->value(8).toString();
                QString QVal8 = query->value(9).toString();
                QString QVal9 = query->value(10).toString();

                src.set(QVal.toInt(), QVal1.toInt(), QVal2.toInt(), QVal3.toStdString().c_str(), QVal4.toStdString().c_str(), QVal5.toStdString().c_str(),
                        QVal6.toStdString().c_str(), QVal7.toStdString().c_str(), QVal8.toStdString().c_str(), QVal9.toStdString().c_str());
                counter++;
            }
            delete query;
            db.close();
            closeDB();
            emit ordersReceived();
            return true;
        }

       closeDB();
       emit ordersReceived();
       return false;

}

/*
  * connect objects with mysql
  */
bool DataConnect::fillCoin(daeObj &src){
     string qry = "SELECT * FROM ccdev_coin" ;
    int counter = 0;
    openDB();
       if(this->db.open()){
            QSqlQuery * query = new QSqlQuery(db);
                query->exec(qry.c_str());
            while(query->next()){
                QString QVal2 = query->value(3).toString();  // skip btc_rate for now
                QString QVal3 = query->value(5).toString();
                QString QVal4 = query->value(12).toString();
                QString QVal5 = query->value(13).toString();
                QString QVal6 = query->value(14).toString();
                QString QVal7 = query->value(15).toString();

                src.setFee(QVal2.toStdString().c_str(), QVal3.toStdString().c_str(), QVal4.toStdString().c_str(), QVal5.toStdString().c_str(), QVal6.toStdString().c_str(), QVal7.toStdString().c_str());
                counter++;
            }
            this->db.close();
            closeDB();
            return true;
        }
       return false;

}

/*
  * connect objects with mysql
  */
bool DataConnect::fillGroups(grpobj &src){
    openDB();
    src.init(20);
    string qry ="SELECT * FROM ccdev_groups";
    int counter = 0;
       if(db.open()){
            QSqlQuery * query = new QSqlQuery(db);
                query->exec(qry.c_str());
            while(query->next()){
                QString QVal_id = query->value(0).toString();
                QString QVal_name = query->value(2).toString();
                QString QVal_WFee = query->value(7).toString();
                QString QVal_TFee = query->value(8).toString();
                QString QVal_XFee = query->value(9).toString();
                QString QVal_MFee = query->value(10).toString();

                src.set(QVal_id.toInt(), QVal_name.toStdString(), QVal_WFee.toStdString(), QVal_TFee.toStdString(), QVal_XFee.toStdString(), QVal_MFee.toStdString());
                counter++;
            }
            db.close();
            closeDB();
            return true;
        }
       return false;
}
/*
 *  Update work order result
*/
bool DataConnect::updateWorkOrders(wrkObj *src, string table, int i, int confirm){
    stringstream os;
               os <<   "UPDATE "<< table <<"  SET status= 1,  result= "<< src->getResult(i) << " , timestamp= NOW()" <<" WHERE basic_id = " << src->getOrderID(i) << endl;
                 cout << os.str() << endl;
        openDB();
        if(this->db.open()){
                QSqlQuery * query = new QSqlQuery(this->db);
                query->prepare(os.str().c_str());
                 query->exec();
                this->db.close();
              closeDB();
       }
       return true;
}

/*
 * set current work order processing
*/
bool DataConnect::updateWorkOrderStatus(string table, int orderNum, int status){
    stringstream os;
               os <<   "UPDATE "<< table <<" SET status = "<< status << " , timestamp= NOW()" <<" WHERE basic_id = " << orderNum << endl;
                 cout << os.str() << endl;
        openDB();
        if(this->db.open()){
                QSqlQuery * query = new QSqlQuery(db);
                query->prepare(os.str().c_str());
                 query->exec();
                this->db.close();
                 closeDB();
       }
       return true;
}
/*
 * Update admin worker status
*/
bool DataConnect::updateWorkerStatus(){
    stringstream os;
               os << "UPDATE ccdev_admin SET worker_status= 1" << endl;
                 cout << os.str() << endl;
       openDB();
        if(db.open()){
                QSqlQuery * query = new QSqlQuery(db);
                query->prepare(os.str().c_str());
                 query->exec();
            db.close();
              closeDB();
       }
       return true;
}
/*
* update the transaction count for billing
*/
bool DataConnect::updateTransCount(int userid){
    stringstream os;
               os << "UPDATE ccdev_billing_summary SET billing_trans = billing_trans + 1, total_trans = total_trans + 1 WHERE user_id =" << userid << endl;
                 cout << os.str() << endl;
        openDB();
        if(db.open()){
            QSqlQuery * query = new QSqlQuery(db);
            query->prepare(os.str().c_str());
            query->exec();
            db.close();
            closeDB();
       }
       return true;
}



/*
 * Write a single transaction - sender/reciever/amount
 */
void DataConnect::writeTransaction(wrkObj *src, int wrkNum, string tranID, string fee){
    QDateTime current = QDateTime::currentDateTime();

    stringstream os;
    os << "INSERT INTO ccdev_transactions" << " (`basic_id`, `bundle_type`, `uid`, `coin_code`, `sender`, `receiver`, `tranid`, `amount`, `fee`, `status`, `timestamp`)"
          << " VALUES  (NULL, 'withdrawal', " << "\'" << src->getUser(wrkNum) << "\', \'"
          << src->getCoin(wrkNum) << "\', \'" << src->getSender(wrkNum)  << "\', \'"
          << src->getRecipient(wrkNum) << "\', \'" << tranID << "\', \'"
          << src->getAmount(wrkNum) << "\', \'" << fee << "\', \'" << src->getStatus(wrkNum)  << "\', NOW())" <<endl;
    cout << os.str() << endl;

    openDB();
    if(db.open()){
        QSqlQuery * query = new QSqlQuery(db);
        query->prepare(os.str().c_str());
        query->exec();
        db.close();
        closeDB();
   }

}

/*
 * Write a single transaction - sender/reciever/amount
 */
void DataConnect::updateTransaction(wallet *src, int wrkNum, string tranID){
    stringstream os;
    stringstream qry;

    qry << "amount=" << src->getStrAmount() << " , status=" << src->getConfirm() << " , timestamp= NOW()";

        os << "UPDATE ccdev_transactions SET " << qry.str() << " WHERE tranid=\'" << tranID << "\'" <<endl;
   cout << os.str() << endl;


    qDebug() << src->getStrAmount().c_str() << " = STR AMOUNT IN WALLET" << endl;
    qDebug() << src->getAmount() << "  = AMOUNT DBL IN WALLET" << endl;

    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }

}

/*
 * Write a single transaction - sender/reciever/amount
 */
void DataConnect::confirmWithdrawal(wallet *src, int wrkNum){
    stringstream os;
    stringstream qry;

    qry << "coin_withdraw= coin_withdraw - " << src->getAmount() << " , updated= NOW()";

    os << "UPDATE ccdev_balance SET " << qry.str() << " WHERE uid=\'" << src->getUser() << "\' AND coin_code=\'" << src->getCoin() << "\'"<<endl;
   cout << os.str() << endl;

    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
           query->prepare(QString(os.str().c_str()));
           query->exec();
           db.close();
           closeDB();
      }
}

/*
 * Write a single transaction - sender/reciever/amount
 */
void DataConnect::writeFee(wallet *wall){
    stringstream os;
    os << "INSERT INTO ccdev_transactions" << " (`basic_id`, `bundle_type`, `uid`, `coin_code`, `sender`, `receiver`, `amount`, `status`, `timestamp`)"
          << " VALUES  (NULL, 'fee', " << "\'" << wall->getUser() << "\', \'"
          << wall->getCoin() << "\', \'" << wall->getAccount()  << "\', \'"
          << wall->getAddress() << "\', \'" << wall->getStrAmount() << "\', \'"
          << wall->getConfirm()  << "\',NOW())" <<endl;
    cout << os.str() << endl;

    openDB();
    if(db.open()){
            QSqlQuery * query = new QSqlQuery(db);
            query->prepare(os.str().c_str());
             query->exec();
        db.close();
        closeDB();
    }

}

/*
 *   Update billing summary for user
 */
void DataConnect::writeBillingSummary(wallet *wall, string fee, int grpid){
    stringstream os, qry;

        qry << " grp_id="<< grpid << ", paid_amt="<< wall->getFee() << ", total_paid_amt= total_paid_amt + "<< wall->getFee() << ", billing_trans = 0" << " , last_paid_date= NOW() , billing_cycle = NOW()"<<endl;

   os << "UPDATE ccdev_billing_summary SET" << qry.str() << " WHERE user_id=\'" << wall->getUser() << "\'" <<endl;
    cout << os.str() << endl;
    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }

}


/*
 *  Insert new bill for the user's billing cycle
 */
void DataConnect::writeBill(wallet *wall, int grpid, string grpname){
    stringstream os;
    os << "INSERT INTO ccdev_billing" << " (`basic_id`, `bundle_type`, `user_id`, `grp_id`, `grp_name`, `paid_amt`, `paid_coin`, `paid_date`)"
          << " VALUES  (NULL, 'service_charge', " << "\'" << wall->getUser() << "\', \'"
          << grpid << "\', \'"<< grpname << "\', \'" << wall->getFee()  << "\', \'"
          << wall->getCoin() << "\', NOW())" <<endl;
    cout << os.str() << endl;

    openDB();
    if(db.open()){
            QSqlQuery * query = new QSqlQuery(db);
            query->prepare(os.str().c_str());
             query->exec();
        db.close();
        closeDB();
   }
}


/*
 * Write a single generated wallet account
 */
bool DataConnect::writeGeneratedWallet(wallet *wall, wrkObj *src, int workNum){
    stringstream os;

     os << "INSERT INTO ccdev_wallets" << " (`basic_id`, `bundle_type`, `uid`, `orderid`, `balance_total`, `pending_total`, `fee_total`, `coins_enabled`, `walletaddress`, `walletaccount`, `last_processed_id`, `confirm`, `timestamp`)"
           << " VALUES  (NULL, 'work_order', " << "\'" << src->getUser(workNum) << "\', \'" << src->getOrderID(workNum) << "\', \'0\', \'0\', \'0\', \'" << src->getCoin(workNum) << "\', \'"
           << wall->getAddress() << "\', \'" << wall->getAccount() << "\',\'"<< src->getOrderID(workNum)  << "\',0,NOW())" <<endl;

     cout << os.str() << endl;

     openDB();
       if(this->db.open()){
            QSqlQuery * query = new QSqlQuery(db);
                query->prepare(QString(os.str().c_str()));
                query->exec();
             this->db.close();
             closeDB();
       }
       return true;

}
/*
 *  Write a single confirmation FEE update
 */
bool DataConnect::getWalletPending(wallet &wall){
    char *myQry;     /// string for final query
     myQry = new char[200];
     sprintf(myQry, "SELECT * FROM ccdev_wallets where walletaddress= \'%s'", wall.getAddress().c_str() );
     qDebug() << myQry << endl;
    int counter = 0;
    openDB();
       if(this->db.open()){
            QSqlQuery * query = new QSqlQuery(db);
                query->exec(myQry);
            while(query->next()){

                QString QValAmt = query->value(5).toString();
                QString QValFlag = query->value(13).toString();

                wall.setAmount(QValAmt.toDouble());
                wall.setFlag(QValFlag.toStdString());
                counter++;
            }
            this->db.close();
            closeDB();
            if(counter > 0){
                return true;
            }else{
                wall.setFlag("expired");
                return false;
            }
        }
       wall.setFlag("expired");
       return false;

}

/*
 *  Write a single confirmation of a transaction
 */
bool DataConnect::writeConfirmWallet(wallet *src, int orderid, bool final, bool CONFIRMED, bool walExists){
    stringstream os, is, qry;
    string flag;

    if(!walExists){
        flag = src->getFlag();
    }else{
        flag = "-";
    }
    if(!final){  /// we're confirming for balance update
        qry << " fee_total="<< src->getFee() << ", last_processed_id="<< orderid << ", flag=\'"<< "-"<< "\', pending_total= pending_total + "<< src->getStrAmount() << " , timestamp= NOW()"<<endl;
   }else{
   qry << " last_processed_id="<< orderid << ", balance_total="<< src->getStrAmount() << ", flag=\'"<< flag<< "\', pending_total=0, timestamp= NOW()" << endl;
  }

    if(CONFIRMED){
        qry << ", confirm=" << src->getConfirm();
    }



  //  os << "UPDATE ccdev_wallets SET" << qry.str() << " WHERE walletaddress=\'" << src->getAddress() << "\'" <<endl;
     os << "UPDATE " << qry.str() <<endl;

    is << "INSERT INTO ccdev_wallets" << " (`basic_id`, `bundle_type`, `uid`, `orderid`, `balance_total`, `pending_total`, `fee_total`, `coins_enabled`, `walletaddress`, `walletaccount`, `last_processed_id`, `confirm`, `flag`,`timestamp`)"
          << " VALUES  (NULL, 'work_order', " << "\'" << src->getUser() << "\', \'" << orderid << "\', \'" << 0 <<"\', \'"<< src->getStrAmount() << "\', \'"<< src->getFee() << "\', \'" << src->getCoin() << "\', \'"
          << src->getAddress() << "\', \'" << src->getAccount() << "\',\'"<< orderid  << "\',\'"<< src->getConfirm() << "\',\'"<< flag <<"\',NOW()) ON DUPLICATE KEY "<< os.str() <<endl;

    cout << is.str() << endl;

    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(is.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
}

/*
 *  Write a single confirmation of a transaction
 */
bool DataConnect::updateFee(wallet *src, int orderid){
    stringstream os, os2;
    QString qAmount =  QString("%1").arg(src->getAmount(),0,'g',8);


   os << "UPDATE ccdev_wallets SET fee_total="<< qAmount.toStdString() << " , timestamp= NOW()" << " WHERE walletaddress=\'" << src->getAddress() << "\'" <<endl;
  cout << os.str() << endl;
    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
      openDB();

      os2 << "UPDATE ccdev_balance SET coin_balance= coin_balance - "<< qAmount.toStdString() << " , updated= NOW() WHERE uid=\'" << src->getUser() << "\' AND coin_code=\'" << src->getCoin() << "\'" <<endl;
     cout << os2.str() << endl;
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os2.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
}

/*
 * Update a PENDING account balance
 */
bool DataConnect::writePendingBalance(wallet *src, bool withdrawal){
    stringstream os;
    stringstream qry;
    if(!withdrawal){
    qry << " coin_pending= coin_pending + ( " << src->getStrAmount() << " )";
    }else{
        qry << " coin_balance= coin_balance - " << src->getStrAmount() << " - " << src->getFee()
               << " , coin_withdraw = coin_withdraw + " <<  src->getStrAmount();
    }
    qry << " , updated= NOW()";

   os << "UPDATE ccdev_balance SET" << qry.str() << " WHERE uid=\'" << src->getUser() << "\' AND coin_code=\'"<<src->getCoin() << "\'" <<endl;
  cout << os.str() << endl;
  openDB();

      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
}

/*
 *   Update a FINAL account balance after withdrawal
 */
bool DataConnect::writeConfirmedBalance(wallet *src, bool withdrawal){
    stringstream os;
    stringstream qry;
    QString qWallConfirm =  QString("%1").arg(src->getAmount(),0,'g',8);

    if(!withdrawal){
        if(src->getFlag() != "expired"){
            qry << " coin_balance= coin_balance + ( " <<  qWallConfirm.toStdString() << " - " << src->getFee() << " )"
            << ", coin_pending= coin_pending - ( " << qWallConfirm.toStdString() << " - " << src->getFee() << " )";
        }else if(src->getFlag() == "expired"){
            qry << " coin_pending= coin_pending - ( " << qWallConfirm.toStdString() << " - " << src->getFee() << " )";
        }
    }else{
        qry << " coin_withdraw= coin_withdraw - " <<  qWallConfirm.toStdString();
    }
    qry<< " , updated= NOW()";
   os << "UPDATE ccdev_balance SET" << qry.str() << " WHERE uid=\'" << src->getUser() << "\' AND coin_code=\'"<<src->getCoin() << "\'" <<endl;
  cout << os.str() << endl;

  openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
      return true;

}

/*
 *   Update a FINAL account balance after withdrawal
 */
bool DataConnect::writeUpdatedBalance(int userID, string balance, string coin_code){
    stringstream os;
    stringstream qry;
    qry << " coin_balance= " << balance.c_str() << " , coin_pending= 0,"<< " coin_withdraw= 0,"<<" updated= NOW()";
   os << "UPDATE ccdev_balance SET" << qry.str() << " WHERE uid=\'" << userID << "\' AND coin_code=\'"<<coin_code << "\'" <<endl;
  cout << os.str() << endl;

    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
      return true;
}

/*
 *  Update an account balance for a fee added/removed
 */
bool DataConnect::writeFeeBalance(int userID, string amount, string coin_code, bool addremove){
    stringstream os;
    stringstream qry;
    if(addremove == false){ /// remove from balance
        qry << " coin_balance= coin_balance -" << amount.c_str() << ", updated= NOW()";
    }else{  /// add to balance
        qry << " coin_balance= coin_balance +" << amount.c_str() << ", updated= NOW()";
    }

   os << "UPDATE ccdev_balance SET" << qry.str() << " WHERE uid=\'" << userID << "\' AND coin_code=\'"<<coin_code << "\'" <<endl;
  cout << os.str() << endl;
    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
      return true;
}


/*
 *   Update a FINAL account balance after withdrawal
 */
bool DataConnect::writeModifiedBalance(int userID, string balance, string coin_code){
    stringstream os;
    stringstream qry;

    qry << " coin_balance= " << balance.c_str() << " - coin_pending," << " updated= NOW()";
   os << "UPDATE ccdev_balance SET" << qry.str() << " WHERE uid=\'" << userID << "\' AND coin_code=\'"<<coin_code << "\'" <<endl;
  cout << os.str() << endl;
    openDB();
      if(db.open()){
           QSqlQuery * query = new QSqlQuery(db);
               query->prepare(QString(os.str().c_str()));
               query->exec();
              db.close();
              closeDB();
      }
      return true;
}

/*
 *  Write a single generated balance
 */
bool DataConnect::writeGeneratedAccount(wrkObj *src, int workNum){
    stringstream os;
     os << "INSERT INTO ccdev_balance" << " (`basic_id`, `bundle_type`, `uid`, `coin_name`, `coin_code`, `coin_pending`, `coins_balance`, `updated`)"
           << " VALUES  (NULL, 'work_order', " << "\'" << src->getUser(workNum) << "\', \'" << src->getCoin(workNum) << "\', \'" << src->getCoin(workNum)  << "\', \'0\',\'0\',NOW())" <<endl;

     cout << os.str() << endl;
     openDB();
       if(db.open()){
            QSqlQuery * query = new QSqlQuery(db);
                query->prepare(QString(os.str().c_str()));
                query->exec();
              db.close();
              closeDB();
       }
       return true;


}

