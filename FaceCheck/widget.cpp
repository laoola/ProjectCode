#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

class VideoAudioEncode videoaudioencode_0;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //驾驶室摄像头
    //工作对象
    CreateSignal();
    videoRead_WorkClass_0=new VideoReadThread_0;
    videoRead_Workthread_0=new QThread;
    //连接摄像头采集信号,在主线程实时显示视频画面
    connect(videoRead_WorkClass_0,SIGNAL(VideoDataOutput(QImage )),this,SLOT(VideoDataDisplay_0(QImage )));
    //摄像头初始化函数
    connect(this,SIGNAL(Init_VideoAudio_WorkClass_0()),videoRead_WorkClass_0,SLOT(run()));
    //停止视频采集
    connect(this,SIGNAL(Stop_AudioVideo0_work_0()),videoRead_WorkClass_0,SLOT(stop()));
    //将工作对象移动到子线程里工作
    videoRead_WorkClass_0->moveToThread(videoRead_Workthread_0);
    //更新设备列表
    UpdateVideoAudiodDevice(ui->comboBox_video_0,ui->plainTextEdit_log_0);
    //
    timer.start(100);
    connect(&timer,SIGNAL(timeout()), this, SLOT(update()));
    timer.start(100);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::CreateSignal()
{
    QObject::connect(ui->pushButton_update, SIGNAL(clicked()), this, SLOT(on_pushButton_update_clicked()));
    QObject::connect(ui->pushButton_Start, SIGNAL(clicked()), this, SLOT(on_pushButton_Start_clicked()));
    QObject::connect(ui->pushButton_stop, SIGNAL(clicked()), this, SLOT(on_pushButton_stop_clicked()));
}

//分类器的路径
#define source_xml_addr "/home/zwh/File/opencv-3.4.15/opencv-3.4.15/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml"

//将要检测的图片路径
#define source_pix_addr "/home/zwh/File/check.jpg"

//人脸检测代码
void Widget::opencv_face(QImage qImage)
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
        Log_Display_0("分类器加载错误.\n");
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
        Log_Display_0("图片加载错误.\n");
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
    Log_Display_0(tr("耗时:%1 ms\n").arg(time.elapsed()));
}

/*将QImage图片转为opecv的qimage格式*/
IplImage *Widget::QImageToIplImage(const QImage * qImage)
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

/*将opecv的图片转为qimage格式*/
QImage *Widget::IplImageToQImage(IplImage *img)
{
    QImage *qmg;
    uchar *imgData=(uchar *)img->imageData;
    qmg = new QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    *qmg=qmg->rgbSwapped(); //BGR格式转RGB
    return qmg;
}

//显示检测的结果
void Widget::show_face(IplImage* img)
{
    uchar *imgData=(uchar *)img->imageData;
    QImage  my_image=QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    my_image=my_image.rgbSwapped(); //BGR格式转RGB
    QPixmap my_pix; //创建画图类
    my_pix.convertFromImage(my_image);
    /*在控件上显示*/
    ui->label_display->setPixmap(my_pix);
}

//开始采集
void Widget::on_pushButton_Start_clicked()
{
    //设置当前选择的摄像头
    videoaudioencode_0.camera=video_dev_list.at(ui->comboBox_video_0->currentIndex());
    Stop_VideoAudioEncode_0(true);
    Start_VideoAudioEncode_Thread_0();
}

//析构函数
VideoReadThread_0::~VideoReadThread_0()
{

}

//停止视频采集
void VideoReadThread_0::stop()
{
    qDebug()<<"停止视频采集--stop";
    if(camera)
    {
        camera->stop();
        delete camera;
        camera=nullptr;
    }
    if(m_pProbe)
    {
        delete  m_pProbe;
        m_pProbe=nullptr;
    }
}

//执行线程
void VideoReadThread_0::run()
{
    stop();
    Camear_Init();
    qDebug()<<"摄像头开始采集数据";
}

