#include "main.hpp"
#include "player.hpp"

Player::Player(float x, float y, float alpha)
{
    m_x = x;
    m_y = y;
    m_alpha = alpha;
}

Player::~Player()
{
}


float Player::getX()     {   return m_x;     }
float Player::getY()     {   return m_y;     }
float Player::getAlpha() {   return m_alpha; }

void Player::setX(float x)           {   m_x = x;            }
void Player::setY(float y)           {   m_y = y;            }
void Player::setAlpha(float alpha)
{   
    if (alpha < 0.f)
        m_alpha = PI2 + alpha;
    else if (alpha >= PI2)
        m_alpha = alpha - PI2;
    else
        m_alpha = alpha;
}