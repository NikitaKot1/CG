#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "struct.h"
#include <QMouseEvent>
#include <QTableWidgetItem>
#include <chrono>
#include <thread>
#include <cmath>

vector <figure> lol;
int iter = 0;
bool flagEnter = false;
QColor pencolor = Qt::white;
QColor shadecolor = Qt::white;
bool flg = true;
int ymin = 2000;
int ymax = 0;
active_sects acts;
active_sects2 sects_act;
active_sects2 now_active;

#define RED 1
#define BLUE 2
#define GREEN 3
#define WHITE 4
#define YELLOW 5
#define BACK 6

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene.setSceneRect(0, 0, 850, 900);
    ui->graphicsView->setScene(&scene);
    vector <dot> v0;
    figure fig;
    fig.body = v0;
    fig.size = 0;
    lol.push_back(fig);

    addColorsToComboBox(ui->comboBox);
    addColorsToComboBox(ui->comboBox_2);
    scene.setBackgroundBrush(QBrush(Qt::black));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addColorsToComboBox(QComboBox *cmb)
{
    cmb->addItem(QString("red"), RED);
    cmb->addItem(QString("blue"), BLUE);
    cmb->addItem(QString("green"), GREEN);
    cmb->addItem(QString("white"), WHITE);
    cmb->addItem(QString("yellow"), YELLOW);
    cmb->addItem(QString("фон"), BACK);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (flg)
        colorCheck();
    QPoint remapped = ui->graphicsView->mapFromParent(event->pos());
    if (ui->graphicsView->rect().contains(remapped))
    {
        QPointF mousePoint = ui->graphicsView->mapToScene(remapped);
        dot d;
        d.x = mousePoint.x();
        d.y = mousePoint.y();
        if (flagEnter && lol[iter].size != 0)
        {
            int dx = lol[iter].body[lol[iter].size - 1].x - d.x;
            int dy = lol[iter].body[lol[iter].size - 1].y - d.y;
            if (abs(dx) > abs(dy))
                d.y += dy;
            else
                d.x += dx;
        }
        ui->graphicsView->scene()->addEllipse(QRect(d.x - 5, d.y - 5, 10, 10), QPen(pencolor), QBrush(pencolor));
        if (lol[iter].size != 0)
            ui->graphicsView->scene()->addLine(QLine(d.x, d.y, lol[iter].body[lol[iter].size - 1].x,
                                               lol[iter].body[lol[iter].size - 1].y), QPen(pencolor));
        lol[iter].body.push_back(d);
        lol[iter].size++;
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        char buf[10];
        std::sprintf(buf, "%d", d.x);
        QTableWidgetItem *iten = new QTableWidgetItem(QString(buf));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, iten);
        std::sprintf(buf, "%d", d.y);
        QTableWidgetItem *iten2 = new QTableWidgetItem(QString(buf));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, iten2);
        if (d.y > ymax)
            ymax = d.y;
        if (d.y < ymin)
            ymin = d.y;
        if (lol[iter].size > 1)
        {
            int x1 = lol[iter].body[lol[iter].size - 1].x;
            int x2 = lol[iter].body[lol[iter].size - 2].x;
            int y1 = lol[iter].body[lol[iter].size - 1].y;
            int y2 = lol[iter].body[lol[iter].size - 2].y;
            active_sect s;
            s.start_x = y1 > y2 ? x2 : x1;
            s.start_y = y1 > y2 ? y2 : y1;
            s.ny = abs(y2 - y1);
            s.dx = y1 > y2 ? double(x1 - x2) / s.ny : double(x2 - x1) / s.ny;
            sects_act.act.push_back(s);
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        flagEnter = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control)
        flagEnter = false;
}

void MainWindow::on_buttonCloseFig_clicked()
{
    if (lol[iter].size > 2)
    {
        ui->graphicsView->scene()->addLine(lol[iter].body[0].x, lol[iter].body[0].y,
                lol[iter].body[lol[iter].size - 1].x, lol[iter].body[lol[iter].size - 1].y, QPen(pencolor));

        int x1 = lol[iter].body[lol[iter].size - 1].x;
        int x2 = lol[iter].body[0].x;
        int y1 = lol[iter].body[lol[iter].size - 1].y;
        int y2 = lol[iter].body[0].y;
        active_sect s;
        s.start_x = y1 > y2 ? x2 : x1;
        s.start_y = y1 > y2 ? y2 : y1;
        s.ny = abs(y2 - y1);
        s.dx = y1 > y2 ? double(x1 - x2) / s.ny : double(x2 - x1) / s.ny;
        sects_act.act.push_back(s);

        vector <dot> v0;
        figure fig;
        fig.body = v0;
        fig.size = 0;
        lol.push_back(fig);
        iter++;
    }
    flg = true;
}

