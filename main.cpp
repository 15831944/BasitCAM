/*
*    Copyright (c) 2015 suleyman All Rights Reserved.
*
*           File : main.cpp
*         Author : suleymanturkoglu@hotmail.com
*           Date : 2014-07-15 21:00
*        Version : OpenCASCADE6.9.0 & Qt5.4
*
*    Description : OpenCASCADE with Qt demo.
*/

#include "occQt.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    occQt w;
    w.show();

    return a.exec();
}








