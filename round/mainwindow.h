#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

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
    void on_radioBlue_clicked();
    void on_radioRed_clicked();
    void on_radioGreen_clicked();
    void on_radioWhite_clicked();
    void on_radioYellow_clicked();
    void on_radioAqua_clicked();
    void on_radioFront_clicked();

    void on_buttonClear_clicked();
    void on_buttonDrawSpec_clicked();
    void on_buttonDrawOkr_clicked();

private:
    void drawSpectorOkr(int xc, int yc, int n, int r, bool fl);
    void drawSpectorEll(int xc, int yc, int n, int a, int b, bool fl);
    void setPixel(int x, int y);


    void drawBrezenheimOkr(int xc, int yc, int r, bool dr);
    void drawBrezenheimEll(int xc, int yc, int a, int b, bool dr);

    void drawCanonOkr(int xc, int yc, int r, bool dr);
    void drawCanonEll(int xc, int yc, int a, int b, bool dr);

    void drawParamOkr(int xc, int yc, int r, bool dr);
    void drawParamEll(int xc, int yc, int a, int b, bool dr);

    void drawSrDotOkr(int xc, int yc, int r, bool dr);
    void drawSrDotEll(int xc, int yc, int a, int b, bool dr);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
};
#endif // MAINWINDOW_H
