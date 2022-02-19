#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stack>
#include <math.h>
#include <iostream>
#include <QPolygon>
#define _USE_MATH_DEFINES
#include <cmath>
#include <QGraphicsItem>

using namespace std;
stack <BirdDots> steck;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    drawStartBird();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPixel(double x, double y)
{
   //ui->graphicsView->scene()->addEllipse(x, y, 1, 1);

   QPolygon polygon;
   polygon << QPoint(x, y) << QPoint(x, y + 1) << QPoint(x + 1, y);
   ui->graphicsView->scene()->addPolygon(polygon);
}

void MainWindow::drawAxis()
{
    drawLine(0, 0, 20, 0);
    drawLine(20, 0, 15, -3);
    drawLine(20, 0, 15, 3);

    drawLine(0, 0, 0, 20);
    drawLine(0, 20, -3, 15);
    drawLine(0, 20, 3, 15);

    QGraphicsTextItem *num = new QGraphicsTextItem;
    num->setPos(-20, -20);
    num->setPlainText(QString("[0:0]"));
    ui->graphicsView->scene()->addItem(num);
}

void MainWindow::drawEllipse(double xc, double yc, double a, double b, double ugl)
{
    ugl = ugl / 360 * 2 * M_PI;
    double mr = a > b ? a : b;
    double fi = fabs(1 / mr);
    for (double chert = 0; chert < 2 * M_PI; chert = chert + fi)
    {
        double r = a * b / sqrt(pow(a * sin(chert), 2) + pow(b * cos(chert), 2)) / 2;
        double slx = r * cos( - ugl + chert), sly = r * sin( - ugl + chert);
        double new_x = xc + slx, new_y = yc + sly;
        setPixel(floor(new_x), floor(new_y));
    }
}

void swap(double *x1, double *y1, double *x2, double *y2)
{
    *x1 = *x1 + *x2;
    *x2 = *x1 - *x2;
    *x1 = *x1 - *x2;
    *y1 = *y1 + *y2;
    *y2 = *y1 - *y2;
    *y1 = *y1 - *y2;
}

void MainWindow::drawLine(double x1, double y1, double x2, double y2)
{
    int dx = roundf(fabs(x2 - x1));
    int dy = roundf(fabs(y2 - y1));
    if (dx > dy)
    {
        if (x1 > x2)
            swap(&x1, &y1, &x2, &y2);
        double y = y1;
        double err = 0, derr = double(dy + 1) / double(dx + 1);
        int diry = y2 - y1; diry = diry > 0 ? 1 : -1;
        for (; x1 < x2; x1++)
        {
            setPixel(x1, y);
            err += derr;
            while (err >= 1)
            {
                y += diry;
                err -= 1;
            }
        }
    }
    else
    {
        if (y1 > y2)
            swap(&x1, &y1, &x2, &y2);
        double x = x1;
        double err = 0, derr = double(dx + 1) / double(dy + 1);
        int dirx = x2 - x1; dirx = dirx > 0 ? 1 : -1;
        for (; y1 < y2; y1++)
        {
            setPixel(x, y1);
            err += derr;
            while (err >= 1)
            {
                x += dirx;
                err -= 1;
            }
        }
    }
}

void MainWindow::drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3)
{
    drawLine(x1, y1, x2, y2);
    drawLine(x3, y3, x2, y2);
    drawLine(x1, y1, x3, y3);
}

void MainWindow::drawBird(BirdDots bird)
{
    drawEllipse(bird.bodyc_x, bird.bodyc_y, bird.body_a, bird.body_b, bird.body_ug);
    drawEllipse(bird.headc_x, bird.headc_y, bird.head_a, bird.head_b, bird.body_ug);

    drawTriangle(bird.beak1_x, bird.beak1_y, bird.beak2_x, bird.beak2_y, bird.beak3_x, bird.beak3_y);
    drawTriangle(bird.tail1_x, bird.tail1_y, bird.tail2_x, bird.tail2_y, bird.tail3_x, bird.tail3_y);
    drawTriangle(bird.wing1_x, bird.wing1_y, bird.wing2_x, bird.wing2_y, bird.wing3_x, bird.wing3_y);

    drawLine(bird.legl3_x, bird.legl3_y, bird.legl4_x, bird.legl4_y);
    drawLine(bird.legr1_x, bird.legr1_y, bird.legr2_x, bird.legr2_y);
}