void VideoReadThread_0::Camear_Init()
{
    /*创建摄像头对象，根据选择的摄像头打开*/
    camera = new QCamera(videoaudioencode_0.camera);
    m_pProbe = new QVideoProbe;
    if(m_pProbe != nullptr)
    {
        m_pProbe->setSource(camera); // Returns true, hopefully.
        connect(m_pProbe, SIGNAL(videoFrameProbed(QVideoFrame)),this, SLOT(slotOnProbeFrame(QVideoFrame)), Qt::QueuedConnection);
    }

    /*配置摄像头捕    QCamera *camera;
    QVideoProbe *m_pProbe;获模式为帧捕获模式*/
    camera->setCaptureMode(QCamera::CaptureStillImage);  //如果在Linux系统下运行就这样设置
    //camera->setCaptureMode(QCamera::CaptureVideo);//如果在android系统下运行就这样设置

    /*启动摄像头*/
    camera->start();

    /*设置摄像头的采集帧率和分辨率*/
    QCameraViewfinderSettings settings;
    settings.setPixelFormat(QVideoFrame::Format_YUYV); //设置像素格式  Android上只支持NV21格式
    settings.setResolution(QSize(VIDEO_WIDTH,VIDEO_HEIGHT)); //设置摄像头的分辨率
    camera->setViewfinderSettings(settings);
}

/**
 * NV21是android相机默认格式
 * @param data
 * @param rgb
 * @param width
 * @param height
 */
void NV21_TO_RGB24(unsigned char *yuyv, unsigned char *rgb, int width, int height)
{
        const int nv_start = width * height ;
        int  index = 0, rgb_index = 0;
        uint8_t y, u, v;
        int r, g, b, nv_index = 0,i, j;

        for(i = 0; i < height; i++){
            for(j = 0; j < width; j ++){
                //nv_index = (rgb_index / 2 - width / 2 * ((i + 1) / 2)) * 2;
                nv_index = i / 2  * width + j - j % 2;

                y = yuyv[rgb_index];
                u = yuyv[nv_start + nv_index ];
                v = yuyv[nv_start + nv_index + 1];

                r = y + (140 * (v-128))/100;  //r
                g = y - (34 * (u-128))/100 - (71 * (v-128))/100; //g
                b = y + (177 * (u-128))/100; //b

                if(r > 255)   r = 255;
                if(g > 255)   g = 255;
                if(b > 255)   b = 255;
                if(r < 0)     r = 0;
                if(g < 0)     g = 0;
                if(b < 0)     b = 0;

                index = rgb_index % width + (height - i - 1) * width;
                //rgb[index * 3+0] = b;
                //rgb[index * 3+1] = g;
                //rgb[index * 3+2] = r;

                //颠倒图像
                //rgb[height * width * 3 - i * width * 3 - 3 * j - 1] = b;
                //rgb[height * width * 3 - i * width * 3 - 3 * j - 2] = g;
                //rgb[height * width * 3 - i * width * 3 - 3 * j - 3] = r;

                //正面图像
                rgb[i * width * 3 + 3 * j + 0] = b;
                rgb[i * width * 3 + 3 * j + 1] = g;
                rgb[i * width * 3 + 3 * j + 2] = r;

                rgb_index++;
            }
        }
}
/*
函数功能: 将YUV数据转为RGB格式
函数参数:
unsigned char *yuv_buffer: YUV源数据
unsigned char *rgb_buffer: 转换之后的RGB数据
int iWidth,int iHeight   : 图像的宽度和高度
*/
void yuyv_to_rgb(unsigned char *yuv_buffer,unsigned char *rgb_buffer,int iWidth,int iHeight)
{
    int x;
    int z=0;
    unsigned char *ptr = rgb_buffer;
    unsigned char *yuyv= yuv_buffer;
    for (x = 0; x < iWidth*iHeight; x++)
    {
        int r, g, b;
        int y, u, v;

        if (!z)
        y = yuyv[0] << 8;
        else
        y = yuyv[2] << 8;
        u = yuyv[1] - 128;
        v = yuyv[3] - 128;

        r = (y + (359 * v)) >> 8;
        g = (y - (88 * u) - (183 * v)) >> 8;
        b = (y + (454 * u)) >> 8;

        *(ptr++) = (r > 255) ? 255 : ((r < 0) ? 0 : r);
        *(ptr++) = (g > 255) ? 255 : ((g < 0) ? 0 : g);
        *(ptr++) = (b > 255) ? 255 : ((b < 0) ? 0 : b);

        if(z++)
        {
            z = 0;
            yuyv += 4;
        }
    }
}

