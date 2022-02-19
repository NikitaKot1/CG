#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_drawButton_clicked();

private:
    int SayNigga();
    int getDotsParams();
    void drawDot(double x, double y, int i);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;

};
#endif // MAINWINDOW_H
