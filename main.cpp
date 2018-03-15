#include "chesswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChessWindow w;
    //禁止放大缩小
    w.setWindowFlags(w.windowFlags() &~ Qt::WindowMaximizeButtonHint);
    w.setFixedSize(w.width()-30, w.height()+200);
    w.show();

    return a.exec();
}
