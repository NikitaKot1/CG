#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QImage>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <stack>
#include <QComboBox>

struct dot{
    int x;
    int y;
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
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void on_buttonCloseFig_clicked();
    void on_buttonFill_clicked();
    void on_buttonFillStop_clicked();
    void on_pushButton_3_clicked();
    void on_butClear_clicked();
    void on_buttonAddOkr_clicked();

public:
    void appendDot(int x, int y);
    void LineSeedShade(dot seed, bool draw);
    void findSeed(std::stack <dot> &st, int x_left, int x_right, int y);
    bool unvoidPixel(int x, int y);
    void addColorsToComboBox(QComboBox *cmb);
    void colorCheck();

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QImage image;
    QPainter *painter;
    QGraphicsEllipseItem *it = nullptr;
    QColor pencolor = Qt::red;
    QColor shadecolor = Qt::green;
    QPoint d0 = QPoint(-1, -1);
    QPoint dstart = QPoint(-1, -1);
};


#endif // MAINWINDOW_H
