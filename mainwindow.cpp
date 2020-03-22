#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QScreen>
#include <QString>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->move((QApplication::screenAt(QCursor().pos())->geometry().width() - this->geometry().width()) / 2, 0);
    showRect = this->geometry();
    for(QScreen* item : QApplication::screens())
        screenList.append(item->availableGeometry());
    hideWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_lockButton_clicked()
{
    emit restNow();
    QThread::msleep(1000);
    EventHandler::closeScreen();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
//    qDebug() << "move:" << e->pos() - startPos;
    if(isMoving)
    {
        this->setGeometry(QRect(this->geometry().topLeft() + (e->pos() - startPos), QSize(180, 30)));
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    isMoving = false;

    bool edges[4] = {false, false, false, false};
    for(QRect item : screenList) //判断四个顶点是否在屏幕区域内
    {
        if(item.contains(this->geometry().topLeft()))
            edges[0] = true;
        if(item.contains(this->geometry().topRight()))
            edges[1] = true;
        if(item.contains(this->geometry().bottomLeft()))
            edges[2] = true;
        if(item.contains(this->geometry().bottomRight()))
            edges[3] = true;
    }


    QRect currScreen = QApplication::screenAt(e->globalPos())->availableGeometry();
    qDebug() << "screen:" << currScreen;
    if(edges[0] && edges[1] && edges[2] && edges[3]) //跨屏幕边缘情况
        isEdge = false;
    else if(!edges[0] && !edges[1])
    {
        edgeSide = SIDE_UP;
        isEdge = true;
        this->move(this->geometry().left(), 0);
    }
    else if(!edges[2] && !edges[3])
    {
        edgeSide = SIDE_DOWN;
        isEdge = true;
        this->move(this->geometry().left(), currScreen.top() + currScreen.height() - showRect.height());
    }
    else if(!edges[0] && !edges[2])
    {
        edgeSide = SIDE_LEFT;
        isEdge = true;
        this->move(0, this->geometry().top());
    }
    else if(!edges[1] && !edges[3])
    {
        edgeSide = SIDE_RIGHT;
        isEdge = true;
        this->move(currScreen.left() + currScreen.width() - showRect.width(), this->geometry().top());
    }
    showRect = this->geometry();
    qDebug() << "target:" << showRect;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    startPos = e->pos();
    isMoving = true;
}

void MainWindow::enterEvent(QEvent *e)
{
    if(isEdge)
        showWindow();
}

void MainWindow::leaveEvent(QEvent *e)
{
    if(isEdge)
        hideWindow();
}

void MainWindow::on_closeButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pauseButton_clicked(bool checked)
{
    ui->lockButton->setEnabled(!checked);
    emit pause(!checked);
}

void MainWindow::showWindow()
{
    this->move(showRect.topLeft());
    qDebug() << "show:" << this->geometry();
}

void MainWindow::hideWindow()
{
    if(!nearZero)
    {
        if(edgeSide == SIDE_UP)
            this->move(showRect.x(), -showRect.height() + EDGESIZE);
        else if(edgeSide == SIDE_DOWN)
            this->move(showRect.x(), showRect.top() + showRect.height() - 2 * EDGESIZE);
        else if(edgeSide == SIDE_LEFT)
            this->move(-showRect.width() + EDGESIZE, showRect.y());
        else if(edgeSide == SIDE_RIGHT)
            this->move(showRect.left() + showRect.width() - EDGESIZE, showRect.y());
        qDebug() << "hide:" << this->geometry();
    }
}

void MainWindow::nextSecond(EventHandler::timerState st, int currScnds)
{
    if(st == EventHandler::STATE_CTDN)
    {
        nearZero = currScnds <= 30;
        ui->ctdnLabel->setText(QString("%1 %2:%3:%4")
                               .arg("W")
                               .arg(currScnds / 60 / 60, 2, 10, QLatin1Char('0'))
                               .arg(currScnds / 60 % 60, 2, 10, QLatin1Char('0'))
                               .arg(currScnds % 60, 2, 10, QLatin1Char('0')));
    }
    else
    {
        ui->ctdnLabel->setText(QString("%1 %2:%3:%4")
                               .arg("R")
                               .arg(currScnds / 60 / 60, 2, 10, QLatin1Char('0'))
                               .arg(currScnds / 60 % 60, 2, 10, QLatin1Char('0'))
                               .arg(currScnds % 60, 2, 10, QLatin1Char('0')));
    }
}

