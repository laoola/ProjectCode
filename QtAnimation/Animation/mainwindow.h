#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showEvent(QShowEvent *event);

    void CreateSignal();
    void InitWindow();

public slots:
    void slotBtnPrePage();
    void slotBtnNextPage();
    void slotBtnGoPage();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
