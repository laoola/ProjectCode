#ifndef MYPDFCANVAS_H
#define MYPDFCANVAS_H
#include "pdfutils.h"
#include <QMap>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QResizeEvent>
#include <QVector>
#include <QWidget>
class MyPdfCanvas : public QWidget {
  Q_OBJECT
public:
  explicit MyPdfCanvas(QWidget *parent = 0);
  ~MyPdfCanvas();
  void resizeEvent(QResizeEvent *e);
  void paintEvent(QPaintEvent *e);
  void mousePressEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *e);
  void setMaxCachedNum(int maxCachedNum);
  //如果不能解析pdf返回false
  bool setPath(QString pdfPath);
  //页码从0开始
  bool setPage(int pageNumber);
  //获取页数
  int getNumPages();
  float getScaledRatio();
  //显示裁剪后的图片
  bool showClipImage(int pageNumber, int x, int y, int w, int h);
  //取消显示裁剪图片,恢复正常显示
  void cancelClip();
  //实际获取的pdf宽高度
  QSize pdfActualSize;
signals:
  void pageChanged(int currentPage);

private:
  PdfUtils *pdfUtils;
  QString pdfPath;
  //最大缓存图片数量
  int maxCachedNum;
  //用来缓存pdf的每一个页面的图像(从0开始)
  QMap<int, QImage> cachedImageMap;
  //用来存储已缓存的pdf页面序号(从0开始)
  //  QQueue<int> cachedPageQueue;
  //当前页码(从0开始)
  int currentPage;
  //总页码(从0开始)
  int numPages;
  bool isMouseDown;
  int lastMouseY;
  //当前pdf页面的图像
  QImage image;
  int imageX;
  int imageY;
  int imageMinY;
  //是否是剪裁状态
  bool isClip;
  //获取指定页的图片
  bool getPdfImage(int pageNumber);
  void reachTop();
  void reachBottom();
  //判断是否需要发送重定位签名框的信号
  void needLocateSignArea();
};
#endif // MYPDFCANVAS_H
