#include "majorimageprocessingthread.h"

MajorImageProcessingThread::MajorImageProcessingThread()
{
    qDebug()<<"MajorImageProcessingThread init";
    stopped = false;
    majorindex = -1;
}

void MajorImageProcessingThread::stop()
{
    stopped = true;
}

void MajorImageProcessingThread::init(int index)
{
    stopped = false;
    majorindex = index;
}

void MajorImageProcessingThread::run()
{
    if(majorindex != -1)
    {
        while(!stopped)
        {
            msleep(1000/30);

            QImage img;
            int ret = LPF_GetFrame();
            if(ret == 0)
            {
                int WV = LPF_GetCurResWidth();
                int HV = LPF_GetCurResHeight();
                img = QImage(rgb24, WV, HV, QImage::Format_RGB888);
            }

            emit SendMajorImageProcessing(img, ret);
            emit SendImgaeFaceCheck(img);
        }
    }
}


FaceCheckThread::FaceCheckThread()
{
    qDebug()<<"init FaceCheckThread";
    bStop = false;
}

void FaceCheckThread::stop()
{
    bStop = true;
}


//分类器的路径
#define source_xml_addr "/home/zwh/File/opencv-3.4.15/opencv-3.4.15/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml"

//将要检测的图片路径
#define source_pix_addr "/home/zwh/File/check.jpg"

void FaceCheckThread::opencvFace(QImage qImage)
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
        printf("分类器加载错误.\n");
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
        printf("图片加载错误.\n");
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
    showFace(img);  //显示检测的结果
    cvReleaseImage(&gray);
    cvReleaseImage(&small_img);
    //释放图片
    cvReleaseImage(&img);
    printf("耗时:%d ms\n", time.elapsed());


}

void FaceCheckThread::showFace(IplImage *img)
{
    uchar *imgData=(uchar *)img->imageData;
    QImage  my_image=QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    my_image=my_image.rgbSwapped(); //BGR格式转RGB
    QPixmap my_pix; //创建画图类
    my_pix.convertFromImage(my_image);
    /*在控件上显示*/
    emit SendFaceCheckRlt(my_image);
  //  ui->label_check->setPixmap(my_pix);
}

IplImage *FaceCheckThread::QImageToIplImage(const QImage *qImage)
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

QImage *FaceCheckThread::IplImageToQImage(IplImage *img)
{
    QImage *qmg;
    uchar *imgData=(uchar *)img->imageData;
    qmg = new QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    *qmg=qmg->rgbSwapped(); //BGR格式转RGB
    return qmg;
}

void FaceCheckThread::run()
{
    while(!bStop)
    {
         msleep(1000/30);
        if(!m_qImage.isNull())
        {
            opencvFace(m_qImage);
        }
    }

}

void FaceCheckThread::slot_setCheckImage(QImage image)
{
    qDebug()<<"recv face check image";
    m_qImage = image;
}
