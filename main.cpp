#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.centralwidget->setLayout(w.layout);
    w.show();
    return a.exec();
}
