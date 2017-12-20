#include "stdafx.h"
#include "MCModCrafter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MCModCrafter w;
    w.show();
    return a.exec();
}
