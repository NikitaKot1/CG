#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPixmap>
#include <QVector>
#include <QLabel>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_addLinePushButton_clicked();
	void on_setClipperPushButton_clicked();
	void on_setLineBaseColorPushButton_clicked();
	void on_setClipperColorPushButton_clicked();
	void on_setClippedLineColorPushButton_clicked();
	void on_clipPushButton_clicked();
	void on_clearPushButton_clicked();
    void on_closeClipperButton_clicked();
    void on_closeFigureButton_clicked();
    void on_clearPushButtonFigure_clicked();
    void on_clearPushButtonClipper_clicked();

protected:
	void mousePressEvent(QMouseEvent *event);
	void receiveKeyboardModifiers(int &x, int &y);

private:
	Ui::MainWindow *ui;

	QPixmap pixmap;

	void clearImage();
	void displayImage();

	QColor lineBaseColor;
	QColor clipperColor;
	QColor clippedLineColor;
	void setLineBaseColor(const QColor &color);
	void setClipperColor(const QColor &color);
	void setClippedLineColor(const QColor &color);
	void colorLabel(const QColor &color, QLabel *label);
    void appendDotClipper(int x1, int y1);
    void appendDotClipperNot1(int x1, int y1);
    void appendDotFigure(int x, int y);
    void appendDotFigureNot1(int x, int y);
    void clearTable(QTableWidget* table);

    int checkClipper();
    QVector<QPoint> clipPolygon(int direction);
    bool checkIntersection(const QPoint &sp, const QPoint &ep, const QPoint &p0, const QPoint &p1);
    int isVisible(const QPoint &p, const QPoint &p1, const QPoint &p2);
    QPoint intersection(QPoint &p1, QPoint &p2, QPoint &cp1, QPoint &cp2);

    QVector<QPoint> linesClip;
    QVector<QPoint> linesFig;

    void addLineClipper(const QPoint &point);

    bool startClipper = false;
    bool startFigure = false;
};

#endif // MAINWINDOW_H
