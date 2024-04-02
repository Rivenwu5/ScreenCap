#include "mywidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置中文字体
    a.setFont(QFont("Microsoft Yahei", 9));

    QScreen *screen = QApplication::screenAt(QCursor().pos());// 获取鼠标所在屏幕
    QImage screenImg = screen->grabWindow(0).toImage();// 获取屏幕的图像数据

    MyWidget w(&screenImg);
//    w.resize(1200,650);
//    w.setFixedSize(QApplication::desktop()->size());
    w.setWindowFlags(Qt::FramelessWindowHint);//无标题、边框窗口
//    w.showFullScreen(); // 显示为全屏模式
    //可以转化为灰度显示，或者透明，表明当前区域可截
//    w.resize(1600,900);
    w.resize(screen->size().width()-100,screen->size().height()-50);
    w.setAttribute(Qt::WA_TranslucentBackground);
    w.show();
    return a.exec();
}

/*
我发现，只能用MSVC-64bit编译器来编译
MSVC-32bit，直接 Debug assertion failed.
可能是程序里面某些库dll库是32位的，不能用64为编译器来编译
*/
