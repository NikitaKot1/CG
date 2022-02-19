#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPolygon>

#define RPix 5
static double *dots_x = (double *)calloc(3, sizeof(double));
static double *dots_y = (double *)calloc(3, sizeof(double));
static int col = 3;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void centre(double *x, double *y, double x1, double y1, double x2, double y2, double x3, double y3)
{
    double z1 = x1 * x1 + y1 * y1;
    double z2 = x2 * x2 + y2 * y2;
    double z3 = x3 * x3 + y3 * y3;
    double zx = (y1 - y2) * z3 + (y2 - y3) * z1 + (y3 - y1) * z2;
    double zy = (x1 - x2) * z3 + (x2 - x3) * z1 + (x3 - x1) * z2;
    double z = (x1 - x2) * (y3 - y1) - (y1 - y2) * (x3 - x1);
    *x = - zx / 2 / z;
    *y = zy / 2 / z;
}

double len_vec(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double S_tr(double a, double b, double c)
{
    double p = (a + b+ c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

double S_okr(double a, double b, double c)
{
    double r = a * b * c / 4 / S_tr(a, b, c);
    return M_PI * r * r;
}

int MainWindow::SayNigga()
{
    bool ok;
    col = ui->numOfDots->text().toInt(&ok, 10);
    return !ok;
}

void MainWindow::on_pushButton_clicked()
{
    if (SayNigga())
        ui->label_2->setText(QString("Количество введенных точек должно быть натуральным числом"));
    else if (col <= 2)
        ui->label_2->setText(QString("Количество введенных точек должно быть не меньше 3"));
    else
    {
        ui->label_2->setText(QString(" "));
        ui->tableWidget->setRowCount(col + 1);
        if (col < 7)
            ui->tableWidget->setFixedHeight((col + 1) * 75 / 2);
        else
            ui->tableWidget->setFixedHeight(225);
    }
    if (dots_x)
        free(dots_x);
    dots_x = (double *)calloc(col, sizeof(double));
    if (dots_y)
        free(dots_y);
    dots_y = (double *)calloc(col, sizeof(double));
    update();
}

int MainWindow::getDotsParams()
{
    bool ok = true; int net = 0, i = 0;
    while (i + net < col && ok)
    //for (int i = 0; i < col && ok; i++)
    {
        dots_x[i] = ui->tableWidget->item(i + net + 1, 0)->text().toFloat(&ok);
        if (ok)
        {
            dots_y[i] = ui->tableWidget->item(i + net + 1, 1)->text().toFloat(&ok);
            if (ok)
            {
                for (int j = 0; j < i; j++)
                {
                    if (fabs(dots_x[j] - dots_x[i]) < 1e-6 && fabs(dots_y[j] - dots_y[i]) < 1e-6)
                    {
                        i--;
                        net++;
                        break;
                    }
                }
            }
        }
        i++;
    }
    if (!ok)
        ui->label_2->setText(QString("Координаты введенных точек должны быть представлены действительными числами"));
    col -= net;
    if (col < 3)
    {
        ui->label_2->setText(QString("Количество введенных точек должно быть не меньше 3"));
        ok = false;
    }
    return !ok;
}

void MainWindow::drawDot(double x, double y, int i)
{
    ui->graphicsView->scene()->addEllipse(QRectF(x - RPix, y - RPix, 2 * RPix, 2 * RPix), QPen(Qt::red), QBrush(Qt::red));
    QGraphicsTextItem *num = new QGraphicsTextItem;
    num->setPos(x + RPix, y - RPix);
    char txt[1024] = " 1 {1.11, 1.11}";
    sprintf(txt, "%d {%.2f, %.2f}", i, dots_x[i - 1], dots_y[i - 1]);
    num->setPlainText(QString(txt));

    ui->graphicsView->scene()->addItem(num);
}

void MainWindow::on_drawButton_clicked()
{
    ui->graphicsView->setScene(&scene);
    ui->graphicsView->scene()->clear();
    ui->label_otv->setText(QString(" "));
    ui->label_2->setText(QString(" "));
    int wid = 440; //ui->graphicsView->width() - RPix * 2;
    int hei = 440; //ui->graphicsView->height() - RPix * 2;
    if (!getDotsParams())
    {
        double strin = 0, sokru = 0, razn = 0;
        double strin_ob = 0, sokru_ob = 0, razn_ob = -1;
        int d1, d2, d3;
        for (int i = 0; i < col; i++)
            for (int j = i + 1; j < col; j++)
                for (int k = j + 1; k < col; k++)
                {
                    double a = len_vec(dots_x[i], dots_y[i], dots_x[j], dots_y[j]);
                    double b = len_vec(dots_x[i], dots_y[i], dots_x[k], dots_y[k]);
                    double c = len_vec(dots_x[k], dots_y[k], dots_x[j], dots_y[j]);
                    strin = S_tr(a, b, c);
                    if (strin > 1e-12)
                    {
                        sokru = S_okr(a, b, c);
                        razn = sokru - strin;
                        if (razn_ob < 0 || (razn < razn_ob))
                        {
                            strin_ob = strin;
                            sokru_ob = sokru;
                            razn_ob = razn;
                            d1 = i; d2 = j; d3 = k;
                        }
                    }
                }
        if (fabs(razn_ob + 1) < 1e-12)
            ui->label_2->setText(QString("Введенные точки образуют только вырожденные треугольники"));
        else
        {
            double a = len_vec(dots_x[d1], dots_y[d1], dots_x[d2], dots_y[d2]);
            double b = len_vec(dots_x[d1], dots_y[d1], dots_x[d3], dots_y[d3]);
            double c = len_vec(dots_x[d3], dots_y[d3], dots_x[d2], dots_y[d2]);
            double r = a * b * c / 4 / S_tr(a, b, c);
            double lin = hei / r / 2;


            double cent_x, cent_y;
            centre(&cent_x, &cent_y, dots_x[d1], dots_y[d1], dots_x[d2], dots_y[d2], dots_x[d3], dots_y[d3]);

            ui->graphicsView->scene()->addEllipse(QRectF(RPix, -RPix, wid, hei), QPen(Qt::green), QBrush(Qt::green));

            QGraphicsTextItem *num = new QGraphicsTextItem;
            num->setPos(wid / 2 + RPix, hei / 2 - RPix);
            char txt[1024] = "центр {1.11, 1.11}";
            sprintf(txt, "центр {%.2f, %.2f}", cent_x, cent_y);
            num->setPlainText(QString(txt));

            double minx = cent_x - r, maxx = cent_x + r, miny = cent_y - r, maxy = cent_y + r;
            double xpix1 = (dots_x[d1] - minx) * lin + RPix, ypix1 =  hei - (dots_y[d1] - miny) * lin - RPix;
            double xpix2 = (dots_x[d2] - minx) * lin + RPix, ypix2 =  hei - (dots_y[d2] - miny) * lin - RPix;
            double xpix3 = (dots_x[d3] - minx) * lin + RPix, ypix3 =  hei - (dots_y[d3] - miny) * lin - RPix;
            QPolygon polygon;
            polygon << QPoint(xpix1, ypix1) << QPoint(xpix2, ypix2) << QPoint(xpix3, ypix3);
            ui->graphicsView->scene()->addPolygon(polygon, QPen(Qt::white), QBrush(Qt::gray));

            ui->graphicsView->scene()->addEllipse(QRectF(wid / 2 + RPix, hei / 2 - RPix, 2 * RPix, 2 * RPix), QPen(Qt::blue), QBrush(Qt::blue));

            for (int i = 0; i < col; i++) {
                if (dots_x[i] >= minx && dots_x[i] < maxx && dots_y[i] >= miny && dots_y[i] < maxy)
                    drawDot((dots_x[i] - minx) * lin + RPix, hei - (dots_y[i] - miny) * lin - RPix, i + 1);
            }

            ui->graphicsView->scene()->addItem(num);

            char otv[1024];
            sprintf(otv, "Треугольник построен на точках:\n%d - %.3lf:%.3lf\n%d - %.3lf:%.3lf\n%d - %.3lf:%.3lf\nПлощадь треугольника = %.3lf\nПлощадь окружности = %.3lf\nРазность площадей = %.3lf",\
                    d1 + 1, dots_x[d1], dots_y[d1], d2 + 1, dots_x[d2], dots_y[d2], d3 + 1, dots_x[d3], dots_y[d3], strin_ob, sokru_ob, razn_ob);
            ui->label_otv->setText(QString(otv));
        }
    }

    update();
}
