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
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    showRect.setHeight(30);

    settings = new MySettings("rest_settings.ini", MySettings::IniFormat, this);

    for(QScreen* item : QApplication::screens())
        screenList.append(item->availableGeometry());

    myTimer = new MyTimer(this);
    connect(myTimer, &MyTimer::scndChanged, this, &MainWindow::nextSecond);
    connect(myTimer, &MyTimer::nearZeroAlert, this, &MainWindow::showWindow);
    connect(myTimer, &MyTimer::newRound, this, &MainWindow::hideWindow);
    connect(this, &MainWindow::restNow, myTimer, &MyTimer::setState);
    connect(this, &MainWindow::pause, myTimer, &MyTimer::enableTimer);
    onSettingChanged(settings->value("isSpl").toBool(),
                     settings->value("Wh").toInt(),
                     settings->value("Wm").toInt(),
                     settings->value("Ws").toInt(),
                     settings->value("Rh").toInt(),
                     settings->value("Rm").toInt(),
                     settings->value("Rs").toInt());

    myTimer->setState(MyTimer::STATE_CTDN);

    menu = new QMenu(this);
    menu->addAction("Rest now", [ = ]()
    {
        on_lockButton_clicked();
    });
    menu->addAction("Pause", [ = ]()
    {
        on_pauseButton_clicked(!(ui->pauseButton->isChecked()));
    });
    menu->addAction("Settings", [ = ]()
    {
        enterSettings();
    });
    menu->addAction("Exit", [ = ]()
    {
        on_closeButton_clicked();
    });

    this->move((QApplication::screenAt(QCursor().pos())->geometry().width() - this->geometry().width()) / 2, 0);
    hideWindow();// 启动后直接隐藏窗口

    update();

    WTSRegisterSessionNotification((HWND)this->winId(), NOTIFY_FOR_ALL_SESSIONS);
}

MainWindow::~MainWindow()
{
    WTSUnRegisterSessionNotification((HWND)this->winId());
    delete ui;
}


void MainWindow::on_lockButton_clicked()
{
    emit restNow();
    QThread::msleep(1000);
    MyTimer::closeScreen();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
//    qDebug() << "move:" << e->pos() - startPos;
    if(isMoving)
    {
        this->setGeometry(QRect(this->geometry().topLeft() + (e->pos() - startPos), showRect.size()));
        ui->centralwidget->resize(showRect.size());
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
//    qDebug() << "screen:" << currScreen;
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
    ui->centralwidget->resize(showRect.size());
//    qDebug() << "target:" << showRect;
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    startPos = e->pos();
    isMoving = true;
}

void MainWindow::enterEvent(QEvent *e)
{
    showWindow();
}

void MainWindow::leaveEvent(QEvent *e)
{
    hideWindow();
}

void MainWindow::on_closeButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_pauseButton_clicked(bool checked)
{
    ui->pauseButton->setChecked(checked);
    ui->lockButton->setEnabled(!checked);
    emit pause(!checked);
}

void MainWindow::showWindow()
{
    if(isEdge)
    {
        this->move(showRect.topLeft());
//        qDebug() << "show:" << this->geometry();
    }
}

void MainWindow::hideWindow()
{
    if(!nearZero && isEdge)
    {
        if(edgeSide == SIDE_UP)
            this->move(showRect.x(), -showRect.height() + EDGESIZE);
        else if(edgeSide == SIDE_DOWN)
            this->move(showRect.x(), showRect.top() + showRect.height() - 2 * EDGESIZE);
        else if(edgeSide == SIDE_LEFT)
            this->move(-showRect.width() + EDGESIZE, showRect.y());
        else if(edgeSide == SIDE_RIGHT)
            this->move(showRect.left() + showRect.width() - EDGESIZE, showRect.y());
//        qDebug() << "hide:" << this->geometry();
    }
}

void MainWindow::nextSecond(MyTimer::timerState st, int currScnds)
{
    if(st == MyTimer::STATE_CTDN)
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

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    qDebug() << "triggered";
    menu->exec(event->globalPos());

}

void MainWindow::enterSettings()
{
    SettingDialog* settingDialog = new SettingDialog(settings, this);
    connect(settingDialog, &SettingDialog::settingChanged, this, &MainWindow::onSettingChanged);
    settingDialog->show();
//    settingDialog->exec();
}
void MainWindow::onSettingChanged(bool isSpl, int Wh, int Wm, int Ws, int Rh, int Rm, int Rs)
{
    qDebug() << ui->ctdnLabel->width() << ui->lockButton->width() << ui->pauseButton->width() << ui->closeButton->width();
    qDebug() << this->width() << ui->centralwidget->width();
    if(isSpl)
    {
        ui->lockButton->setVisible(false);
        ui->pauseButton->setVisible(false);
        ui->closeButton->setVisible(false);
        showRect.setWidth(5 * 2 + ui->ctdnLabel->width());
    }
    else
    {
        ui->lockButton->setVisible(true);
        ui->pauseButton->setVisible(true);
        ui->closeButton->setVisible(true);
        showRect.setWidth(5 * 5 + ui->ctdnLabel->width() + ui->lockButton->width() + ui->pauseButton->width() + ui->closeButton->width());
    }
    qDebug() << this->width() << ui->centralwidget->width();
    myTimer->setCtdnTime(Wh * 3600 + Wm * 60 + Ws);
    myTimer->setRestTime(Rh * 3600 + Rm * 60 + Rs);
    ui->centralwidget->resize(showRect.size());
    this->resize(showRect.size());
    this->repaint();
    // It seems that the the size of MainWindow will not update now.
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* winMsg = static_cast<MSG *>(message);
    if(winMsg->message == WM_WTSSESSION_CHANGE)
    {
        qDebug() << GetCurrentTime() << QString::number(winMsg->message, 16) << winMsg->lParam << winMsg->wParam;
        if(winMsg->wParam == WTS_SESSION_UNLOCK && myTimer->getState() == MyTimer::STATE_REST)
        {
            myTimer->closeScreen();
        }
    }
    return false;
}

//void MainWindow::resizeEvent(QResizeEvent *event)
//{
//    qDebug() << event << event->oldSize() << event->size() << event->type();
//}
//void MainWindow::moveEvent(QMoveEvent *event)
//{
//    qDebug() << event << event->oldPos() << event->pos() << event->type();
//}
