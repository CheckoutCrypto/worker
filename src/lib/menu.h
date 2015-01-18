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
#ifndef MENU_H
#define MENU_H

#include <QDebug>
#include <QtSql>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include "src/db/cache.h"
#include "src/objects/daeobj.h"
#include "src/objects/dbobj.h"


using namespace std;

class menu
{

public:
    menu();
   /// Primary menus
    int main(cache cah, string pass);
    int menuSelection();
    int DBMenuSelection();
    int DaeMenuSelection();
    int CoinSelection();

    /// password
    string getPassword(cache cah);

    /// Secondary menus
    void getDBInput(dbObj &src);
    void getDaeInput(daeObj &src);

    void getDBInputEdit(dbObj &src);
    void getDaeInputEdit(daeObj &src, cache cah);
    void getDaeRemove(daeObj *src, cache cah);

    virtual ~menu();
};

#endif // MENU_H
