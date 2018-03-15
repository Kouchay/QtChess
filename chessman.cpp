#include "chessman.h"

Chessman::Chessman()
{

}

int Chessman::getId()
{
    return this->m_id;
}

void Chessman::init(int id)
{
    struct
    {
        int row, col;
        Chessman::TYPE type;
    } pos[16] = {
            {0, 0, Chessman::JU},
            {0, 1, Chessman::MA},
            {0, 2, Chessman::XIANG},
            {0, 3, Chessman::SHI},
            {0, 4, Chessman::JIANG},
            {0, 5, Chessman::SHI},
            {0, 6, Chessman::XIANG},
            {0, 7, Chessman::MA},
            {0, 8, Chessman::JU},

            {2, 1, Chessman::PAO},
            {2, 7, Chessman::PAO},
            {3, 0, Chessman::BING},
            {3, 2, Chessman::BING},
            {3, 4, Chessman::BING},
            {3, 6, Chessman::BING},
            {3, 8, Chessman::BING},
        };

    //this->m_id = id;
    this->dead = false;
    this->red = id < 16;

    if(id < 16)
    {
        row = pos[id].row;
        col = pos[id].col;
        type = pos[id].type;
    }
    else
    {
        row = 9-pos[id-16].row;
        col = 8-pos[id-16].col;
        type = pos[id-16].type;
    }
}

void Chessman::rotate()
{
    col = 8 - col;
    row = 9 - row;
}

int Chessman::getRow()
{
    return this->row;
}

int Chessman::getCol()
{
    return this->col;
}

bool Chessman::getDead()
{
    return this->dead;
}

bool Chessman::getRed()
{
    return this->red;
}

Chessman::TYPE Chessman::getType()
{
    return this->type;
}

void Chessman::setDead(bool dead)
{
    this->dead = dead;
}

void Chessman::setRow(int row)
{
    this->row = row;
}

void Chessman::setCol(int col)
{
    this->col = col;
}

void Chessman::setRed(bool red)
{
    this->red = red;
}

QString Chessman::getChessmanName()
{
    switch(this->type)
        {
        case JU:
            return "车";
        case MA:
            return "马";
        case PAO:
            return "炮";
        case BING:
            return "兵";
        case JIANG:
            return "将";
        case SHI:
            return "士";
        case XIANG:
            return "相";
        }
        return "错误";

}
