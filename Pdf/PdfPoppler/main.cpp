#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  //    PdfUtils pdfObj("/home/zwh/Desktop/policy.pdf");

  //    system("rm -rfd /home/zwh/project/bin/pages/*");
  //    for(int i = 0; i < pdfObj.getNumPages(); ++i)
  //    {
  //        QString strPage = "/home/zwh/project/bin/pages/PNG";
  //        strPage += QString("%1").arg(i, 4, 10, QChar('0'));
  //        strPage += ".png";
  //        bool bRlt = pdfObj.getPdfImage(i).save(strPage);

  //        printf("%d %s\n", bRlt, strPage.toStdString().c_str());
  //    }

  //    exit(0);

  MainWindow w;
  w.resize(500, 500);
  w.show();
  QString path = "/home/zwh/Desktop/policy.pdf";
  w.setPdfPath(path);
  w.setWindowTitle(path);

  return a.exec();
}
