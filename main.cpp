#include "mainwindow.h"
#include <QPixmap>
#include <QSplashScreen>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/new/image/Book_and_Quill_JE2_BE2.gif");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();
    MainWindow w;
    w.show();
    return a.exec();
}
