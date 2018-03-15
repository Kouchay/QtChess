
#include "Board.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>


Board::Board(QWidget *parent) : QWidget(parent)
{
    this->_r = 30;
    init(true);
    this->AiMove = true;
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
            QBrush(QPixmap(":/img/material/board.jpg").scaled(// 缩放背景图.
                this->size(),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(palette);                           // 给widget加上背景图
}

Board::~Board()
{

}

//初始化棋盘
void Board::init(bool bRedSide)
{
    for(int i=0; i<32; ++i)
    {
        m_chessman[i].init(i);   //初始化棋子位置
    }
    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            m_chessman[i].rotate();   //旋转
        }
    }

    m_selectid = -1;    //刚开始不选中任何棋子
    m_bRedTurn = true;  //红方先走
    m_bSide = bRedSide;
    update();
}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 2 * this->_r; //一个单元的大小

    painter.setPen(QPen(Qt::black, 2));
    painter.setRenderHint(QPainter::Antialiasing, true);

    //画横线
    for(int i = 1; i <= 10; ++i)
    {
        painter.drawLine(QPoint(d, i*d), QPoint(9*d, i*d));
    }

    //画竖线
    for(int i = 1; i <=9; ++i)
    {
        if (i == 1 || i == 9)
            painter.drawLine(QPoint(i*d, d), QPoint(i*d, 10*d));
        else
        {
            painter.drawLine(QPoint(i*d, d), QPoint(i*d, 5*d));
            painter.drawLine(QPoint(i*d, 6*d), QPoint(i*d, 10*d));
        }
    }

    //画九宫格
    painter.drawLine(QPoint(4*d, d), QPoint(6*d, 3*d));
    painter.drawLine(QPoint(4*d, 3*d), QPoint(6*d, d));
    painter.drawLine(QPoint(4*d, 8*d), QPoint(6*d, 10*d));
    painter.drawLine(QPoint(4*d, 10*d), QPoint(6*d, 8*d));

    //写字
    painter.setFont(QFont("微软雅黑", 26));
    painter.drawText(QPoint(2*d, 5.7*d)," 楚 河              汉 界" );

    //绘制32颗棋子
    for (int i = 0; i < 32; i++)
    {
        drawChessman(painter, i);
    }
}

QSize Board::sizeHint() const
{
    return QSize(20*this->_r, 22*this->_r);
}

//绘制棋子
void Board::drawChessman(QPainter &painter,int id)
{
    if (m_chessman[id].getDead())
        return;

    QPoint c = center(m_chessman[id].getRow(), m_chessman[id].getCol());
    QRect rect = QRect(c.x()-_r, c.y()-_r, 2*_r, 2*_r);

    if (id == m_selectid)
        painter.setBrush(Qt::gray); //被选中的棋子为灰色
    else
        painter.setBrush(QColor(255,193,37));

    //painter.setFont(QFont("华文隶书", 26));

    if (m_chessman[id].getRed())
    {
        painter.setPen(QPen(Qt::red,2));
        painter.drawEllipse(center(id), _r, _r);  //_r长半径 _r短半径

        painter.drawText(rect,m_chessman[id].getChessmanName(),QTextOption(Qt::AlignCenter));
        painter.setPen(QPen(Qt::black,2));
    }
    else
    {
        painter.setPen(QPen(Qt::black,2));
        painter.drawEllipse(center(id), _r, _r);  //_r长半径 _r短半径

        painter.drawText(rect,m_chessman[id].getChessmanName(),QTextOption(Qt::AlignCenter));
    }
}

void Board::restart()
{
    this->init(true);
}

bool Board::getRedTurn()
{
    //qDebug()<<this->m_bRedTurn;
    return this->m_bRedTurn;
}

bool Board::getAiMove()
{
    return this->AiMove;
}

void Board::setLevel(const int &)
{
}

//获得死亡状态
bool Board::isDead(int id)
{
    if(id == -1)return true;
    return m_chessman[id].getDead();
}

Chessman::TYPE Board::getChessmanType(const int &id)
{
    return m_chessman[id].getType();
}


//是否是“红方”
bool Board::getRed(const int &id)
{
    return m_chessman[id].getRed();
}

//得到中心点的坐标
QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col + 1) * 2 * _r;   //注意，这里要赋值给QPoint类中的rx()ry()函数，而非x y
    ret.ry() = (row + 1) * 2 * _r;
    return ret;
}

//得到中心点左上角的坐标
QPoint Board::center(int id)
{
    return center(m_chessman[id].getRow(), m_chessman[id].getCol());
}


bool Board::getClickRowCol(QPoint pt, int &row, int &col)
{
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            //棋子所控的范围就是以中心点为圆心，棋子半径为半径的圆圈
            QPoint distance = center(row, col) - pt;
            if(distance.x() * distance.x() + distance.y() * distance.y() < _r* _r)
                return true;
        }
    }
    return false;
}

