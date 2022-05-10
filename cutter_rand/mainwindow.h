#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPixmap>
#include <QVector>
#include <QLabel>

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
    void clearTable();

    int checkClipper();

	QVector<QLine> lines;
    QVector<QPoint> linesClip;
	QRect clipper;
	bool left_clicked;
	bool right_clicked;
	void addLine(const QLine &line);
    void addLineClipper(const QPoint &point);

    void clipLine(const QLine &line, int direction, QVector<QLine> enges, QPainter &painter);
	int code(const QPoint &point, int xl, int xr, int yb, int yt);

    bool startClipper = false;
};

#endif // MAINWINDOW_H
