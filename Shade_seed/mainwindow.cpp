#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <stack>
#include <thread>
#include <chrono>

#define RED 1
#define BLUE 2
#define GREEN 3
#define WHITE 4
#define YELLOW 5
#define BACK 6

bool flagEnter = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    image = QImage(900, 900, QImage::Format_RGB32);
    scene.setSceneRect(0, 0, 900, 900);
    scene.addPixmap(QPixmap::fromImage(image));
    ui->graphicsView->setScene(&scene);
    addColorsToComboBox(ui->comboBox);
    addColorsToComboBox(ui->comboBox_2);
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

void MainWindow::appendDot(int x, int y)
{
    painter = new QPainter(&image);
    painter->setPen(pencolor);
    if (d0.x() == -1 || d0.y() == -1)
    {
        painter->drawPoint(x, y);
        dstart = QPoint(x, y);
    }
    else
        painter->drawLine(d0.x(), d0.y(), x, y);
    d0 = QPoint(x, y);
    scene.addPixmap(QPixmap::fromImage(image));
    delete painter;

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    char buf[10];
    std::sprintf(buf, "%d", x);
    QTableWidgetItem *iten = new QTableWidgetItem(QString(buf));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, iten);
    std::sprintf(buf, "%d", y);
    QTableWidgetItem *iten2 = new QTableWidgetItem(QString(buf));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, iten2);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    colorCheck();
    QPoint remapped = ui->graphicsView->mapFromParent(event->pos());
    if (ui->graphicsView->rect().contains(remapped))
    {
        QPointF mousePoint = ui->graphicsView->mapToScene(remapped);
        int x = mousePoint.x();
        int y = mousePoint.y();
        if (ui->radioSeed->isChecked())
        {
            if (it != nullptr)
                delete it;
            it = new QGraphicsEllipseItem(x - 5, y - 5, 10, 10);
            it->setBrush(QBrush(Qt::blue));
            it->setPen(QPen(Qt::white));
            scene.addItem(it);
        }
        else
        {
            if (flagEnter)
            {
                int dx = x - d0.x();
                int dy = y - d0.y();
                if (abs(dx) > abs(dy))
                    y -= dy;
                else
                    x -= dx;
            }
            appendDot(x, y);
        }
    }
}

void MainWindow::on_buttonCloseFig_clicked()
{
    painter = new QPainter(&image);
    painter->setPen(pencolor);
    painter->drawLine(d0, dstart);
    scene.addPixmap(QPixmap::fromImage(image));
    delete painter;
    d0 = QPoint(-1, -1);
    dstart = QPoint(-1, -1);
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

bool MainWindow::unvoidPixel(int x, int y)
{
    if (x < image.width() - 1 && x > 0 && y < image.height() - 1 && y > 0)
    {
        QColor cl = image.pixelColor(x, y);
        return cl.toRgb() == pencolor.toRgb() || cl.toRgb() == shadecolor.toRgb();
    }
    return false;
}

void MainWindow::findSeed(std::stack<dot> &st, int x_left, int x_right, int y)
{
    int x = x_left;
    bool flag = false;
    while (x <= x_right)
    {
        while (!unvoidPixel(x, y) && x < x_right)
        {
            flag = true;
            x++;
        }
        if (flag)
        {
            //if (x == x_right && !unvoidPixel(x, y))
            dot d;
            d.x = x - 1;
            d.y = y;
            st.push(d);
        }
        do
            x++;
        while (unvoidPixel(x, y) && x < x_right);
    }
}

void MainWindow::LineSeedShade(dot seed, bool stop)
{
    colorCheck();
    std::stack <dot> st;
    dot px;
    int x, y;
    int x_right, x_left;
    st.push(seed);
    while (st.size() > 0)
    {
        px = st.top();
        st.pop();
        x = px.x;
        y = px.y;
        if (image.pixelColor(x, y).toRgb() == shadecolor.toRgb())
            continue;
        while (image.pixelColor(x, y).toRgb() != pencolor.toRgb() && x < image.width())
        {
            image.setPixelColor(x, y, shadecolor);
            x++;
        }
        x_right = x - 1;
        x = px.x - 1;
        while (image.pixelColor(x, y).toRgb() != pencolor.toRgb() && x > 0)
        {
            image.setPixelColor(x, y, shadecolor);
            x--;
        }
        x_left = x + 1;
        if (y > 0)
            findSeed(st, x_left, x_right, y - 1);
        if (y < image.height())
            findSeed(st, x_left, x_right, y + 1);
        ui->graphicsView->scene()->clear();
        scene.addPixmap(QPixmap::fromImage(image));
        if (stop)
            qApp->processEvents();
    }
}

void MainWindow::on_buttonFill_clicked()
{
    dot d;
    d.x = it->rect().x() + 5;
    d.y = it->rect().y() + 5;
    delete it;
    it = nullptr;
    timeval sta, enn;
    int64_t elapsed_time_k = 0;
    mingw_gettimeofday(&sta, NULL);
    LineSeedShade(d, false);
    mingw_gettimeofday(&enn, NULL);
    elapsed_time_k = (enn.tv_sec - sta.tv_sec) * 1000000LL + (enn.tv_usec - sta.tv_usec);
    char buf[1024];
    sprintf(buf, "%.3lf c", elapsed_time_k / 1000000.);
    ui->timeLable->setText(QString(buf));

}


void MainWindow::on_buttonFillStop_clicked()
{
    dot d;
    d.x = it->rect().x() + 5;
    d.y = it->rect().y() + 5;
    delete it;
    it = nullptr;
    timeval sta, enn;
    int64_t elapsed_time_k = 0;
    mingw_gettimeofday(&sta, NULL);
    LineSeedShade(d, true);
    mingw_gettimeofday(&enn, NULL);
    elapsed_time_k = (enn.tv_sec - sta.tv_sec) * 1000000LL + (enn.tv_usec - sta.tv_usec);
    char buf[1024];
    sprintf(buf, "%.3lf c", elapsed_time_k / 1000000.);
    ui->timeLable->setText(QString(buf));

}


void MainWindow::on_pushButton_3_clicked()
{
    int x = ui->spinBoxX->value();
    int y = ui->spinBoxY->value();
    appendDot(x, y);
}


void MainWindow::on_butClear_clicked()
{
    ui->graphicsView->scene()->clear();
    image = QImage(900, 900, QImage::Format_RGB32);
    scene.addPixmap(QPixmap::fromImage(image));
}


void MainWindow::on_buttonAddOkr_clicked()
{
    int x = ui->spinXOkr->value();
    int y = ui->spinYOkr->value();
    int r = ui->spinROkr->value();
    painter = new QPainter(&image);
    painter->setPen(pencolor);
    painter->drawEllipse(x - r, y - r, 2 * r, 2 * r);
    scene.addPixmap(QPixmap::fromImage(image));
    delete painter;
}