//得到该row、col上面的棋子
int Board::getStoneId(int row, int col)
{
    for(int i=0; i<32; ++i)
    {
        if(m_chessman[i].getRow() == row && m_chessman[i].getCol() == col && !isDead(i))
            return i;
    }

    return -1;    //该row、col上没有棋子
}


void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());
}
void Board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getClickRowCol(pt, row, col);   //判断是否点中了棋子
    if(!bClicked) return;    //没有点中任何棋子

    int id = getStoneId(row, col);
    click(id, row, col);
}
void Board::click(int id, int row, int col)
{
    if(this->m_selectid == -1)  //如果该棋子以前未被选中，那么就选中该棋子
    {
        trySelectStone(id);
    }
    else                       //如果该棋子已经被选中，那么就要尝试移动棋子
    {
        tryMoveStone(id, row, col);
    }
}

bool Board::canSelect(int id)
{
    return m_bRedTurn == m_chessman[id].getRed();
}

void Board::trySelectStone(int id)
{
    if(id == -1) return;

    if(!canSelect(id)) return;

    m_selectid = id;
    update();
}

//判断两个棋子颜色是否一样
bool Board::sameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;
    return getRed(id1) == getRed(id2);
}

//尝试移动棋子
void Board::tryMoveStone(int killid, int row, int col)
{
    if(killid != -1 && sameColor(killid, m_selectid))
    {
        trySelectStone(killid);
        return;
    }
    if(canMove(m_selectid, killid, row, col))
    {
        moveStone(m_selectid, killid, row, col);
        m_selectid = -1;
        update();
    }
}




//移动规则模块是整个中国象棋游戏的核心之一

