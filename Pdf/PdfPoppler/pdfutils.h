#ifndef PDFUTILS_H
#define PDFUTILS_H
#include "poppler-qt5.h"
#include <QDebug>
#include <QImage>
#include <QObject>
#include <QSize>
class PdfUtils {
public:
  explicit PdfUtils(QString filePath);
  ~PdfUtils();
  //获取指定页pdf图像(页码从0开始)
  QImage getPdfImage(int pageNumber);
  //获取pdf总页码
  int getNumPages();
  //获取pdf页面大小
  QSize getPageSize();

private:
  QString filePath;
  int numPages;
  QSize pageSize;
  void getPdfInfo();
};
#endif // PDFUTILS_H
