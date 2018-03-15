#ifndef BOARD_H
#define BOARD_H

#include "chessman.h"
#include "Step.h"
#include <QVector>
#include <QMessageBox>
#include <QPainter>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);
    ~Board();

    void    init(bool bRedSide);

    /* 绘图 */
    void    paintEvent(QPaintEvent *);
    void    drawChessman(QPainter &painter, int id);
    virtual QSize sizeHint() const;


    /* 辅助函数 */

    bool        getRed(const int &id);
    bool        sameColor(int id1, int id2);
    int         getStoneId(int row, int col);
    void        killStone(int id);
    void        reliveStone(int id);
    void        moveStone(int moveid, int row, int col);
    bool        isDead(int id);
    Chessman::TYPE getChessmanType(const int &id);

    QPoint  center(int row, int col);
    QPoint  center(int id);
    bool    getClickRowCol(QPoint pt, int& row, int& col);

    /* 棋子移动 */
    void    mouseReleaseEvent(QMouseEvent *);
    void    click(QPoint pt);
    virtual void click(int id, int row, int col);
    void    trySelectStone(int id);
    void    tryMoveStone(int killid, int row, int col);
    void    moveStone(int moveid, int killid, int row, int col);
    void    saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void    backOne();
    void    back(Step* step);
    virtual void back();

    /* 规则 */
    bool    canMove(int moveid, int killid, int row, int col);
    bool    canMoveChe(int moveid, int killid, int row, int col);
    bool    canMoveMa(int moveid, int killid, int row, int col);
    bool    canMovePao(int moveid, int killid, int row, int col);
    bool    canMoveBing(int moveid, int killid, int row, int col);
    bool    canMoveJiang(int moveid, int killid, int row, int col);
    bool    canMoveShi(int moveid, int killid, int row, int col);
    bool    canMoveXiang(int moveid, int killid, int row, int col);

    bool    canSelect(int id);

    /* 规则辅助函数 */
    int     getChessmanCountAtLine(int row1, int col1, int row2, int col2);
    int     relation(int row1, int col1, int row, int col);
    bool    isBottomSide(int id);
    void    GetRowCol(int &row, int &col, const int &id);

    void restart();
    bool getRedTurn();
    bool getAiMove();
    virtual void setLevel(const int &);

private:
    Chessman   m_chessman[32];
    int     _r;
    //QPoint  _off;
    bool    m_bSide;
    QVector<Step*>  m_steps;

    /* game status */
    int     m_selectid;
    bool    m_bRedTurn;
    bool    AiMove;
signals:


public slots:
    void    slotBack();

};

#endif // BOARD_H
