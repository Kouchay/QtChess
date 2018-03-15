#include "chesswindow.h"

ChessWindow::ChessWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->initWindow();
    m_pBoard = new Board();
    this->setCentralWidget(m_pBoard);
}

ChessWindow::~ChessWindow()
{

}

void ChessWindow::initWindow()
{
    this->setWindowTitle("中国象棋");

    /* 菜单栏 */
    p_MenuBar = menuBar();
    p_GameMenu = p_MenuBar->addMenu("&游戏");

    p_GameMenu->addAction("&重新开始",this,[this](){
        m_pBoard->restart();
    });
    /* 悔棋*/
    p_GameMenu->addSeparator();
    p_GameMenu->addAction("&悔棋",this,&slotBack,QKeySequence::Back);

    /* 选择难度*/
//    QMenu *chooseLevelMenu = p_GameMenu->addMenu("选择难度");

//    chooseLevelMenu->addAction("&简单",this,[this](){
//        m_pBoard->setLevel(2);
//    });
//    chooseLevelMenu->addAction("&中等",this,[this](){
//        m_pBoard->setLevel(3);
//    });
//    chooseLevelMenu->addAction("&困难",this,[this](){
//        m_pBoard->setLevel(4);
//    });
    //p_GameMenu->addMenu(chooseLevelMenu);

    p_GameMenu->addSeparator();
    p_GameMenu->addAction("&退出",this,&close,QKeySequence::Close);
}

void ChessWindow::slotBack()
{
    m_pBoard->slotBack();
    //QMessageBox::about(NULL,QString("Back testing"),QString("Back testing"));
}
