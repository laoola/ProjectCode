#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  pdfCanvas = new MyPdfCanvas(this);
  scrollbar = new QScrollBar(Qt::Vertical, this);
  setWidgetVisible(false, false);
  connect(pdfCanvas, SIGNAL(pageChanged(int)), this, SLOT(onPageChange(int)));
  connect(scrollbar, SIGNAL(valueChanged(int)), this,
          SLOT(onScrollBarValueChange()));
}
MainWindow::~MainWindow() {}
void MainWindow::resizeEvent(QResizeEvent *e) { resizeCanvas(); }
bool MainWindow::setPdfPath(QString path) {
  bool result = pdfCanvas->setPath(path);
  if (result) {
    int numPages = pdfCanvas->getNumPages();
    if (numPages > 1) {
      scrollbar->setMaximum(numPages - 1);
      scrollbar->setValue(0);
    }
    pdfCanvas->setPage(0);
  }
  resizeCanvas();
  return result;
}
void MainWindow::resizeCanvas() {
  qDebug() << "resize " << this->rect() << ", " << pdfCanvas->rect();
  int numPages = pdfCanvas->getNumPages();
  if (numPages == 1) {
    pdfCanvas->setGeometry(this->rect());
    setWidgetVisible(true, false);
  } else if (numPages > 1) {
    pdfCanvas->setGeometry(0, 0, this->width() - SCROLLBAR_WIDTH,
                           this->height());
    scrollbar->setGeometry(this->width() - SCROLLBAR_WIDTH, 0,
                           this->width() - SCROLLBAR_WIDTH, this->height());
    setWidgetVisible(true, true);
  } else {
    // numPages <= 0
    setWidgetVisible(false, false);
  }
}
void MainWindow::setWidgetVisible(bool pdfCanvasVisible,
                                  bool scrollbarVisible) {
  pdfCanvas->setVisible(pdfCanvasVisible);
  scrollbar->setVisible(scrollbarVisible);
}
void MainWindow::onPageChange(int currentPage) {
  pdfCanvas->setPage(currentPage);
}
void MainWindow::onScrollBarValueChange() {
  pdfCanvas->setPage(scrollbar->value());
}