void MainWindow::drawStartBird()
{
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->scene()->clear();
    drawAxis();

    BirdDots *bird = new BirdDots;

    bird->headc_x = 102;
    bird->headc_y = 48 + 18;
    bird->head_a = 26 * 2;
    bird->head_b = 26 * 2;

    bird->beak1_x = 32;
    bird->beak1_y = 50 + 15 + 3;
    bird->beak2_x = 90;
    bird->beak2_y = 50 + 15 + 3;
    bird->beak3_x = 90;
    bird->beak3_y = 28 + 15 + 3;

    bird->body_ug = 0;
    bird->bodyc_x = 184;
    bird->bodyc_y = 137;
    bird->body_a = 114 * 2;
    bird->body_b = 60 * 2;

    bird->wing1_x = 181;
    bird->wing1_y = 144;
    bird->wing2_x = 240;
    bird->wing2_y = 144;
    bird->wing3_x = 280;
    bird->wing3_y = 222;

    bird->tail1_x = 273;
    bird->tail1_y = 106;
    bird->tail2_x = 355;
    bird->tail2_y = 104;
    bird->tail3_x = 282;
    bird->tail3_y = 142;

    bird->legl3_x = 113;
    bird->legl3_y = 188 - 5;
    bird->legl4_x = 64;
    bird->legl4_y = 243;

    bird->legr1_x = 162;
    bird->legr1_y = 196;
    bird->legr2_x = 185;
    bird->legr2_y = 262;

    steck.push(*bird);
    ui->graphicsView->scene()->clear();
    drawAxis();
    drawBird(steck.top());
}

void MainWindow::transferBird(BirdDots bird, double x, double y)
{
    BirdDots *new_bird = new BirdDots;

    new_bird->headc_x = bird.headc_x + x;
    new_bird->headc_y = bird.headc_y + y;

    new_bird->beak1_x = bird.beak1_x + x;
    new_bird->beak1_y = bird.beak1_y + y;
    new_bird->beak2_x = bird.beak2_x + x;
    new_bird->beak2_y = bird.beak2_y + y;
    new_bird->beak3_x = bird.beak3_x + x;
    new_bird->beak3_y = bird.beak3_y + y;


    new_bird->bodyc_x = bird.bodyc_x + x;
    new_bird->bodyc_y = bird.bodyc_y + y;
    new_bird->body_ug = bird.body_ug;

    new_bird->wing1_x = bird.wing1_x + x;
    new_bird->wing1_y = bird.wing1_y + y;
    new_bird->wing2_x = bird.wing2_x + x;
    new_bird->wing2_y = bird.wing2_y + y;
    new_bird->wing3_x = bird.wing3_x + x;
    new_bird->wing3_y = bird.wing3_y + y;

    new_bird->tail1_x = bird.tail1_x + x;
    new_bird->tail1_y = bird.tail1_y + y;
    new_bird->tail2_x = bird.tail2_x + x;
    new_bird->tail2_y = bird.tail2_y + y;
    new_bird->tail3_x = bird.tail3_x + x;
    new_bird->tail3_y = bird.tail3_y + y;

    new_bird->legl3_x = bird.legl3_x + x;
    new_bird->legl3_y = bird.legl3_y + y;
    new_bird->legl4_x = bird.legl4_x + x;
    new_bird->legl4_y = bird.legl4_y + y;

    new_bird->legr1_x = bird.legr1_x + x;
    new_bird->legr1_y = bird.legr1_y + y;
    new_bird->legr2_x = bird.legr2_x + x;
    new_bird->legr2_y = bird.legr2_y + y;

    steck.push(*new_bird);
    ui->graphicsView->scene()->clear();
    drawAxis();
    drawBird(steck.top());

}

