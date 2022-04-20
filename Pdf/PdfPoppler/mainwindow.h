#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "mypdfcanvas.h"
#include <QMainWindow>
#include <QScrollBar>
#define SCROLLBAR_WIDTH 30
class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  void resizeEvent(QResizeEvent *e);
  bool setPdfPath(QString path);
  //重新调整pdf界面大小
  void resizeCanvas();
  void setWidgetVisible(bool pdfCanvasVisible, bool scrollbarVisible);
public slots:
  //当拖动pdf上滑到顶（或下滑到底）时触发该方法
  void onPageChange(int currentPage);
  //当滑动条的滑块被滑动时，会调用该方法
  void onScrollBarValueChange();

private:
  MyPdfCanvas *pdfCanvas;
  QScrollBar *scrollbar;
};
#endif // MAINWINDOW_H
