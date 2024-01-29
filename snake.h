#pragma once

#include <qwidget.h>

class Snake : public QWidget
{
public:
    Snake(QWidget* parent = nullptr);

private:
    static constexpr int WIDTH {500};
    static constexpr int HEIGHT {400};
    static constexpr int DOT_PIXELS {25};
    static constexpr int DELAY {140};

    QImage i_head;
    QImage i_apple;
    QImage i_dot;
    int timerID;

    QPoint head;
    QPoint apple;
    QList<QPoint> dots;
    QPoint tail_shadow;

    bool up_direction;
    bool down_direction;
    bool right_direction;
    bool left_direction;
    bool keyopen;

    void loadImages();
    void putApple();
    void moveHead();
    void moveTail();
    void eatApple();
    bool isGameOver();

    void paintEvent(QPaintEvent* event);
    void timerEvent(QTimerEvent* event);
    void keyPressEvent(QKeyEvent* event);
};