double scaleDot(double x, double xm, double kx)
{
    return kx * x + ( 1 - kx) * xm;
}

void MainWindow::scaleBird(BirdDots bird, double xm, double ym, double kx, double ky)
{
    BirdDots *new_bird = new BirdDots;
    double ang = bird.body_ug / 360 * 2 * M_PI;
    new_bird->headc_x = scaleDot(bird.headc_x, xm, kx);
    new_bird->headc_y = scaleDot(bird.headc_y, ym, ky);
    double xa = bird.headc_x + bird.head_a * cos(ang) / 2;
    double ya = bird.headc_y + bird.head_a * sin(ang) / 2;
    double xb = bird.headc_x - bird.head_b * sin(ang) / 2;
    double yb = bird.headc_y + bird.head_b * cos(ang) / 2;
    xa = scaleDot(xa, xm, kx);
    ya = scaleDot(ya, ym, ky);
    xb = scaleDot(xb, xm, kx);
    yb = scaleDot(yb, ym, ky);
    new_bird->head_a = sqrt(pow(xa - new_bird->headc_x, 2) + pow(ya - new_bird->headc_y, 2)) * 2;
    new_bird->head_b = sqrt(pow(xb - new_bird->headc_x, 2) + pow(yb - new_bird->headc_y, 2)) * 2;
    //new_bird->head_a = bird.head_a * kx;
    //new_bird->head_b = bird.head_b * ky;


    new_bird->beak1_x = scaleDot(bird.beak1_x, xm, kx);
    new_bird->beak1_y = scaleDot(bird.beak1_y, ym, ky);
    new_bird->beak2_x = scaleDot(bird.beak2_x, xm, kx);
    new_bird->beak2_y = scaleDot(bird.beak2_y, ym, ky);
    new_bird->beak3_x = scaleDot(bird.beak3_x, xm, kx);
    new_bird->beak3_y = scaleDot(bird.beak3_y, ym, ky);


    new_bird->bodyc_x = scaleDot(bird.bodyc_x, xm, kx);
    new_bird->bodyc_y = scaleDot(bird.bodyc_y, ym, ky);
    //new_bird->body2_x = scaleDot(bird.body2_x, xm, kx);
    //new_bird->body2_y = scaleDot(bird.body2_y, ym, ky);

    xa = bird.bodyc_x + bird.body_a * cos(ang) / 2;
    ya = bird.bodyc_y + bird.body_a * sin(ang) / 2;
    xb = bird.bodyc_x - bird.body_b * sin(ang) / 2;
    yb = bird.bodyc_y + bird.body_b * cos(ang) / 2;
    xa = scaleDot(xa, xm, kx);
    ya = scaleDot(ya, ym, ky);
    xb = scaleDot(xb, xm, kx);
    yb = scaleDot(yb, ym, ky);
    new_bird->body_a = sqrt(pow(xa - new_bird->bodyc_x, 2) + pow(ya - new_bird->bodyc_y, 2)) * 2;
    new_bird->body_b = sqrt(pow(xb - new_bird->bodyc_x, 2) + pow(yb - new_bird->bodyc_y, 2)) * 2;
    //new_bird->body_a = bird.body_a * kx;
    //new_bird->body_b = bird.body_b * ky;
    new_bird->body_ug = asin((ya - new_bird->bodyc_y) / new_bird->body_a * 2) / 2 / M_PI * 360;

    new_bird->wing1_x = scaleDot(bird.wing1_x, xm, kx);
    new_bird->wing1_y = scaleDot(bird.wing1_y, ym, ky);
    new_bird->wing2_x = scaleDot(bird.wing2_x, xm, kx);
    new_bird->wing2_y = scaleDot(bird.wing2_y, ym, ky);
    new_bird->wing3_x = scaleDot(bird.wing3_x, xm, kx);
    new_bird->wing3_y = scaleDot(bird.wing3_y, ym, ky);

    new_bird->tail1_x = scaleDot(bird.tail1_x, xm, kx);
    new_bird->tail1_y = scaleDot(bird.tail1_y, ym, ky);
    new_bird->tail2_x = scaleDot(bird.tail2_x, xm, kx);
    new_bird->tail2_y = scaleDot(bird.tail2_y, ym, ky);
    new_bird->tail3_x = scaleDot(bird.tail3_x, xm, kx);
    new_bird->tail3_y = scaleDot(bird.tail3_y, ym, ky);

    new_bird->legl3_x = scaleDot(bird.legl3_x, xm, kx);
    new_bird->legl3_y = scaleDot(bird.legl3_y, ym, ky);
    new_bird->legl4_x = scaleDot(bird.legl4_x, xm, kx);
    new_bird->legl4_y = scaleDot(bird.legl4_y, ym, ky);

    new_bird->legr1_x = scaleDot(bird.legr1_x, xm, kx);
    new_bird->legr1_y = scaleDot(bird.legr1_y, ym, ky);
    new_bird->legr2_x = scaleDot(bird.legr2_x, xm, kx);
    new_bird->legr2_y = scaleDot(bird.legr2_y, ym, ky);

    steck.push(*new_bird);
    ui->graphicsView->scene()->clear();
    drawAxis();
    drawBird(steck.top());
}

