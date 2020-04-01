#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mytimer.h"

#include <QMainWindow>
#include <QMouseEvent>
#include <QTimer>
#include <QThread>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void nextSecond(MyTimer::timerState st, int currScnds);
    void showWindow();
    void hideWindow();
private slots:
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

    void on_lockButton_clicked();
    void on_closeButton_clicked();

    void on_pauseButton_clicked(bool checked);

    void contextMenuEvent(QContextMenuEvent *event);
private:
    Ui::MainWindow *ui;
    QMenu *menu;

    QPoint startPos;
    QRect showRect;
    bool isMoving = false;

    enum sideType
    {
        SIDE_UP,
        SIDE_DOWN,
        SIDE_LEFT,
        SIDE_RIGHT,
    };
    sideType edgeSide = SIDE_UP;
    bool isEdge = true;
    bool nearZero = false;
    const int EDGESIZE = 3;
    QList<QRect> screenList;

signals:
    void restNow(MyTimer::timerState st=MyTimer::STATE_REST);
    void pause(bool st);
};
#endif // MAINWINDOW_H