//移动规则辅助函数（1）————“车”、“炮”
//获取这两个鼠标点之间的象棋的个数
int Board::getChessmanCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 < col2 ? col2 : col1;
        for(int col = min+1; col<max; ++col)
        {
            if(getStoneId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 < row2 ? row2 : row1;
        for(int row = min+1; row<max; ++row)
        {
            if(getStoneId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}
//移动规则辅助函数（2）————“马”、“兵”、“将”、“士”、“象”
//获取行列之间的关系(从而去限定棋子走的路线)，在判断给定类型棋子能否走的规则中用到
int Board::relation(int row1, int col1, int row, int col)
{
    // D = qAbs(row1-row)*10+qAbs(col1-col) ——— D值是棋子移动步长的重要指标，相应的棋子必须满足“一定”的步长
    return qAbs(row1-row)*10+qAbs(col1-col);
}
//移动规则辅助函数（3）————“兵”、“将”、“士”、“象”
//判断棋子属于哪一边
bool Board::isBottomSide(int id)
{
    return m_bSide == m_chessman[id].getRed();
}

void Board::GetRowCol(int &row, int &col, const int &id)
{
    row = m_chessman[id].getRow();
    col = m_chessman[id].getCol();
}

bool Board::canMove(int moveid, int killid, int row, int col)
{
    //qDebug()<<moveid<<this->name(moveid);
    //qDebug()<<killid<<this->name(killid);
    if(sameColor(moveid, killid)) return false;//如果moveid的颜色和killid的颜色相同，就要把killid设置成新的要走的棋子（换选择）

    switch (m_chessman[moveid].getType())
    {
    case Chessman::JU:
        return canMoveChe(moveid, killid, row, col);

    case Chessman::MA:
        return canMoveMa(moveid, killid, row, col);

    case Chessman::PAO:
        return canMovePao(moveid, killid, row, col);

    case Chessman::BING:
        return canMoveBing(moveid, killid, row, col);

    case Chessman::JIANG:
        return canMoveJiang(moveid, killid, row, col);

    case Chessman::SHI:
        return canMoveShi(moveid, killid, row, col);

    case Chessman::XIANG:
        return canMoveXiang(moveid, killid, row, col);

    }
    return false;
}

//“车”移动规则
bool Board::canMoveChe(int moveid, int, int row, int col)
{
    //走直线、两点中间不能有棋子
    if (getChessmanCountAtLine(m_chessman[moveid].getRow(), m_chessman[moveid].getCol(), row, col) == -1)
        return false;
    if (getChessmanCountAtLine(m_chessman[moveid].getRow(), m_chessman[moveid].getCol(), row, col) > 0)
        return false;

    return true;
}

//“马”移动规则
bool Board::canMoveMa(int moveid, int, int row, int col)
{
    int _row,_col;
    GetRowCol(_row, _col, moveid);
    int r = relation(_row, _col, row, col);
    if(r != 12 && r != 21)   //横日：12；竖日：21
        return false;

    if(r == 12)   //寻找横日马腿
    {
        if(getStoneId(_row, (col+_col)/2) != -1)    //!= -1表示：马腿上有棋子
            return false;
    }
    else          //寻找竖日马腿
    {
        if(getStoneId((row+_row)/2, _col) != -1)    //!= -1表示：马腿上有棋子
            return false;
    }

    return true;
}

//“炮”移动规则
bool Board::canMovePao(int moveid, int killid, int row, int col)
{
    int r,c;
    GetRowCol(r, c, moveid);
    int ret = getChessmanCountAtLine(row, col, r, c);

    if(killid != -1)   //吃棋
    {
        if(ret == 1) return true;
    }
    else               //走棋
    {
        if(ret == 0) return true;
    }

    return false;
}

//“兵”移动规则
bool Board::canMoveBing(int moveid, int, int row, int col)
{
    int _row,_col;
    GetRowCol(_row, _col, moveid);
    int r = relation(_row, _col, row, col);
    if(r != 1 && r != 10)   //“兵”的移动步长也是1
        return false;

    if(isBottomSide(moveid))  //下方兵未过河
    {
        if(row > _row)
            return false;  //不能后退
        if(_row >= 5 && row == _row)
            return false;  //不能平走
    }
    else    //上方兵未过河
    {
        if(row < _row)
            return false;
        if(_row <= 4 && row == _row)
            return false;
    }

    return true;
}

//“将”移动规则
bool Board::canMoveJiang(int moveid, int killid, int row, int col)
{
    //“将” ——— 首先“将”必须在九宫内；其次移动步长是一个格子

    int _row,_col;
    if(killid != -1 && m_chessman[killid].getRed() == Chessman::JIANG)  //将、将照面直接吃
        return canMoveChe(moveid, killid, row, col);

    GetRowCol(_row, _col, moveid);

    //限制上、下“将”的九宫格
    if(col < 3 || col > 5) return false;
    if(isBottomSide(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }

    //移动步长是一个格子
    int r = relation(_row, _col, row, col);
    if(r != 1 && r != 10)    //要么行相同，要么列相同，且步长是“1”
        return false;

    return true;
}

//“士”移动规则
bool Board::canMoveShi(int moveid, int, int row, int col)
{
    int _row,_col;
    GetRowCol(_row, _col, moveid);
    int r = relation(_row, _col, row, col);
    if(r != 11)     //"士"走斜线
        return false;

    //限制上、下“士”的九宫格
    if(col < 3 || col > 5) return false;
    if(isBottomSide(moveid))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

//“象”移动规则
bool Board::canMoveXiang(int moveid, int, int row, int col)
{
    int _row,_col;
    GetRowCol(_row, _col, moveid);  //获得移动棋子的row, col
    int r = relation(_row, _col, row, col);
    if(r != 22)
        return false;

    int rEye = (row+_row)/2;
    int cEye = (col+_col)/2;  //找象眼
    if(getStoneId(rEye, cEye) != -1)   // =-1 ：表示该row、col上没有棋子
        return false;

    //“象”不能过河
    if(isBottomSide(moveid))
    {
        if(row < 4) return false;
    }
    else
    {
        if(row > 5) return false;
    }
    return true;
}


//杀死棋子
void Board::killStone(int id)
{
    if(id==-1) return;
    m_chessman[id].setDead(true);
    if(m_chessman[20].getDead()){
        QMessageBox::about(this,"游戏结束","黑方赢");
        this->AiMove = false;
    }
    if(m_chessman[4].getDead()){
        QMessageBox::about(this,"游戏结束","红方赢");
        this->AiMove = false;
    }
}

//移动棋子
void Board::moveStone(int moveid, int row, int col)
{
    m_chessman[moveid].setRow(row);
    m_chessman[moveid].setCol(col);

    m_bRedTurn = !m_bRedTurn;
}
void Board::moveStone(int moveid, int killid, int row, int col)
{
    saveStep(moveid, killid, row, col, m_steps);

    killStone(killid);
    moveStone(moveid, row, col);
}


//保存移动棋子的该步————“悔棋”、“人工智能”
void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    int r,c;
    GetRowCol(r, c, moveid);
    Step* step = new Step;
    step->colFrom = c;
    step->colTo = col;
    step->rowFrom = r;
    step->rowTo = row;
    step->moveid = moveid;
    step->killid = killid;

    steps.append(step);
}



//“悔棋”模块
void Board::back(Step *step)
{
    reliveStone(step->killid);
    moveStone(step->moveid, step->rowFrom, step->colFrom);
}

void Board::reliveStone(int id)
{
    if(id == -1) return;
    m_chessman[id].setDead(false);
}

void Board::backOne()
{
    if(this->m_steps.size() == 0) return;

    Step* step = this->m_steps.last();
    m_steps.removeLast();
    back(step);

    update();
    delete step;
}

void Board::back()
{
    backOne();
}

void Board::slotBack()
{
    back();
}