double rotateDotX(double x, double y, double xc, double yc, double ugl)
{
    ugl = ugl / 360 * 2 * M_PI;
    return xc + (x - xc) * cos(ugl) + (y - yc) * sin(ugl);
}

double rotateDotY(double x, double y, double xc, double yc, double ugl)
{
    ugl = ugl / 360 * 2 * M_PI;
    return yc + (y - yc) * cos(ugl) - (x - xc) * sin(ugl);
}

void MainWindow::rotateBird(BirdDots bird, double x, double y, double ugl)
{
    BirdDots *new_bird = new BirdDots;

    new_bird->headc_x = rotateDotX(bird.headc_x, bird.headc_y, x, y, ugl);
    new_bird->headc_y = rotateDotY(bird.headc_x, bird.headc_y, x, y, ugl);
    new_bird->head_a = bird.head_a;
    new_bird->head_b = bird.head_b;

    new_bird->beak1_x = rotateDotX(bird.beak1_x, bird.beak1_y, x, y, ugl);
    new_bird->beak1_y = rotateDotY(bird.beak1_x, bird.beak1_y, x, y, ugl);
    new_bird->beak2_x = rotateDotX(bird.beak2_x, bird.beak2_y, x, y, ugl);
    new_bird->beak2_y = rotateDotY(bird.beak2_x, bird.beak2_y, x, y, ugl);
    new_bird->beak3_x = rotateDotX(bird.beak3_x, bird.beak3_y, x, y, ugl);
    new_bird->beak3_y = rotateDotY(bird.beak3_x, bird.beak3_y, x, y, ugl);

    new_bird->bodyc_x = rotateDotX(bird.bodyc_x, bird.bodyc_y, x, y, ugl);
    new_bird->bodyc_y = rotateDotY(bird.bodyc_x, bird.bodyc_y, x, y, ugl);
    new_bird->body_a = bird.body_a;
    new_bird->body_b = bird.body_b;
    new_bird->body_ug = bird.body_ug + ugl;

    new_bird->wing1_x = rotateDotX(bird.wing1_x, bird.wing1_y, x, y, ugl);
    new_bird->wing1_y = rotateDotY(bird.wing1_x, bird.wing1_y, x, y, ugl);
    new_bird->wing2_x = rotateDotX(bird.wing2_x, bird.wing2_y, x, y, ugl);
    new_bird->wing2_y = rotateDotY(bird.wing2_x, bird.wing2_y, x, y, ugl);
    new_bird->wing3_x = rotateDotX(bird.wing3_x, bird.wing3_y, x, y, ugl);
    new_bird->wing3_y = rotateDotY(bird.wing3_x, bird.wing3_y, x, y, ugl);

    new_bird->tail1_x = rotateDotX(bird.tail1_x, bird.tail1_y, x, y, ugl);
    new_bird->tail1_y = rotateDotY(bird.tail1_x, bird.tail1_y, x, y, ugl);
    new_bird->tail2_x = rotateDotX(bird.tail2_x, bird.tail2_y, x, y, ugl);
    new_bird->tail2_y = rotateDotY(bird.tail2_x, bird.tail2_y, x, y, ugl);
    new_bird->tail3_x = rotateDotX(bird.tail3_x, bird.tail3_y, x, y, ugl);
    new_bird->tail3_y = rotateDotY(bird.tail3_x, bird.tail3_y, x, y, ugl);

    new_bird->legl3_x = rotateDotX(bird.legl3_x, bird.legl3_y, x, y, ugl);
    new_bird->legl3_y = rotateDotY(bird.legl3_x, bird.legl3_y, x, y, ugl);
    new_bird->legl4_x = rotateDotX(bird.legl4_x, bird.legl4_y, x, y, ugl);
    new_bird->legl4_y = rotateDotY(bird.legl4_x, bird.legl4_y, x, y, ugl);

    new_bird->legr1_x = rotateDotX(bird.legr1_x, bird.legr1_y, x, y, ugl);
    new_bird->legr1_y = rotateDotY(bird.legr1_x, bird.legr1_y, x, y, ugl);
    new_bird->legr2_x = rotateDotX(bird.legr2_x, bird.legr2_y, x, y, ugl);
    new_bird->legr2_y = rotateDotY(bird.legr2_x, bird.legr2_y, x, y, ugl);

    steck.push(*new_bird);
    ui->graphicsView->scene()->clear();
    drawAxis();
    drawBird(steck.top());
}

