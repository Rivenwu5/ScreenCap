#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include"defines.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
//    MyWidget(QWidget *parent = nullptr);
    MyWidget(QImage* img);
    ~MyWidget() override;

private:
    Ui::MyWidget *ui;

public:
    void paintEvent(QPaintEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    //全局记录矩形的起始、结束坐标、宽高
    int rect_b_x,rect_b_y;
    int rect_e_x,rect_e_y;
    int rect_w,rect_h;

    void InitDrawRect();
private:
    MouseStatus status;
    QRect capturedRect;
    QImage* screenImg;
public:
    QPushButton* yesBtn;
    QPushButton* noBtn;
    void setEndBtnLoc(int ax,int ay);
    QRect CapturedDialogRect2ImgRect(QRect& capturedDialogRect);



};
#endif // MYWIDGET_H
