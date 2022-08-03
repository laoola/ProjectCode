/******************************************************************************
 * Copyright 2020-2022 Co., Ltd.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       main.cpp
 * @brief       后台截屏
 *
 * @author     zhangwenhao
 * @date       2022/06/02
 * @history
 *****************************************************************************/
#include <QApplication>
#include <QWindow>
#include <QScreen>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
//#include <QClipboard>
//#include <QMimeData>


//void GetClipboardImage()
//{
//    QClipboard *clipboard = QApplication::clipboard();
//    const QMimeData *mimeData = clipboard->mimeData();
//    if (mimeData->hasImage())
//    {
//        //将图片数据转为QImage
//        QImage img = qvariant_cast<QImage>(mimeData->imageData());
//        if(!img.isNull())
//        {
//           //绘制显示图片
//           img.save("/home/zwh/Desktop/clipboard.png");
//        }
//        else
//        {
//            printf("clipboard is null.\n");
//        }
//    }
//}



int main(int argc, char *argv[])
{
    printf("ghaSrceenHot version:(%s, %s)\n", __DATE__, __TIME__);

    for(int i = 0; i < argc; ++i)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    if(argc != 3)
    {
        printf("must set hot.png path and quality.\n");

        return 0;
    }

    QApplication app(argc, argv);
    QScreen *screen = QGuiApplication::primaryScreen();
    std::string strPngfile = argv[1];
    std::string strPngPath = "";
    int nQuality = atoi(argv[2]);
    int npos = strPngfile.rfind("/");

    strPngPath = (npos == -1) ? strPngfile : strPngfile.substr(0, npos);

    struct stat st;
    if (0 != lstat(strPngPath.c_str(), &st) && !S_ISDIR(st.st_mode))
    {
        printf("not path %s.\n", strPngPath.c_str());

        return 0;
    }

    if(nQuality < -1 || nQuality > 100)
    {
        printf("quality[-1, 100] %d.\n", nQuality);

        return 0;
    }

    if(screen->grabWindow(0).save(strPngfile.c_str(), Q_NULLPTR, nQuality))
    {
        printf("hot success %s\n", strPngfile.c_str());
    }
    else
    {
        printf("hot fail %s\n", strPngfile.c_str());
    }

    //GetClipboardImage();

    exit(0);
    return app.exec();
}
