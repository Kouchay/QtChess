#ifndef CHESSMAN_H
#define CHESSMAN_H

#include <QString>
class Chessman
{
public:
    Chessman();
    enum TYPE{JIANG, JU, PAO, MA, BING, SHI, XIANG};





    int m_id;


    int getId();
    void init(int id);
    void rotate();

    int getRow();
    int getCol();
    bool getDead();
    bool getRed();
    TYPE getType();

    void setDead(bool dead);
    void setRow(int row);
    void setCol(int col);
    void setRed(bool red);
    QString getChessmanName();

private:
    int row;
    int col;
    bool dead;
    bool red;
    TYPE type;
};

#endif // CHESSMAN_H
