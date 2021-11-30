//opencv include
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "opencv2/opencv.hpp"

#include <QCamera>
#include <QVideoProbe>
#include <QAbstractVideoSurface>
#include <QVideoProbe>
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <QCamera>
#include <QPen>
#include <QPainter>
#include <QRgb>
#include <QDateTime>
#include <QCameraInfo>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QComboBox>
#include <QTimer>

#include <sys/stat.h>
#include <dirent.h>


//头文件
#include <dirent.h>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/contrib/contrib.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<iostream>

//分类器的路径
#define source_xml_addr "/home/zwh/File/opencv-3.4.15/opencv-3.4.15/data/haarcascades_cuda/haarcascade_frontalface_alt2.xml"

void getFilesFromFolder(const std::string& folderName,std::vector<std::string>& vFiles/*,const std::string strFileType*/)
{
    if (folderName.empty()) { return; } //字符串判断

    const int iRecursiveTimes = 3;      //目标递归层数
    static int s_iRecursiveTimes = 0;   //当前递归层数

    //文件夹判断
    struct stat stFolderStat;
    stat(folderName.c_str(), &stFolderStat);
    if (!S_ISDIR(stFolderStat.st_mode)) { return; }

    DIR* open_dir = opendir(folderName.c_str());
    if (NULL == open_dir) { return; }   //打开失败

    dirent* pDirent = NULL;
    while( (pDirent = readdir(open_dir)) != NULL) {
        struct stat stFileStata;
        if (pDirent->d_name[0] != '.') {
            std::string name = folderName+std::string("/")+std::string(pDirent->d_name);
            stat(name.c_str(), &stFileStata);

            if (S_ISDIR(stFileStata.st_mode)) {//是目录就进行递归
                if(iRecursiveTimes == s_iRecursiveTimes) { continue; }
                else{ s_iRecursiveTimes++; }
                //getFilesFromFolder(name,vFiles,strFileType);
                s_iRecursiveTimes--;
            }
            else if (S_ISREG(stFileStata.st_mode))
            {
                //if(name.find(strFileType,0)>=0)
                {
                    vFiles.push_back(name);
                } //进行类型判断并保存文件名
            }
        }
    }
    closedir(open_dir);
}

void showFace(IplImage* img)
{
    uchar *imgData=(uchar *)img->imageData;
    QImage  my_image=QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    my_image=my_image.rgbSwapped(); //BGR格式转RGB
    QPixmap my_pix; //创建画图类
    my_pix.convertFromImage(my_image);
    /*在控件上显示*/
    my_pix.save(QString("/home/zwh/File/CheckImage/first/%2.png").arg(QTime::currentTime().toString("hh_mm_ss_zzz")));
}

IplImage *QImageToIplImage(const QImage *qImage)
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

QImage *IplImageToQImage(IplImage *img)
{
    QImage *qmg;
    uchar *imgData=(uchar *)img->imageData;
    qmg = new QImage(imgData,img->width,img->height,QImage::Format_RGB888);
    *qmg=qmg->rgbSwapped(); //BGR格式转RGB
    return qmg;
}


void opencvFace(QImage qImage)
{
    QTime time;
    time.start();

    static CvMemStorage* storage = NULL;
    static CvHaarClassifierCascade* cascade = NULL;
    const char *cascade_name =source_xml_addr;
    //加载分类器
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
    if( !cascade )
    {
        printf("分类器加载错误.\n");
        return ;
    }

    //创建内存空间
    storage = cvCreateMemStorage(0);
    IplImage* img = QImageToIplImage(&qImage);

    if(img == NULL)
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
                                         5,
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
    printf("耗时:%d ms %d个人脸\n", time.elapsed(), objects->total);
}

void faceCapture(QString strFileName)
{
    cv::CascadeClassifier cascade;
    std::string imageName, cascadeName;
    cv::Mat image, gray;
    std::vector<cv::Rect> faces;
    cv::Scalar greenColor = cv::Scalar(0,255,0);

    cascadeName = source_xml_addr;
    imageName = strFileName.toStdString().c_str();
    //load image and class
    if(!cascade.load(cascadeName)){
        std::cerr << "Could not load classifier cascade" << std::endl;
    }else if(imageName.empty()){
        std::cout<<"Could not read"<<imageName<<std::endl;
    }else{
        image = cv::imread(imageName,1);
        if (image.empty())
        {
            std::cout << "Could not imread image" << std::endl;
            /* code */
        }
    }
    //convert to gray
    cv::cvtColor(image,gray,cv::COLOR_BGR2GRAY);

    // run face detector
    cascade.detectMultiScale(image,faces,1.1,5,0,cv::Size(30,30));
    printf("人脸个数: %d\n", faces.size());

    //output rectange
    for(size_t i = 0; i < faces.size(); ++i){
        cv::Rect r = faces[i];
        cv::rectangle(
                    image,cv::Point(cvRound(r.x),cvRound(r.y)),
                    cv::Point(cvRound(r.x+r.width-1),cvRound(r.y+r.height-1)),
                    greenColor,3,8,0);
    }

    std::vector<int> compression_params;
    if(CV_VERSION_MAJOR==3)
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    // if OpenCV2 comment this
    else
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);

    // save image
    imwrite(QString("/home/zwh/File/CheckImage/secend/%2.png").arg(QTime::currentTime().toString("hh_mm_ss_zzz")).toStdString().c_str()
            , image, compression_params);
}


