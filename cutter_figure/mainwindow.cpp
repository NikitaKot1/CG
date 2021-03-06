#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QPixmap>
#include <QLayout>
#include <cmath>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	pixmap = QPixmap(ui->drawLabel->width(), ui->drawLabel->height());
	ui->drawLabel->setPixmapPointer(pixmap);

	on_clearPushButton_clicked();
}

MainWindow::~MainWindow()
{
	delete ui;
}

int sign(int x)
{
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    return 0;
}

void MainWindow::clearTable(QTableWidget* table)
{
    int n = table->rowCount();
    for (int i = n - 1; i >= 0; i--)
    {
        table->item(i, 0);
        table->item(i, 1);
    }
    table->setRowCount(0);
    linesClip.clear();
}

void MainWindow::appendDotClipperNot1(int x1, int y1)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    char buf[10];
    std::sprintf(buf, "%d", x1);
    QTableWidgetItem *iten = new QTableWidgetItem(QString(buf));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, iten);
    QTableWidgetItem *iten3 = new QTableWidgetItem(QString(buf));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 2, 2, iten3);
    std::sprintf(buf, "%d", y1);
    QTableWidgetItem *iten2 = new QTableWidgetItem(QString(buf));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, iten2);
    QTableWidgetItem *iten4 = new QTableWidgetItem(QString(buf));
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 2, 3, iten4);
}

void MainWindow::appendDotClipper(int x1, int y1)
{
    if (startClipper)
        clearTable(ui->tableWidget);
    if (ui->tableWidget->rowCount() == 0)
    {
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        char buf[10];
        std::sprintf(buf, "%d", x1);
        QTableWidgetItem *iten = new QTableWidgetItem(QString(buf));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, iten);
        std::sprintf(buf, "%d", y1);
        QTableWidgetItem *iten2 = new QTableWidgetItem(QString(buf));
        ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, iten2);
    }
    else
    {
        appendDotClipperNot1(x1, y1);
    }
    startClipper = false;
    linesClip.push_back(QPoint(x1, y1));
    displayImage();
}

void MainWindow::appendDotFigureNot1(int x, int y)
{
    ui->tableWidgetFigure->setRowCount(ui->tableWidgetFigure->rowCount()+1);
    char buf[10];
    std::sprintf(buf, "%d", x);
    QTableWidgetItem *iten = new QTableWidgetItem(QString(buf));
    ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 1, 0, iten);
    QTableWidgetItem *iten3 = new QTableWidgetItem(QString(buf));
    ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 2, 2, iten3);
    std::sprintf(buf, "%d", y);
    QTableWidgetItem *iten2 = new QTableWidgetItem(QString(buf));
    ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 1, 1, iten2);
    QTableWidgetItem *iten4 = new QTableWidgetItem(QString(buf));
    ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 2, 3, iten4);
}

void MainWindow::appendDotFigure(int x, int y)
{
    if (startFigure)
        clearTable(ui->tableWidgetFigure);
    if (ui->tableWidgetFigure->rowCount() == 0)
    {
        ui->tableWidgetFigure->setRowCount(ui->tableWidgetFigure->rowCount()+1);
        char buf[10];
        std::sprintf(buf, "%d", x);
        QTableWidgetItem *iten = new QTableWidgetItem(QString(buf));
        ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 1, 0, iten);
        std::sprintf(buf, "%d", y);
        QTableWidgetItem *iten2 = new QTableWidgetItem(QString(buf));
        ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 1, 1, iten2);
    }
    else
    {
        appendDotFigureNot1(x, y);
    }
    startFigure = false;
    linesFig.push_back(QPoint(x, y));
    displayImage();
}

void MainWindow::on_addLinePushButton_clicked()
{
    const int x1 = ui->x1setLineSpinBox->value();
    const int y1 = ui->y1setLineSpinBox->value();

    appendDotFigure(x1, y1);
}

void MainWindow::on_setClipperPushButton_clicked()
{
    int x1 = ui->x1addDotClipSpinBox->value();
    int y1 = ui->y1addDotClipSpinBox->value();

    appendDotClipper(x1, y1);
}

void MainWindow::on_setLineBaseColorPushButton_clicked()
{
	setLineBaseColor(QColorDialog::getColor(lineBaseColor, this, "Pick a line base color", QColorDialog::DontUseNativeDialog));
	displayImage();
}

void MainWindow::on_setClipperColorPushButton_clicked()
{
	setClipperColor(QColorDialog::getColor(clipperColor, this, "Pick a clipper color", QColorDialog::DontUseNativeDialog));
	displayImage();
}

