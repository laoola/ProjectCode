#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CreateSignal();
    //获取系统中可用的摄像头信息
    QList<QCameraInfo> infos = QCameraInfo::availableCameras();
    //显示到下拉列表
    foreach(QCameraInfo info,infos){
        qDebug()<<info.deviceName()<<info.description();
        //把摄像头设备名加入列表
        ui->comboBox->addItem(info.deviceName());
    }

    ui->btn_stop->setEnabled(false);
    ui->btn_save->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CreateSignal()
{
    QObject::connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(slot_capture_clicked()));
    QObject::connect(ui->btn_start, SIGNAL(clicked()), this, SLOT(slot_start_clicked()));
    QObject::connect(ui->btn_stop, SIGNAL(clicked()), this, SLOT(slot_stop_clicked()));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(slot_TimeOut()));
}

//分类器的路径
#define source_xml_addr "/home/zwh/File/opencv-3.4.15/opencv-3.4.15/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml"

//将要检测的图片路径
#define source_pix_addr "/home/zwh/File/check.jpg"

void MainWindow::opencv_face(QImage qImage)
{
    QTime time;
    time.start();

    static CvMemStorage* storage = NULL;
    static CvHaarClassifierCascade* cascade = NULL;
    const char*cascade_name =source_xml_addr;
    //加载分类器
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
    if( !cascade )
    {
        qDebug("分类器加载错误.\n");
        return ;
    }

    //创建内存空间
    storage = cvCreateMemStorage(0);

    //加载需要检测的图片
    //const char* filename =source_pix_addr;
    //IplImage* img = cvLoadImage(filename, 1 );
    IplImage* img = QImageToIplImage(&qImage);

    if(img ==nullptr )
    {
        qDebug("图片加载错误.\n");
        return;
    }

    double scale=1.2;
    static CvScalar colors[] = {
        {{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}},
        {{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}}
    };//Just some pretty colors to draw with
    IplImage* gray = cvCreateImage(cvSize(img->width,img->height),8,1);
    IplImage* small_img=cvCreateImage(cvSize(cvRound(img->width/scale),cvRound(img->height/scale)),8,1);
    cvCvtColor(img,gray, CV_BGR2GRAY);
    cvResize(gray, small_img, CV_INTER_LINEAR);
    cvEqualizeHist(small_img,small_img); //直方图均衡
    cvClearMemStorage(storage);
    double t = (double)cvGetTickCount();
    CvSeq* objects = cvHaarDetectObjects(small_img,
                                         cascade,
                                         storage,
                                         1.1,
                                         2,
                                         0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                         cvSize(30,30));
    t = (double)cvGetTickCount() - t;
    //遍历找到对象和周围画盒
    for(int i=0;i<(objects->total);++i)
    {
        CvRect* r=(CvRect*)cvGetSeqElem(objects,i);
        cvRectangle(img, cvPoint(r->x*scale,r->y*scale), cvPoint((r->x+r->width)*scale,(r->y+r->height)*scale), colors[i%8]);
    }

    for( int i = 0; i < (objects? objects->total : 0); i++ )
    {
        CvRect* r = (CvRect*)cvGetSeqElem( objects, i );
        CvPoint center;
        int radius;
        center.x = cvRound((r->x + r->width*0.5)*scale);
        center.y = cvRound((r->y + r->height*0.5)*scale);
        radius = cvRound((r->width + r->height)*0.25*scale);
        cvCircle(img, center, radius, colors[i%8], 3, 8, 0 );
    }
    show_face(img);  //显示检测的结果
    cvReleaseImage(&gray);
    cvReleaseImage(&small_img);
    //释放图片
    cvReleaseImage(&img);
    qDebug()<<tr("耗时:%1 ms\n").arg(time.elapsed());
}

void MainWindow::show_face(IplImage *img)
{
    uchar *imgData=(uchar *)img->imageData;
    QImage  my_image=QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    my_image=my_image.rgbSwapped(); //BGR格式转RGB
    QPixmap my_pix; //创建画图类
    my_pix.convertFromImage(my_image);
    /*在控件上显示*/
    ui->label_check->setPixmap(my_pix);
}

