#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    // x,y cooridinate
    float m_x;
    float m_y;
    // look direction in radians
    float m_alpha;

public:
    Player(float x, float y, float alpha);
    ~Player();

    float getX();
    float getY();
    float getAlpha();

    void setX(float x);
    void setY(float y);
    void setAlpha(float alpha);
};

#endif

