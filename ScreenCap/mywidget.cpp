#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::~MyWidget()
{
    delete ui;
}

//MyWidget::MyWidget(QWidget *parent)
//    : QWidget(parent)
//    , ui(new Ui::MyWidget)
//{}
MyWidget::MyWidget(QImage* img)
{
    ui->setupUi(this);
    this->setWindowTitle("screen");
    this->screenImg = img;

//    setMouseTracking(true);//开启鼠标跟踪
    setMouseTracking(false);//关闭鼠标跟踪

    InitDrawRect();//初始化开始状态的绘制参数

//    yesBtn = new QPushButton(this);
//    yesBtn->setFixedSize(150,50);
//    yesBtn->move(1000,900);
////    yesBtn->setVisible(false);

//    noBtn = new QPushButton(this);
//    noBtn->setFixedSize(150,50);
//    noBtn->move(1200,900);
//    noBtn->setVisible(false);

}
void MyWidget::setEndBtnLoc(int ax,int ay)
{
//    output << "===================================";
//    yesBtn->move(ax-200,ay);
//    yesBtn->setVisible(true);

//    noBtn->move(ax,ay);
//    noBtn->setVisible(true);
}
void MyWidget::InitDrawRect()
{
    rect_b_x = 0;
    rect_b_y = 0;
    rect_e_x = 0;
    rect_e_y = 0;
    rect_w = 0;
    rect_h = 0;

    status = Explore;//默认状态为：鼠标的探索状态
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    //先在窗口内部绘制获取的背景图
    //就是模拟当前窗口，可截图区域
    //其实截图工具应该都是基于这个原理，就是开启截图功能后，实际就是把屏幕当前的背景，本质就是
    //一张图，把这张图拷贝一下，作为背景绘制到你的QT窗口中，然后在这个窗口内部截图，
    //就相当于在电脑上截图...
    QPainter painter(this);
    painter.drawImage(QRect(QPoint(0,0),this->size()),*(this->screenImg));

    //设置ESC，Enter说明
    QFont myFont("微软雅黑",20);
    painter.setFont(myFont);
    painter.setPen(Qt::red);
    painter.drawText(this->width()/2,100,"ESC取消截图内容，Enter截取内容复制到剪切板");


    if(status == Explore)
    {
        return ;
    }

    if(status == Capturing)
    {
        QPainter myPainter(this);
        //设置画笔属性
        myPainter.setPen(QPen(Qt::red,5));

        rect_w = rect_e_x - rect_b_x;
        rect_h = rect_e_y - rect_b_y;

        myPainter.drawRect(rect_b_x, rect_b_y, rect_w, rect_h);
    }

    if(status == Captured)
    {
        QPainter myPainter(this);
        //设置画笔属性
        myPainter.setPen(QPen(Qt::green,5));
        myPainter.drawRect(capturedRect);

        setEndBtnLoc(rect_e_x,rect_e_y);//设置显示的确认取消按钮
    }
}
//鼠标按下，需要确认矩形的起始坐标(左上角)
void MyWidget::mousePressEvent(QMouseEvent *event)
{
    output<<"鼠标点击->"<<"("<<event->x()<<","<<event->y()<<")\n";
    switch(status)
    {
    case Explore:
        status = Capturing;

        rect_b_x = event->x();
        rect_b_y = event->y();

        rect_e_x = rect_b_x;
        rect_e_y = rect_b_y;//鼠标按下，要将起点和终点重合，否则可能会绘制上一次的end-当前次的begin
        break;
    default:
        break;
    }
    update();
}
//鼠标移动过程中，需要确定矩形的结果坐标(右下角)
//移动过程就是无数次更新绘制
void MyWidget::mouseMoveEvent(QMouseEvent *event)
{
    switch(status)
    {
        case Capturing:
        {
            rect_e_x = event->x();
            rect_e_y = event->y();
            break;
        }
        default:
            break;
    }
    update();
}
void MyWidget::mouseReleaseEvent(QMouseEvent *event)
{
    output<<"  鼠标松开->"<<"("<<event->x()<<","<<event->y()<<")\n";
    switch(status)
    {
        case Capturing:
        {
            status = Captured;
            int tmp_w = rect_e_x - rect_b_x;
            int tmp_h = rect_e_y - rect_b_y;
            capturedRect = QRect(rect_b_x,rect_b_y,tmp_w,tmp_h);
            break;
        }
        default:
            break;
    }
    update();//鼠标松开其实并不需要做什么，只需要重绘，因为当前松开的坐标肯定已经在鼠标move中更新了
}

void MyWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
        {
            this->close();
            break;
        }
        case Qt::Key_Return:
        {
            if(status == Captured)//screen完成后，Enter键事件处理
            {

                //这里有个问题
                //实际你截图的区域，是在QT窗口截的，图片只是放到这个QT窗口上
                //所以你截图的区域，并不会完全和图片区域重合的，
                //需要进行转换
                QRect imgRetRect =  CapturedDialogRect2ImgRect(this->capturedRect);

                //获取截图的区域图片，把图片加载到Windows系统的剪切板上
                auto copyCapturedRect = this->screenImg->copy(imgRetRect);

                QClipboard* cpyBoard = QGuiApplication::clipboard();
                cpyBoard->setImage(copyCapturedRect);

                InitDrawRect();//一次截图完毕后，需要重置状态、矩形的起点、终点，宽、高
            }
            this->close();
            break;
        }
        default:
            break;
     }
}

QRect MyWidget::CapturedDialogRect2ImgRect(QRect& capturedDialogRect)
{
    int dialog_w = this->size().width();
    int dialog_h = this->size().height();
    int img_w = this->screenImg->size().width();
    int img_h = this->screenImg->size().height();

    int dialog_rect_w = capturedDialogRect.width();
    int dialog_rect_h = capturedDialogRect.height();

    int dialog_rect_x = capturedDialogRect.x();
    int dialog_rect_y = capturedDialogRect.y();

    //存储实际对应图片的Rect区域的宽高
    int img_rect_w = dialog_rect_w*img_w/dialog_w;
    int img_rect_h = dialog_rect_h*img_h/dialog_h;

    //存储实际对应图片的Rect区域的起点
    int img_rect_x = dialog_rect_x*img_w/dialog_w;
    int img_rect_y = dialog_rect_y*img_h/dialog_h;

    QRect imgRetRect(img_rect_x,img_rect_y,img_rect_w,img_rect_h);
    return imgRetRect;
}
