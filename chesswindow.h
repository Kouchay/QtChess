#ifndef CHESSWINDOW_H
#define CHESSWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include "Board.h"
#include "AIplayer.h"

class ChessWindow : public QMainWindow
{
    Q_OBJECT

public:
   explicit ChessWindow(QWidget *parent = 0);
    ~ChessWindow();


private:
    void initWindow();

    QMenuBar *p_MenuBar;
    QMenu *p_GameMenu;
    Board *m_pBoard;


public slots:
    void slotBack();
};

#endif // CHESSWINDOW_H
