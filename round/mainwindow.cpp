#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColor>
#include <cmath>

QColor pencolor;
QColor frontcolor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    scene.setSceneRect(0, 0, 1200, 750);
    pencolor = QColor(255, 255, 255);
    frontcolor = QColor(0, 0, 0);
    ui->graphicsView->scene()->setBackgroundBrush(QBrush(frontcolor));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPixel(int x, int y)
{
    ui->graphicsView->scene()->addLine(x, y, x, y, pencolor);
}

void MainWindow::on_buttonDrawOkr_clicked()
{
    int xc = ui->spinCenterX->value();
    int yc = ui->spinCenterY->value();
    if (ui->radioR->isChecked())
    {
        int r = ui->spinBoxR->value();
        if (ui->radioBiblio->isChecked())
            ui->graphicsView->scene()->addEllipse(QRect(xc - r, yc - r, 2 * r, 2 * r), QPen(pencolor));
        if (ui->radioParam->isChecked())
            drawParamOkr(xc, yc, r, true);
        if (ui->radioKanon->isChecked())
            drawCanonOkr(xc, yc, r, true);
        if (ui->radioSrDot->isChecked())
            drawSrDotOkr(xc, yc, r, true);
        if (ui->radioBrez->isChecked())
            drawBrezenheimOkr(xc, yc, r, true);
    }
    if (ui->radioAB->isChecked())
    {
        int a = ui->spinBoxA->value();
        int b = ui->spinBoxB->value();
        if (ui->radioBiblio->isChecked())
            ui->graphicsView->scene()->addEllipse(QRect(xc - a, yc - b, 2 * a, 2 * b), QPen(pencolor));
        if (ui->radioParam->isChecked())
            drawParamEll(xc, yc, a, b, true);
        if (ui->radioKanon->isChecked())
            drawCanonEll(xc, yc, a, b, true);
        if (ui->radioSrDot->isChecked())
            drawSrDotEll(xc, yc, a, b, true);
        if (ui->radioBrez->isChecked())
            drawBrezenheimEll(xc, yc, a, b, true);
    }

}

void MainWindow::on_radioBlue_clicked()
{
    pencolor = Qt::blue;
}

void MainWindow::on_radioRed_clicked()
{
    pencolor = Qt::red;
}

void MainWindow::on_radioGreen_clicked()
{
    pencolor = Qt::green;
}

void MainWindow::on_radioWhite_clicked()
{
    pencolor = Qt::white;
}

void MainWindow::on_radioYellow_clicked()
{
    pencolor = Qt::yellow;
}

void MainWindow::on_radioAqua_clicked()
{
    pencolor = QColor(128, 255, 190);
}

void MainWindow::on_radioFront_clicked()
{
    pencolor = frontcolor;
}

void MainWindow::drawParamOkr(int xc, int yc, int r, bool dr)
{
    int x = 0;
    int y = r;
    double del = 1. / r;
    for (double t = 0; t < M_PI_4; t += del)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);

            setPixel(xc+y, yc+x);
            setPixel(xc-y, yc+x);
            setPixel(xc+y, yc-x);
            setPixel(xc-y, yc-x);
        }
        x = round(r * cos(t));
        y = round(r * sin(t));    }
}

void MainWindow::drawCanonOkr(int xc, int yc, int r, bool dr)
{
    int x = 0;
    int y = r;
    while (x <= r)
    {
        y = round(sqrt(r*r - x*x));
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);

            setPixel(xc+y, yc+x);
            setPixel(xc-y, yc+x);
            setPixel(xc+y, yc-x);
            setPixel(xc-y, yc-x);
        }
        x++;
    }
}