IplImage *MainWindow::QImageToIplImage(const QImage *qImage)
{
    int width = qImage->width();
        int height = qImage->height();
        CvSize Size;
        Size.height = height;
        Size.width = width;
        IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                QRgb rgb = qImage->pixel(x, y);
                CV_IMAGE_ELEM( IplImageBuffer, uchar, y, x*3+0 ) = qBlue(rgb);
                CV_IMAGE_ELEM( IplImageBuffer, uchar, y, x*3+1 ) = qGreen(rgb);
                CV_IMAGE_ELEM( IplImageBuffer, uchar, y, x*3+2 ) = qRed(rgb);
            }
         }
         return IplImageBuffer;
}

QImage *MainWindow::IplImageToQImage(IplImage *img)
{
    QImage *qmg;
       uchar *imgData=(uchar *)img->imageData;
       qmg = new QImage(imgData,img->width,img->height,QImage::Format_RGB888);
       *qmg=qmg->rgbSwapped(); //BGR格式转RGB
       return qmg;
}

void MainWindow::slot_start_clicked()
{
    QTime time;
    time.start();
    //构造摄像头对象
    printf("%s\n", tr("耗时:%1 ms\n").arg(time.elapsed()).toStdString().c_str());
    QCamera cameraObj;
    printf("%s\n", tr("耗时:%1 ms\n").arg(time.elapsed()).toStdString().c_str());
    ca = new QCamera(ui->comboBox->currentText().toUtf8(),this);
    printf("%s\n", tr("耗时:%1 ms\n").arg(time.elapsed()).toStdString().c_str());
    //构造截屏对象
    cap = new QCameraImageCapture(ca,this);

    //连接截屏信号和显示截屏图像的槽
    QObject::connect(cap,&QCameraImageCapture::imageCaptured,this,&MainWindow::show_pic);

#if 0
    //使用父类对象构造显示窗口类
    w = new QVideoWidget(ui->widget_display);

#else
    w = new QCameraViewfinder(ui->widget_display);

#endif
    //调整大小一致
    w->resize(ui->widget_display->size());

    //设置摄像头的显示窗口
    ca->setViewfinder(w);
    w->show();

    //启动摄像头
    ca->start();

    //禁止开始按钮
    ui->btn_start->setEnabled(false);
    ui->btn_stop->setEnabled(true);
    ui->btn_save->setEnabled(true);
    timer.start(100);
    qDebug()<<tr("耗时:%1 ms\n").arg(time.elapsed()).toStdString().c_str();
    printf("%s\n", tr("耗时:%1 ms\n").arg(time.elapsed()).toStdString().c_str());
}

void MainWindow::slot_stop_clicked()
{
    timer.stop();
    //停止摄像头
    ca->stop();
    //隐藏显示窗口
    w->hide();

    //断开信号
    QObject::disconnect(cap,&QCameraImageCapture::imageCaptured,this,&MainWindow::show_pic);

    delete w;
    delete cap;
    delete ca;

    ui->btn_start->setEnabled(true);
    ui->btn_stop->setEnabled(false);
    ui->btn_save->setEnabled(false);
}

void MainWindow::slot_capture_clicked()
{
    cap->capture();
    //不能立即显示图片
}

void MainWindow::show_pic(int id, const QImage &preview)
{
    qDebug()<<id;
    //把QImage转换成QPixmap
    QPixmap pix = QPixmap::fromImage(preview);

    //保存图片
    QString picpath = QString("./%1.jpg").arg(id);
    pix.save(picpath);

    //显示
    pix = pix.scaled(ui->label_save->size());
    ui->label_save->setPixmap(pix);
}

void MainWindow::slot_TimeOut()
{
    QPixmap pixmap(this->size());

    ui->widget_display->render(&pixmap);
    opencv_face(pixmap.toImage());
}

