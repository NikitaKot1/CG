#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <QColor>
#include <QPalette>

static int red;
static int blue;
static int green;
static QColor mycolor;
static QColor pencolor;
static QColor frontcolor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    red = 0;
    blue = 0;
    green = 0;
    mycolor = QColor(0, 0, 0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_sliderRed_valueChanged(int value)
{
    QPalette plt;
    red = value;
    mycolor = QColor(red, green, blue);
    plt.setColor(QPalette::Background, mycolor);
    ui->label->setPalette(plt);
}

void MainWindow::setPixel(int x, int y)
{
    ui->graphicsView->scene()->addLine(x, y, x, y, QPen(pencolor));
}

void MainWindow::on_sliderBlue_valueChanged(int value)
{
    QPalette plt;
    blue = value;
    mycolor = QColor(red, green, blue);
    plt.setColor(QPalette::Background, mycolor);
    ui->label->setPalette(plt);
}


void MainWindow::on_sliderGreen_valueChanged(int value)
{
    QPalette plt;
    green = value;
    mycolor = QColor(red, green, blue);
    plt.setColor(QPalette::Background, mycolor);
    ui->label->setPalette(plt);
}


void MainWindow::on_buttonSetFront_clicked()
{
    frontcolor = mycolor;
    ui->graphicsView->scene()->setBackgroundBrush(QBrush(frontcolor));
    QPalette plt;
    plt.setColor(QPalette::Background, frontcolor);
    ui->frontCol->setPalette(plt);
}


void MainWindow::on_buttonSetLine_clicked()
{
    QPalette plt;
    plt.setColor(QPalette::Background, mycolor);
    pencolor = mycolor;
    ui->lineCol->setPalette(plt);
}


void MainWindow::on_buttonClear_clicked()
{
    ui->graphicsView->scene()->clear();
}


void MainWindow::on_drawSpectr_clicked()
{
    /*if (ui->radioButStandart->isChecked())
    {

    }*/
    drawSpector();
}

void MainWindow::drawBrezenheimInt(int xn, int yn, int xk, int yk)
{
    int dx = abs(xk - xn);
    int dy = abs(yk - yn);
    int signx = xn < xk ? 1 : -1;
    int signy = yn < yk ? 1 : -1;
    int err = dx - dy;
    while (xn != xk || yn != yk)
    {
        setPixel(xn, yn);
        int err2 = err * 2;
        if (err2 > - dy)
        {
            err -= dy;
            xn += signx;
        }
        if (err2 < dx)
        {
            err += dx;
            yn += signy;
        }
    }
}

void MainWindow::drawDiffAnaliz(int xn, int yn, int xk, int yk)
{
    int l = abs(xk - xn) > abs(yk - yn) ? abs(xk - xn) : abs(yk - yn);
    double dx = (double)(xk - xn) / double(l);
    double dy = (double)(yk - yn) / double(l);
    double x1 = xn, y1 = yn;
    setPixel(xn, yn);
    for (int i = 0; i < l; i++)
    {
        x1 += dx;
        y1 += dy;
        setPixel(x1, y1);
    }
}

void MainWindow::drawSpector()
{
    int xc = ui->graphicsView->scene()->width() / 2;
    int yc = ui->graphicsView->scene()->height() / 2;
    int n = ui->spinBox->value();
    int r = ui->spinBox_2->value();
    double ang = 2 * M_PI / n;
    for (int i = 0; i < n; i++)
    {
        double x = xc + r * cos(ang * i);
        double y = yc + r * sin(ang * i);
        if (ui->radioButStandart->isChecked())
            ui->graphicsView->scene()->addLine(xc, yc, x, y, QPen(pencolor));
        if (ui->radioBrenInt->isChecked())
            drawBrezenheimInt(xc, yc, x, y);
        if (ui->redioDifAnal->isChecked())
            drawDiffAnaliz(xc, yc, x, y);
    }
}

void MainWindow::on_drawLine_clicked()
{
    int xn = ui->spinStartX->value();
    int yn = ui->spinStartY->value();
    int xk = ui->spinEndX->value();
    int yk = ui->spinEndY->value();
    if (ui->radioButStandart->isChecked())
        ui->graphicsView->scene()->addLine(xn, yn, xk, yk, QPen(pencolor));
    if (ui->radioBrenInt->isChecked())
        drawBrezenheimInt(xn, yn, xk, yk);
    if (ui->redioDifAnal->isChecked())
        drawDiffAnaliz(xn, yn, xk, yk);

}