void MainWindow::drawBrezenheimOkr(int xc, int yc, int r, bool dr)
{
    int x = 0;
    int y = r;
    int di = 2 * (1 - r);
    int rd2 = round(r / sqrt(2));
    while (x <= rd2 && y >= rd2)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);

            setPixel(xc+y, yc+x);
            setPixel(xc-y, yc+x);
            setPixel(xc+y, yc-x);
            setPixel(xc-y, yc-x);
        }
        if (di < 0)
        {
            int d1 = 2 * di + 2 * y - 1;
            if (d1 < 0)
            {
                x++;
                di += 2 * x + 1;
            }
            else
            {
                x++;
                y--;
                di += 2 * (x - y + 1);
            }
        }
        else if (di == 0)
        {
            x++;
            y--;
            di +=2 * (x - y + 1);
        }
        else
        {
            int d2 = 2 * di - 2 * x - 1;
            if (d2 < 0)
            {
                x++;
                y--;
                di +=2 * (x - y + 1);
            }
            else
            {
                y--;
                di -= 2 * y - 1;
            }
        }
    }
}

void MainWindow::drawSrDotOkr(int xc, int yc, int r, bool dr)
{
    int r2 = r * r;
    int x = 0;
    int y = r;
    int rd2 = round(r / sqrt(2));
    int f = round(r2 - r2 * y + r2 * 0.25 + 0.5);
    int df = 0;
    int delt = -2 * r2 * y;

    while(x <= rd2)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);

            setPixel(xc+y, yc+x);
            setPixel(xc-y, yc+x);
            setPixel(xc+y, yc-x);
            setPixel(xc-y, yc-x);
        }
        x++;
        if (f >= 0)
        {
            y--;
            delt += 2 * r2;
            f += delt;
        }
        df += 2 * r2;
        f += df + r2;
    }
}


void MainWindow::drawParamEll(int xc, int yc, int a, int b, bool dr)
{
    int x = 0;
    int y = b;
    int max = a > b ? a : b;
    double del = 1. / max;
    for (double t = 0; t < M_PI_2; t += del)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);
        }
        x = round(a * cos(t));
        y = round(b * sin(t));
    }
}

void MainWindow::drawCanonEll(int xc, int yc, int a, int b, bool dr)
{
    int x = 0;
    int y = 0;
    while (x <= a)
    {
        y = round(b * sqrt(1 - x * x / double(a) / double(a)));
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);
        }
        x++;
    }
    y = 0;
    while (y <= b)
    {
        x = round(a * sqrt(1 - y * y / double(b) / double(b)));
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);
        }
        y++;
    }
}

void MainWindow::drawBrezenheimEll(int xc, int yc, int a, int b, bool dr)
{
    int x = 0;
    int y = b;
    int a2 = a * a;
    int b2 = b * b;
    int di = a2 + b2 - 2 * a2 * y;
    while (y >= 0)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);
        }
        if (di < 0)
        {
            int d1 = 2 * di + 2 * a2 * y - 1;
            if (d1 < 0)
            {
                x++;
                di += 2 * b2 * x + 1;
            }
            else
            {
                x++;
                y--;
                di += 2 * b2 * x + b2 + a2 - 2 * a2 * y;
            }
        }
        else if (di == 0)
        {
            x++;
            y--;
            di += 2 * b2 * x + b2 + a2 - 2 * a2 * y;
        }
        else
        {
            int d2 = 2 * di - 2 * b2 * x - 1;
            if (d2 <= 0)
            {
                x++;
                y--;
                di += 2 * b2 * x + b2 + a2 - 2 * a2 * y;
            }
            else
            {
                y--;
                di -= 2 * a2 * y - a2;
            }
        }
    }
}