//命名空间
using namespace std;
using namespace cv;

CascadeClassifier faceDetect;   //人脸检测对象

vector<Rect> FaceDetector(Mat img)
{
    Mat src = Mat::zeros(240, 320, img.type());
    vector<Rect> faces;
    cvtColor(img, img, CV_RGB2GRAY);
    resize(img, src, src.size());
    faceDetect.detectMultiScale(img, faces, 1.2, 6, 0,Size(30,30));
    cout <<faces.size()<< endl;
    return faces;
}

void DrawFace(Mat img, vector<Rect> faces)
{
    for(size_t num = 0; num<faces.size(); num++)
    {
        rectangle(img, Point(faces[num].x, faces[num].y),
                  Point(faces[num].x + faces[num].width, faces[num].y + faces[num].height), Scalar(0, 255, 0), 1, 8);
    }
}
void GetFilesNameAndLabels(char *path, vector<string>& filesName,vector<string>& labelsInfo)
{
    string p;
    DIR * dir;
    struct dirent * ptr;
    dir = opendir(path); //打开一个目录
    while((ptr = readdir(dir)) != NULL) //循环读取目录数据
    {
        if(ptr->d_name[0] != '.')//d_name是一个char数组，存放当前遍历到的文件名
        {

            filesName.push_back(p.assign(path).append("/").append(ptr->d_name));
            labelsInfo.push_back(ptr->d_name);
            cout <<p << endl;

        }
    }
    closedir(dir);//关闭目录指针
}

int FaceCheck()
{
    //变量
    Mat frame;
    Mat src;
    Mat facePic;
    Mat recogPic;

    vector<Rect> faces;
    vector<string> filesName;
    char *path = "./smalljpgset";
    string picture;
    string pic;
    vector<Mat> trainPic;//训练图片
    vector<int> labels;//与训练图片一一对应的标签
    int result;
    vector<string> labelsInfo;
    faceDetect.load("./models/haarcascade_frontalface_alt2.xml");

    Ptr<FaceRecognizer> LBPHRecog = createLBPHFaceRecognizer(1, 8 ,3, 3, 10);
    //加载人脸库
    GetFilesNameAndLabels(path, filesName,labelsInfo);
    cout <<filesName.size()<< endl;
    for(int i = 0; i<filesName.size(); i++)
    {
        pic = filesName[i];
        src = imread(pic);
        cvtColor(src, src, CV_RGB2GRAY);
        trainPic.push_back(src);
        labels.push_back(i);
    }

    LBPHRecog->train(trainPic, labels);         //LBP人脸识别训练函数
    picture="./picture/Osama_bin_Laden_0001.jpg";
    frame=imread(picture);
    recogPic = Mat::zeros(200, 200, frame.type());
    faces = FaceDetector(frame);
    DrawFace(frame, faces);
    imshow("1",frame);
    waitKey(0);
    for(size_t i = 0; i<faces.size(); i++)
    {
        facePic = frame(Rect(faces[i]));
        resize(facePic, recogPic, recogPic.size());
        cvtColor(recogPic, recogPic, CV_RGB2GRAY);
        result = LBPHRecog->predict(recogPic);//进行识别

        //将识别结果显示在实时画面上
        if(result ==-1)
        {
            putText(frame, "unknow", Point(faces[i].x, faces[i].y), 3, 0.5, Scalar(0, 255, 255), 1);

        }
        else
        {
            putText(frame, labelsInfo[result], Point(faces[i].x, faces[i].y), 3, 0.5, Scalar(255, 0, 0) ,1,5);
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    std::vector<std::string> vtImageFile;
    getFilesFromFolder("/home/zwh/File/CheckImage", vtImageFile);

    std::vector<std::string>::iterator iter = vtImageFile.begin();

    for(; iter != vtImageFile.end(); ++iter)
    {
        QPixmap pixmap;
        if(!pixmap.load(iter->c_str()))
        {
            QFile file(iter->c_str());
            if(!file.open(QIODevice::ReadOnly))
            {
                printf("Load image error.");

                continue;
            }
            pixmap.loadFromData(file.readAll());
        }
        opencvFace(pixmap.toImage());
        faceCapture(iter->c_str());
    }

    return 0;
}
