#include <QApplication>
#include "snake.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    Snake window;
    window.setWindowTitle("Window");
    window.setWindowIcon(QIcon(":/images/icon.png"));
    window.show();

    return a.exec();
}