void MainWindow::drawSrDotEll(int xc, int yc, int a, int b, bool dr)
{
    int x = 0;
    int y = b;
    int a2 = a * a;
    int b2 = b * b;
    int rd2 = round(a2 / sqrt(a2 + b2));
    int f = round(b2 - a2 * y + a2 * 0.25 + 0.5);
    int df = 0;
    int delt = -2 * a2 * y;

    while(x <= rd2)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);
        }
        x++;
        if (f >= 0)
        {
            y--;
            delt += 2 * a2;
            f += delt;
        }
        df += 2 * b2;
        f += df + b2;
    }
    delt = 2 * b2 * x;
    f += round(-b2 * (x + 0.75) - a2 * (y - 0.75));
    df = -2 * a2 * y;
    while(y >= 0)
    {
        if (dr)
        {
            setPixel(xc+x, yc+y);
            setPixel(xc-x, yc+y);
            setPixel(xc+x, yc-y);
            setPixel(xc-x, yc-y);
        }
        y--;
        if (f < 0)
        {
            x++;
            delt += 2 * b2;
            f += delt;
        }
        df += 2 * a2;
        f += df + a2;
    }
}

void MainWindow::on_buttonClear_clicked()
{
    ui->graphicsView->scene()->clear();
}

void MainWindow::drawSpectorOkr(int xc, int yc, int n, int r, bool fl)
{
    int step = (r - 10) / n;
    if (ui->radioParam->isChecked())
        for (int i = 10 + step; i <= r; i += step)
            drawParamOkr(xc, yc, i, fl);
    if (ui->radioKanon->isChecked())
        for (int i = 10 + step; i <= r; i += step)
            drawCanonOkr(xc, yc, i, fl);
    if (ui->radioBrez->isChecked())
        for (int i = 10 + step; i <= r; i += step)
            drawBrezenheimOkr(xc, yc, i, fl);
    if (ui->radioSrDot->isChecked())
        for (int i = 10 + step; i <= r; i += step)
            drawSrDotOkr(xc, yc, i, fl);
    if (ui->radioBiblio->isChecked())
        for (int i = 10 + step; i <= r; i += step)
            ui->graphicsView->scene()->addEllipse(QRect(xc - i, yc - i, 2 * i, 2 * i), QPen(pencolor));
}

void MainWindow::drawSpectorEll(int xc, int yc, int n, int a, int b, bool fl)
{
    double ia = 10;
    double ib = double(b) / a * 10;
    double stepa = double(a - 10) / n;
    double stepb = (b - ib) / n;
    if (ui->radioParam->isChecked())
        for (int i = 0; i < n; i++)
        {
            ia += stepa;
            ib += stepb;
            drawParamEll(xc, yc, ia, ib, fl);
        }
    ia = 10;
    ib = double(b) / a * 10;
    if (ui->radioKanon->isChecked())
        for (int i = 0; i < n; i++)
        {
            ia += stepa;
            ib += stepb;
            drawCanonEll(xc, yc, ia, ib, fl);
        }
    ia = 10;
    ib = double(b) / a * 10;
    if (ui->radioBrez->isChecked())
        for (int i = 0; i < n; i++)
        {
            ia += stepa;
            ib += stepb;
            drawBrezenheimEll(xc, yc, ia, ib, fl);
        }
    ia = 10;
    ib = double(b) / a * 10;
    if (ui->radioSrDot->isChecked())
        for (int i = 0; i < n; i++)
        {
            ia += stepa;
            ib += stepb;
            drawSrDotEll(xc, yc, ia, ib, fl);
        }
    ia = 10;
    ib = double(b) / a * 10;
    if (ui->radioBiblio->isChecked())
        for (int i = 0; i < n; i++)
        {
            ia += stepa;
            ib += stepb;
            ui->graphicsView->scene()->addEllipse(QRect(xc - ia, yc - ib, 2 * ia, 2 * ib), QPen(pencolor));
        }
}

void MainWindow::on_buttonDrawSpec_clicked()
{
    int n = ui->spinBoxN->value();
    int xc = ui->spinCenterX->value();
    int yc = ui->spinCenterY->value();
    if (ui->radioR->isChecked())
    {
        int r = ui->spinBoxR->value();
        drawSpectorOkr(xc, yc, n, r, true);
    }
    if (ui->radioAB->isChecked())
    {
        int a = ui->spinBoxA->value();
        int b = ui->spinBoxB->value();
        drawSpectorEll(xc, yc, n, a, b, true);
    }
}

