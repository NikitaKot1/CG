#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void on_sliderRed_valueChanged(int value);
    void on_sliderBlue_valueChanged(int value);
    void on_sliderGreen_valueChanged(int value);
    void on_buttonSetFront_clicked();
    void on_buttonSetLine_clicked();
    void on_buttonClear_clicked();
    void on_drawSpectr_clicked();
    void on_drawLine_clicked();

private:
    void drawSpector();
    void setPixel(int x, int y);

    void drawBrezenheimFloat(int xn, int yn, int xk, int yk);
    void drawBrezenheimInt(int xn, int yn, int xk, int yk);
    void drawBrezenheimGlue(int xn, int yn, int xk, int yk);
    void drawVu(int xn, int yn, int xk, int yk);
    void drawDiffAnaliz(int xn, int yn, int xk, int yk);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
};
#endif // MAINWINDOW_H