void MainWindow::colorCheck()
{
    int tag = ui->comboBox->currentIndex();
    switch (tag + 1) {
    case (RED):
        pencolor = Qt::red;
        break;
    case (BLUE):
        pencolor = Qt::blue;
        break;
    case (GREEN):
        pencolor = Qt::green;
        break;
    case (WHITE):
        pencolor = Qt::white;
        break;
    case (YELLOW):
        pencolor = Qt::yellow;
        break;
    case (BACK):
        pencolor = Qt::black;
        break;
    }

    tag = ui->comboBox_2->currentIndex();
    switch (tag+1) {
    case (RED):
        shadecolor = Qt::red;
        break;
    case (BLUE):
        shadecolor = Qt::blue;
        break;
    case (GREEN):
        shadecolor = Qt::green;
        break;
    case (WHITE):
        shadecolor = Qt::white;
        break;
    case (YELLOW):
        shadecolor = Qt::yellow;
        break;
    case (BACK):
        shadecolor = Qt::black;
        break;
    }
}

void MainWindow::on_butClear_clicked()
{
    ui->graphicsView->scene()->clear();
    flg = true;
    lol.clear();
    vector <dot> v0;
    figure fig;
    fig.body = v0;
    fig.size = 0;
    lol.push_back(fig);
    iter = 0;
    int n = ui->tableWidget->rowCount();
    for (int i = n - 1; i >= 0; i--)
    {
        delete ui->tableWidget->item(i, 0);
        delete ui->tableWidget->item(i, 1);
    }
    ui->tableWidget->setRowCount(0);
    sects_act.act.clear();
    now_active.act.clear();
}

bool is_active(dot d1, dot d2, int y)
{
    return (d1.y > y && d2.y <= y) || (d1.y <= y && d2.y > y);
}

void take_active_sects(int y)
{
    for (int i = 0; i < iter; i++)
    {
        for (int j = 0; j < lol[i].size - 1; j++)
            if (is_active(lol[i].body[j], lol[i].body[j+1], y))
            {
                section s;
                s.d1 = lol[i].body[j];
                s.d2 = lol[i].body[j+1];
                acts.act.push_back(s);
            }
        if (is_active(lol[i].body[0], lol[i].body[lol[i].size-1], y))
        {
            section s;
            s.d1 = lol[i].body[0];
            s.d2 = lol[i].body[lol[i].size-1];
            acts.act.push_back(s);
        }
    }
}

int take_x_of_section(section sect, int y)
{
    int dx = sect.d2.x - sect.d1.x;
    int dy = sect.d2.y - sect.d1.y;
    if (dx == 0 || y == sect.d1.y)
        return sect.d1.x;
    if (y == sect.d2.y)
        return sect.d2.x;
    return sect.d1.x + (y - sect.d1.y + 0.5) * dx / dy;
}

void update_groups(int y)
{
    for (int i = 0; i < (int)sects_act.act.size(); i++)
    {
        if (sects_act.act[i].start_y == y)
            now_active.act.push_back(sects_act.act[i]);
    }
    for (int i = now_active.act.size() - 1; i >= 0; i--)
    {
        now_active.act[i].ny--;
        now_active.act[i].start_x += now_active.act[i].dx;
        if (now_active.act[i].ny < 0)
            now_active.act.erase(now_active.act.begin()+i);
    }
}

void MainWindow::shadeFigure(bool stop)
{
    colorCheck();
//    for (int y = ymin + 1; y < ymax; y++)
//    {
//        vector <int> vx;
//        acts.act.clear();
//        take_active_sects(y);
//        for (int j = 0; j < (int)acts.act.size(); j++)
//        {
//            section sect = acts.act[j];
//            int dy = sect.d2.y - sect.d1.y;
//            if (dy == 0)
//            {
//                vx.push_back(sect.d1.x);
//                vx.push_back(sect.d2.x);
//            }
//            else
//                vx.push_back(take_x_of_section(sect, y));
//        }
//        sort(vx.begin(), vx.end());
//        for (int i = 0; i < (int)vx.size(); i+=2)
//            ui->graphicsView->scene()->addLine(QLine(vx[i] + 1, y, vx[i+1] - 1, y), QPen(shadecolor));
//        if (stop)
//        {
//            std::this_thread::sleep_for(std::chrono::microseconds(10000));
//            qApp->processEvents();
//        }
//        vx.clear();
//    }
    for (int y = ymin; y < ymax; y++)
    {
        vector <int> vx;
        update_groups(y);
        for (int i = 0; i < (int)now_active.act.size(); i++)
        {
            vx.push_back(round(now_active.act[i].start_x));
        }
        sort(vx.begin(), vx.end());
        for (int i = 0; i < (int)vx.size(); i+=2)
            ui->graphicsView->scene()->addLine(QLine(vx[i] + 1, y + 1, vx[i+1] - 1, y + 1), QPen(shadecolor));
        if (stop)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10000));
            qApp->processEvents();
        }
        vx.clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    shadeFigure(false);
}


void MainWindow::on_pushButton_2_clicked()
{
    shadeFigure(true);
}