void MainWindow::on_setClippedLineColorPushButton_clicked()
{
	setClippedLineColor(QColorDialog::getColor(clippedLineColor, this, "Pick a clipped line color", QColorDialog::DontUseNativeDialog));
	displayImage();
}

QVector<QLine> verticesToEdges(const QVector<QPoint>& vertices)
{
    QVector<QLine> edges;

    for (int i = 1; i < vertices.size(); ++i)
        edges.push_back(QLine(vertices[i - 1], vertices[i]));
    edges.push_back(QLine(vertices.back(), vertices.front()));

    return edges;
}

void MainWindow::on_clipPushButton_clicked()
{
    if (!startFigure) {
            QMessageBox::critical(this, "Error", "Polygon must be closed");
            return;
    }

    if (!startClipper) {
        QMessageBox::critical(this, "Error", "Clipper must be closed");
        return;
    }

    int direction = checkClipper();
    if (!direction) {
        QMessageBox::critical(this, "Error", "Clipper must be convex");
        return;
    }

    QPainter painter(&pixmap);
    painter.setPen(QPen(clippedLineColor, 3));
    auto clipped = clipPolygon(direction);

    for (int i = 1; i < clipped.size(); ++i)
        painter.drawLine(clipped[i - 1], clipped[i]);

    if (clipped.size() > 1)
        painter.drawLine(clipped.back(), clipped.front());

    ui->drawLabel->update();
}

void MainWindow::on_clearPushButton_clicked()
{
    on_clearPushButtonClipper_clicked();
    on_clearPushButtonFigure_clicked();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	int x = event->x() - ui->drawLabel->x();
	int y = event->y() - ui->drawLabel->y();

	if (x < 0 || x >= ui->drawLabel->width() || y < 0 || y >= ui->drawLabel->height())
		return;

	switch (event->button()) {
    case Qt::LeftButton:
        ui->x1setLineSpinBox->setValue(x);
        ui->y1setLineSpinBox->setValue(y);
        appendDotFigure(x, y);
        break;
    case Qt::RightButton:
        ui->x1addDotClipSpinBox->setValue(x);
        ui->y1addDotClipSpinBox->setValue(y);
        appendDotClipper(x, y);
        break;

	default:
		break;
	}
}

void MainWindow::receiveKeyboardModifiers(int &x, int &y)
{
    const int x1 = ui->x1setLineSpinBox->value();
    const int y1 = ui->y1setLineSpinBox->value();

    switch (QApplication::keyboardModifiers()) {
	case Qt::ControlModifier: {
        const int dx = x - x1;
        const int dy = y - y1;
        if (dx > dy)
            y = y1;
        else
            x = x1;
        break;
    }
	default:
		break;
    }
}

void MainWindow::displayImage()
{
	pixmap.fill();

	QPainter painter(&pixmap);

	painter.setPen(lineBaseColor);
    for (int i = 0; i < linesFig.size() - 1; i++)
        painter.drawLine(QLine(linesFig[i], linesFig[i+1]));
    if (startFigure && linesFig.size()) painter.drawLine(QLine(linesFig[0], linesFig[linesFig.size()-1]));

    painter.setPen(QPen(clipperColor, 3));
    for (int i = 0; i < linesClip.size() - 1; i++)
        painter.drawLine(QLine(linesClip[i], linesClip[i+1]));
    if (startClipper && linesClip.size()) painter.drawLine(QLine(linesClip[0], linesClip[linesClip.size()-1]));

	ui->drawLabel->update();
}

void MainWindow::setLineBaseColor(const QColor &color)
{
	lineBaseColor = color;
	colorLabel(lineBaseColor, ui->lineBaseColorLabel);
}

void MainWindow::setClipperColor(const QColor &color)
{
	clipperColor = color;
	colorLabel(clipperColor, ui->clipperColorLabel);
}

void MainWindow::setClippedLineColor(const QColor &color)
{
	clippedLineColor = color;
	colorLabel(clippedLineColor, ui->clippedLineColorLabel);
}

void MainWindow::colorLabel(const QColor &color, QLabel *label) {
	QPalette palette = label->palette();
	palette.setColor(label->backgroundRole(), color);
	label->setAutoFillBackground(true);
	label->setPalette(palette);
}


void MainWindow::addLineClipper(const QPoint &point)
{
    linesClip.push_back(point);
    displayImage();
}

int skewProduct(const QPoint &a, const QPoint &b)
{
    return a.x() * b.y() - a.y() * b.x();
}

int direction(const QPoint &prev, const QPoint &curr, const QPoint &next)
{
    return skewProduct(curr - prev, next - curr) > 0 ? 1 : -1;
}

