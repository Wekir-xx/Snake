#include "Snake.h"
#include <qpainter.h>
#include <QKeyEvent>
#include <qpainterpath.h>
#include <random>


Snake::Snake(QWidget* parent)
    : QWidget{parent}
{
    this->setFixedSize(WIDTH, HEIGHT);
    QBrush brush;
    brush.setTextureImage (QImage(":/images/back.png"));
    QPalette palete;
    palete.setBrush (QPalette::Window, brush);
    this->setPalette(palete);

    head.setX(2);
    head.setY(2);
    dots.push_back(QPoint(1, 2));
    dots.push_back(QPoint(0, 2));

    up_direction = false;
    down_direction = false;
    right_direction = true;
    left_direction = false;
    keyopen = false;

    loadImages();
    putApple();
    timerID = startTimer(DELAY);
}

void Snake::loadImages()
{
    i_dot.load(":/images/dot.png");
    i_dot = i_dot.scaledToWidth(DOT_PIXELS);
    i_head.load(":/images/head.png");
    i_head = i_head.scaledToWidth(DOT_PIXELS);
    i_apple.load(":/images/apple.png");
    i_apple = i_apple.scaledToWidth(DOT_PIXELS);
}

void Snake::putApple()
{
    bool no_snake = false;
    int counter {};
    int x{}, y{};

    while (!no_snake)
    {
        x = std::mt19937(std::random_device{}())() % (WIDTH / DOT_PIXELS);
        y = std::mt19937(std::random_device{}())() % (HEIGHT / DOT_PIXELS);
        if (head.x() != x || head.y() != y)
        {
            for (const auto& dot : dots)
            {
                if (dot.x() != x || dot.y() != y)
                    counter++;
                else
                    break;
            }
        }
        if (counter == dots.size())
            no_snake = true;
    }
    apple.setX(x);
    apple.setY(y);
}

void Snake::moveHead()
{
    tail_shadow = dots.at(dots.size() - 1);
    moveTail();

    if (up_direction)
        head.setY(head.y () - 1);
    else if (down_direction)
        head.setY(head.y () + 1);
    else if (left_direction)
        head.setX(head.x () - 1);
    else
        head.setX(head.x () + 1);
}

void Snake::moveTail()
{
    for (int i = dots.size()-1; i > 0; i--)
        dots.replace(i, QPoint(dots.at(i-1).x(), dots.at(i-1).y()));
    dots.replace(0, QPoint(head.x(), head.y()));
}

void Snake::eatApple()
{
    if (head.x() == apple.x() && head.y() == apple.y())
    {
        dots.push_back(tail_shadow);
        putApple();
    }
}

bool Snake::isGameOver()
{
    if (head.x() < 0 || head.x() >= WIDTH/DOT_PIXELS || head.y() < 0 || head.y() >= HEIGHT/DOT_PIXELS)
        return true;
    for (const auto& dot : dots)
    {
        if (dot.x() == head.x() && dot.y() == head.y())
            return true;
    }
    return false;
}

void Snake::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter* painter = new QPainter(this);

    if (!isGameOver())
    {
        painter->drawImage(head * DOT_PIXELS, i_head);
        painter->drawImage(apple * DOT_PIXELS, i_apple);
        for (const auto& dot : dots)
            painter->drawImage(dot * DOT_PIXELS, i_dot);
    }
    else
    {
        killTimer(timerID);
        painter->setRenderHint(QPainter::Antialiasing);

        QString text("Game Over");
        QFont font("Times New Roman", 35);
        QFontMetrics metrics(font);
        qreal text_width = metrics.horizontalAdvance(text);
        painter->setPen(QPen(QColor("red")));
        painter->setBrush(QBrush(QColor("white")));
        QPainterPath textpath;
        textpath.addText(this->width() / 2 - text_width / 2, this->height() / 2, font, text);
        painter->drawPath(textpath);
    }
    delete painter;
}

void Snake::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event);
    moveHead();
    eatApple();
    this->repaint();
    keyopen = false;
}

void Snake::keyPressEvent(QKeyEvent* event)
{
    if(keyopen)
        return;
    else
        keyopen = true;

    int key = event->key();

    if (key == Qt::Key_Up && !down_direction)
    {
        up_direction = true;
        right_direction = false;
        left_direction = false;
    }
    if (key == Qt::Key_Down && !up_direction)
    {
        down_direction = true;
        right_direction = false;
        left_direction = false;
    }
    if (key == Qt::Key_Right && !left_direction)
    {
        right_direction = true;
        up_direction = false;
        down_direction = false;
    }
    if (key == Qt::Key_Left && !right_direction)
    {
        left_direction = true;
        up_direction = false;
        down_direction = false;
    }
}