void VideoReadThread_0::slotOnProbeFrame(const QVideoFrame &frame)
{
   QVideoFrame cloneFrame(frame);
   cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
   qDebug()<<"height:"<<cloneFrame.height();
   qDebug()<<"width:"<<cloneFrame.width();
   qDebug()<<"bytesPerLine:"<<cloneFrame.bytesPerLine();
   qDebug()<<"mappedBytes:"<<cloneFrame.mappedBytes();
   qDebug()<<"pixelFormat:"<<cloneFrame.pixelFormat();

   unsigned char rgb_buffer[VIDEO_WIDTH*VIDEO_HEIGHT*3];
   if(cloneFrame.pixelFormat()==QVideoFrame::Format_NV21)
   {
        NV21_TO_RGB24(cloneFrame.bits(),rgb_buffer,cloneFrame.width(),cloneFrame.height());
   }
   else if(cloneFrame.pixelFormat()==QVideoFrame::Format_YUYV)
   {
       yuyv_to_rgb(cloneFrame.bits(),rgb_buffer,cloneFrame.width(),cloneFrame.height());
   }
   else
   {
       qDebug("当前格式编码为%1,暂时不支持转换.\n");
   }
    cloneFrame.unmap();

    //加载图片数据
    QImage image(rgb_buffer,
                       cloneFrame.width(),
                       cloneFrame.height(),
                       QImage::Format_RGB888);

    //绘制图片水印
    QDateTime dateTime(QDateTime::currentDateTime());
    //时间效果: 2020-03-05 16:25::04 周一
    QString qStr="";
    qStr+=dateTime.toString("yyyy-MM-dd hh:mm:ss ddd");
    QPainter pp(&image);
    QPen pen = QPen(Qt::white);
    pp.setPen(pen);
    pp.drawText(QPointF(0,20),qStr);

    //提取RGB数据
    unsigned char *p=rgb_buffer;
    for(int i=0;i<image.height();i++)
    {
        for(int j=0;j<image.width();j++)
        {
            QRgb rgb=image.pixel(j,i);
            *p++=qRed(rgb);
            *p++=qGreen(rgb);
            *p++=qBlue(rgb);
        }
    }
    emit VideoDataOutput(image); //发送信号
}

//驾驶室:视频刷新显示
void Widget::VideoDataDisplay_0(QImage image)
{
    QPixmap my_pixmap;
    my_pixmap.convertFromImage(image);
    /*在控件上显示*/
    ui->label_ImageDisplay_0->setPixmap(my_pixmap);
}

//驾驶室:日志显示
void Widget::Log_Display_0(QString text)
{
    Log_Text_Display(ui->plainTextEdit_log_0,text);
}

/*日志显示*/
void Widget::Log_Text_Display(QPlainTextEdit *plainTextEdit_log,QString text)
{
    plainTextEdit_log->insertPlainText(text);
    //移动滚动条到底部
    QScrollBar *scrollbar = plainTextEdit_log->verticalScrollBar();
    if(scrollbar)
    {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

//驾驶室:开启所有采集线程
void Widget::Start_VideoAudioEncode_Thread_0()
{
    videoRead_Workthread_0->start(); //开启视频采集线程
    Init_VideoAudio_WorkClass_0(); //发送初始化信号
}

//驾驶室:退出所有采集线程
void Widget::Stop_VideoAudioEncode_0(bool flag)
{
    if(flag==true)
    {
        Stop_AudioVideo0_work_0(); //发送信号停止摄像头
        QThread::msleep(10);
        //退出视频采集
        videoRead_Workthread_0->quit(); //告诉线程的事件循环以return 0（成功）退出
        videoRead_Workthread_0->wait(); //等待线程退出
    }
}

void Widget::on_pushButton_update_clicked()
{
    UpdateVideoAudiodDevice(ui->comboBox_video_0,ui->plainTextEdit_log_0);
}

/*刷新本机可以的音频设备列表*/
void Widget::UpdateVideoAudiodDevice(
        QComboBox *comboBox_video,
        QPlainTextEdit *plainTextEdit_log)
{
    /*2. 获取摄像头列表*/
    video_dev_list.clear();
    comboBox_video->clear();
    video_dev_list=QCameraInfo::availableCameras();
    for(int i=0;i<video_dev_list.size();i++)
    {
        comboBox_video->addItem(video_dev_list.at(i).deviceName());
    }
    /*如果没有可用的摄像头设备，按钮不可用*/
    if(video_dev_list.size()==0)
    {
        Log_Text_Display(plainTextEdit_log,"未查询到可用的摄像头设备.\n");
    }
}

//停止采集
void Widget::on_pushButton_stop_clicked()
{
    Stop_VideoAudioEncode_0(true);
}

void Widget::update()
{
    if(ui->label_ImageDisplay_0->pixmap())
    opencv_face(ui->label_ImageDisplay_0->pixmap()->toImage());
}