int MainWindow::checkClipper()
{
    int flag = 1;
    QPoint prev = linesClip.back();
    QPoint curr = linesClip[0];
    QPoint next = linesClip[1];

    int prev_direction = direction(prev, curr, next);
    int curr_direction = 0;

    for (int i = 1; flag && i < linesClip.size(); i++)
    {
        prev = curr;
        curr = next;
        next = linesClip[(i + 1) % linesClip.size()];
        curr_direction = direction(prev, curr, next);

        flag = curr_direction == prev_direction;
    }
    return flag * curr_direction;
}

QPointF perpendicular(const QPointF &point)
{
    return {-point.y(), point.x()};
}

double dotProduct(const QPointF &a, const QPointF &b)
{
    return a.x() * b.x() + a.y() * b.y();
}




void MainWindow::on_closeClipperButton_clicked()
{
    if (startClipper) {
            QMessageBox::critical(this, "Error", "Clipper already closed");
            return;
        }

    if (linesClip.size() <= 2) {
        QMessageBox::critical(this, "Error", "Need at least 2 edges");
        return;
    }
    QTableWidgetItem *iten = new QTableWidgetItem(ui->tableWidget->item(0, 0)->text());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, iten);
    QTableWidgetItem *iten2 = new QTableWidgetItem(ui->tableWidget->item(0, 1)->text());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, iten2);
    startClipper = true;
    displayImage();
}

void MainWindow::on_closeFigureButton_clicked()
{
    if (startFigure) {
            QMessageBox::critical(this, "Error", "Figure already closed");
            return;
        }

    if (linesFig.size() <= 2) {
        QMessageBox::critical(this, "Error", "Need at least 2 edges");
        return;
    }
    QTableWidgetItem *iten = new QTableWidgetItem(ui->tableWidgetFigure->item(0, 0)->text());
    ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 1, 2, iten);
    QTableWidgetItem *iten2 = new QTableWidgetItem(ui->tableWidgetFigure->item(0, 1)->text());
    ui->tableWidgetFigure->setItem(ui->tableWidgetFigure->rowCount() - 1, 3, iten2);
    startFigure = true;
    displayImage();
}


void MainWindow::on_clearPushButtonFigure_clicked()
{
    linesFig.clear();
    displayImage();

    setLineBaseColor(Qt::red);
    setClipperColor(Qt::black);
    setClippedLineColor(Qt::blue);

    clearTable(ui->tableWidgetFigure);
    startFigure = true;
}


void MainWindow::on_clearPushButtonClipper_clicked()
{
    linesClip.clear();
    displayImage();

    setLineBaseColor(Qt::red);
    setClipperColor(Qt::black);
    setClippedLineColor(Qt::blue);

    clearTable(ui->tableWidget);
    startClipper = true;
}



QVector<QPoint> MainWindow::clipPolygon(int direction)
{
    QVector<QPoint> result;
    QVector<QPoint> polygon = linesFig;
    QVector<QPoint> clipper = linesClip;
    clipper.push_back(clipper.front());

    QPoint first, start;

    for (int i = 0; i < clipper.size() - 1; ++i) {
        for (int j = 0; j < polygon.size(); ++j) {
            if (!j)
                first = polygon[j];
            else if (checkIntersection(start, polygon[j], clipper[i], clipper[i + 1]))
                result.push_back(intersection(start, polygon[j], clipper[i], clipper[i + 1]));

            start = polygon[j];
            if (isVisible(start, clipper[i], clipper[i + 1]) * direction < 0)
                result.push_back(start);
        }

        if (!result.empty()) {
            if (checkIntersection(start, first, clipper[i], clipper[i + 1]))
                result.push_back(intersection(start, first, clipper[i], clipper[i + 1]));
        }

        polygon = result;
        result.clear();
    }

    return polygon;
}

bool MainWindow::checkIntersection(const QPoint &sp, const QPoint &ep, const QPoint &p0, const QPoint &p1)
{
    return isVisible(sp, p0, p1) * isVisible(ep, p0, p1) <= 0;
}

int MainWindow::isVisible(const QPoint &p, const QPoint &p1, const QPoint &p2)
{
    return sign(skewProduct(p - p1, p2 - p1));
}

QPoint MainWindow::intersection(QPoint &p1, QPoint &p2, QPoint &cp1, QPoint &cp2) {
    const int det = skewProduct(p2 - p1, cp1 - cp2);
    const double t = static_cast<double>(skewProduct(cp1 - p1, cp1 - cp2)) / det;
    return p1 + (p2 - p1) * t;
}