void MainWindow::on_pushButton_clicked()
{
    drawStartBird();
}


void MainWindow::on_transferButton_clicked()
{
    bool ok1, ok2, ok3, ok4, ok = true;
    if (ui->checkBoxSdv->isChecked())
    {
        double dx = ui->editSdvX->text().toFloat(&ok1);
        if (ok1)
        {
            double dy = ui->editSdvY->text().toFloat(&ok2);
            if (ok2)
                transferBird(steck.top(), dx, dy);
        }
        ok = ok1 && ok2;
        if (!ok)
            ui->labelMessage->setText(QString("Данные должны представлять собой\nвещественные десятичные числа!"));

    }
    if (ok && ui->checkBoxMash->isChecked())
    {
        double xm = ui->editMashX->text().toFloat(&ok1);
        double ym = ui->editMashY->text().toFloat(&ok2);
        double kx = ui->editMashKoefX->text().toFloat(&ok3);
        double ky = ui->editMashKoefY->text().toFloat(&ok4);
        ok = ok1 && ok2 && ok3 && ok4;
        if (ok)
            scaleBird(steck.top(), xm, ym, kx, ky);
        if (!ok)
            ui->labelMessage->setText(QString("Данные должны представлять собой\nвещественные десятичные числа!"));
    }
    if (ok && ui->checkBoxPov->isChecked())
    {
        double xm = ui->editPovx->text().toFloat(&ok1);
        double ym = ui->editPovY->text().toFloat(&ok2);
        double ugl = ui->editPovUg->text().toFloat(&ok3);
        ok = ok1 && ok2 && ok3;
        if (ok)
            rotateBird(steck.top(), xm, ym, ugl);
        if (!ok)
            ui->labelMessage->setText(QString("Данные должны представлять собой\nвещественные десятичные числа!"));
    }
    if (ok)
        ui->labelMessage->setText(QString(" "));

}


void MainWindow::on_dackButton_clicked()
{
    steck.pop();
    ui->graphicsView->scene()->clear();
    drawAxis();
    if (steck.empty())
        drawStartBird();
    else
        drawBird(steck.top());
}

