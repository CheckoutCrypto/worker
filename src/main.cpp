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
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <string>
#include <sstream>
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include "threads/control.h"

using namespace std;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    stringstream txt;
    QDateTime current = QDateTime::currentDateTime();

    switch (type) {
    case QtDebugMsg:
        txt << current.toString().toStdString().c_str() << QString(": %1").arg(msg).toStdString().c_str();
        break;
    case QtWarningMsg:
        txt << QString("Warning: %1").arg(msg).toStdString().c_str();
    break;
    case QtCriticalMsg:
        txt << QString("Critical: %1").arg(msg).toStdString().c_str();
    break;
    case QtFatalMsg:
        txt << QString("Fatal: %1").arg(msg).toStdString().c_str();
    }

    ofstream myfile;
    string cache = "/var/log/worker.log";
    myfile.open(cache.c_str(),ios::app);
    myfile << txt.str().c_str() << endl;
    myfile.close();

 /*   stringstream txt;

     QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt.str().c_str() << endl; */

}

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    control con;
    string pass = "";

    if(cmdOptionExists(argv, argv+argc, "-server"))
    {
        for(int i = 0; i < argc; i++)
         if(cmdOptionExists(argv, argv+argc, "-pass")){
             pass = argv[3];
         }
        qInstallMessageHandler(myMessageOutput);
        con.initCache(pass);
        con.initializeServer();
    }else{
        con.main("");
    }

    char * filename = getCmdOption(argv, argv + argc, "-f");

    if (filename)
    {
        qDebug() << "the command arg was " << filename << endl;
    }

    /// test ssl
 /*  QMyServer server;
    if (!server.listen(QHostAddress::Any)) {
    return -1;
    }
   fprintf(stdout, "listening at port %d\n", server.serverPort()); */

   return a.exec();
}
