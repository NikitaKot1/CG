#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

struct BirdDots
{
    double head_ug;
    double headc_x;
    double headc_y;
    double head_a;
    double head_b;

    double beak1_x;
    double beak2_x;
    double beak3_x;
    double beak1_y;
    double beak2_y;
    double beak3_y;

    double body_ug;
    double bodyc_x;
    double bodyc_y;
    double body_a;
    double body_b;

    double wing1_x;
    double wing2_x;
    double wing3_x;
    double wing1_y;
    double wing2_y;
    double wing3_y;

    double tail1_x;
    double tail2_x;
    double tail3_x;
    double tail1_y;
    double tail2_y;
    double tail3_y;

    double legr1_x;
    double legr2_x;
    double legl3_x;
    double legl4_x;
    double legr1_y;
    double legr2_y;
    double legl3_y;
    double legl4_y;
};

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

    void on_transferButton_clicked();

    void on_dackButton_clicked();

private:
    void drawBird(BirdDots bird);
    void drawStartBird();
    void setPixel(double x, double y);
    void drawEllipse(double x1, double y1, double x2, double y2, double ugl);
    void drawLine(double x1, double y1, double x2, double y2);
    void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3);
    void drawAxis();

    void transferBird(BirdDots bird, double x, double y);
    void scaleBird(BirdDots bird, double x, double y, double kx, double ky);
    void rotateBird(BirdDots bird, double x, double y, double ugl);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
};
#endif // MAINWINDOW_H
