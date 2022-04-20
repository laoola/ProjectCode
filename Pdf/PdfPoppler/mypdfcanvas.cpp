#include "mypdfcanvas.h"
MyPdfCanvas::MyPdfCanvas(QWidget *parent) : QWidget(parent) {
  pdfUtils = NULL;
  imageX = 0;
  imageY = 0;
  isClip = false;
  setAutoFillBackground(true);
}
MyPdfCanvas::~MyPdfCanvas() {
  if (pdfUtils != NULL)
    delete pdfUtils;
}
void MyPdfCanvas::resizeEvent(QResizeEvent *e) {
  image = this->cachedImageMap[currentPage];
  if (!image.isNull()) {
    float radio = (float)e->size().width() / (float)e->oldSize().width();
    int imageHeight = image.height() * e->size().width() / image.width();
    image = image.scaled(e->size().width(), imageHeight);
    if (imageHeight < this->height()) {
      imageY = (this->height() - imageHeight) / 2;
      //如果图片高度小于控件高度，则图片居中
      //      imageMinY = imageY;
      imageMinY = 0;
      imageY = imageMinY;
    } else {
      if (radio > 0) {
        imageY = (int)(imageY * radio);
        if (imageY > 0) {
          imageY = 0;
        }
      } else {
        imageY = 0;
      }
    }
  }
}
void MyPdfCanvas::paintEvent(QPaintEvent *e) {
  QPainter *painter = new QPainter(this);
  if (image.isNull()) {
    painter->fillRect(this->rect(), Qt::transparent);
    return;
  }
  painter->drawImage(0, imageY, image);
  delete painter;
}
void MyPdfCanvas::mousePressEvent(QMouseEvent *e) {
  isMouseDown = true;
  lastMouseY = e->y();
}
void MyPdfCanvas::mouseReleaseEvent(QMouseEvent *e) { isMouseDown = false; }
void MyPdfCanvas::mouseMoveEvent(QMouseEvent *e) {
  if (!isMouseDown || image.isNull()) {
    return;
  }
  int distance = e->y() - lastMouseY;
  lastMouseY = e->y();
  imageY += distance;
  if (imageY > 0) {
    imageY = 0;
    reachTop();
    return;
  } else if (imageY < imageMinY) {
    imageY = imageMinY;
    reachBottom();
    return;
  }
  update();
}
void MyPdfCanvas::setMaxCachedNum(int maxCachedNum) {
  this->maxCachedNum = maxCachedNum;
}
bool MyPdfCanvas::setPath(QString pdfPath) {
  this->pdfPath = pdfPath;
  if (pdfUtils != NULL)
    delete pdfUtils;
  pdfUtils = new PdfUtils(pdfPath);
  numPages = pdfUtils->getNumPages();
  if (numPages > 0) {
    isClip = false;
    pdfActualSize = pdfUtils->getPageSize();
  }
  cachedImageMap.clear();
  currentPage = 0;
  imageY = 0;
  lastMouseY = 0;
  return numPages > 0;
}
bool MyPdfCanvas::setPage(int pageNumber) {
  if (!getPdfImage(pageNumber)) {
    return false;
  }
  isClip = false;
  isMouseDown = false;
  image = image.scaledToWidth(this->width());
  imageMinY = this->height() - image.height();
  if (image.height() < this->height()) {
    //如果图片高度小于控件高度，则图片居中
    //    imageMinY /= 2;
    imageMinY = 0;
    imageY = imageMinY;
  } else {
    imageY = 0;
  }
  update();
  return true;
}
int MyPdfCanvas::getNumPages() { return numPages; }
float MyPdfCanvas::getScaledRatio() {
  int pdfWidth = pdfUtils->getPageSize().width();
  return (float)this->width() / (float)pdfWidth;
}
bool MyPdfCanvas::showClipImage(int pageNumber, int x, int y, int w, int h) {
  if (!getPdfImage(pageNumber)) {
    return false;
  }
  isClip = true;
  imageY = 0;
  image = image.copy(x, y, w, h).scaled(this->size());
  update();
}
void MyPdfCanvas::cancelClip() {
  isClip = false;
  setPage(currentPage);
}
bool MyPdfCanvas::getPdfImage(int pageNumber) {
  if (pageNumber < 0 || pageNumber >= numPages) {
    return false;
  }
  if (cachedImageMap.contains(pageNumber)) {
    image = cachedImageMap.value(pageNumber);
  } else {
    image = pdfUtils->getPdfImage(pageNumber);
    if (!image.isNull()) {
      cachedImageMap[pageNumber] = image;
      pdfActualSize = image.size();
    }
  }
  if (image.isNull()) {
    return false;
  }
  currentPage = pageNumber;
  return true;
}
void MyPdfCanvas::reachTop() {
  if (currentPage > 0) {
    emit pageChanged(currentPage - 1);
  }
}
void MyPdfCanvas::reachBottom() {
  if (currentPage < numPages - 1) {
    emit pageChanged(currentPage + 1);
  }
}
