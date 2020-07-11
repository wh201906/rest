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

    settings = new MySettings("rest_settings.ini", MySettings::IniFormat, this);

    for(QScreen* item : QApplication::screens())
        screenList.append(item->availableGeometry());

    myTimer = new MyTimer(this);
    connect(myTimer, &MyTimer::scndChanged, this, &MainWindow::nextSecond);
    connect(myTimer, &MyTimer::nearZeroAlert, this, &MainWindow::showWindow);
    connect(myTimer, &MyTimer::newRound, this, &MainWindow::hideWindow);
    connect(this, &MainWindow::restNow, myTimer, &MyTimer::setState);
    connect(this, &MainWindow::pause, myTimer, &MyTimer::enableTimer);
    onSettingChanged(settings->getCurrent());

    myTimer->setState(MyTimer::STATE_CTDN);

    menu = new QMenu(this);
    myInfo = new QAction("wh201906", this);
    connect(myInfo, &QAction::triggered, [ = ]()
    {
        QDesktopServices::openUrl(QUrl("https://github.com/wh201906"));
    });

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
    menu->addSeparator();
    menu->addAction(myInfo);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    showRect.setSize(QSize(this->width(), 30)); // set the showRect first, then resize the window and widgets.
    showRect.moveTo(
        settings->value("lastPositionX", (QApplication::screenAt(QCursor().pos())->geometry().width() - this->geometry().width()) / 2).toInt(),
        settings->value("lastPositionY", 0).toInt()); // don't use setTopLeft()
    this->setFixedSize(showRect.size());
    this->move(showRect.topLeft());
    edgeDetect();
    hideWindow();// If the window is at the edge, then hide it.

    WTSRegisterSessionNotification((HWND)this->winId(), NOTIFY_FOR_ALL_SESSIONS);
}

MainWindow::~MainWindow()
{
    WTSUnRegisterSessionNotification((HWND)this->winId());
    qDebug() << "MainWindow destroyed";
    delete ui;
}


void MainWindow::on_lockButton_clicked()
{
    emit restNow();
    QThread::msleep(1000);
    MyTimer::Lock();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(isMoving)
    {
        showRect.moveTo(showRect.topLeft() + (e->pos() - startPos));
        this->move(showRect.topLeft());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    isMoving = false;

    edgeDetect(); // the mouse and window are in the same screen.

    this->move(showRect.topLeft());
    settings->setValue("lastPositionX", showRect.left());
    settings->setValue("lastPositionY", showRect.top());

}

void MainWindow::edgeDetect()
{
    bool edges[4] = {false, false, false, false};
    QRect currScreen;
    for(QRect item : screenList) //detect whether the screens contains the 4 corners
    {
        if(item.contains(this->geometry().topLeft()))
            edges[0] = true;
        if(item.contains(this->geometry().topRight()))
            edges[1] = true;
        if(item.contains(this->geometry().bottomLeft()))
            edges[2] = true;
        if(item.contains(this->geometry().bottomRight()))
            edges[3] = true;

        if(item.intersects(showRect)) // detect the current screen
        {
            currScreen = QApplication::screenAt(item.center())->availableGeometry();
        }
    }

    if(edges[0] && edges[1] && edges[2] && edges[3]) //detect which edge is in the screen area
        edgeSide = SIDE_NONE;
    else if(!edges[0] && !edges[1])
    {
        edgeSide = SIDE_UP;
        showRect.moveTo(this->geometry().left(), 0);
    }
    else if(!edges[2] && !edges[3])
    {
        edgeSide = SIDE_DOWN;
        showRect.moveTo(this->geometry().left(), currScreen.top() + currScreen.height() - showRect.height());
    }
    else if(!edges[0] && !edges[2])
    {
        edgeSide = SIDE_LEFT;
        showRect.moveTo(0, this->geometry().top());
    }
    else if(!edges[1] && !edges[3])
    {
        edgeSide = SIDE_RIGHT;
        showRect.moveTo(currScreen.left() + currScreen.width() - showRect.width(), this->geometry().top());
    }
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
    if(edgeSide != SIDE_NONE)
    {
        this->move(showRect.topLeft());
    }
}

void MainWindow::hideWindow()
{
    if(!nearZero && myTimer->getState() != MyTimer::STATE_REST && edgeSide != SIDE_NONE)
    {
        if(edgeSide == SIDE_UP)
            this->move(showRect.x(), -showRect.height() + EDGESIZE);
        else if(edgeSide == SIDE_DOWN)
            this->move(showRect.x(), showRect.top() + showRect.height() - 2 * EDGESIZE);
        else if(edgeSide == SIDE_LEFT)
            this->move(-showRect.width() + EDGESIZE, showRect.y());
        else if(edgeSide == SIDE_RIGHT)
            this->move(showRect.left() + showRect.width() - EDGESIZE, showRect.y());
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
    menu->exec(event->globalPos());
}

void MainWindow::enterSettings()
{
    SettingDialog* settingDialog = new SettingDialog(settings, this);
    connect(settingDialog, &SettingDialog::settingChanged, this, &MainWindow::onSettingChanged);
    settingDialog->show();
}
void MainWindow::onSettingChanged(MySettings::Items items)
{
    if(items["isSimple"].toBool())
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
    myTimer->setCtdnTime(items["Wh"].toInt() * 3600 + items["Wm"].toInt() * 60 + items["Ws"].toInt());
    myTimer->setRestTime(items["Rh"].toInt() * 3600 + items["Rm"].toInt() * 60 + items["Rs"].toInt());
    isForceLock = items["isForceLock"].toBool();
    this->setFixedWidth(showRect.width()); // resize() dosen't work there.
    ui->centralwidget->setFixedWidth(showRect.width()); // resize() dosen't work there.
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* winMsg = static_cast<MSG *>(message);
    if(winMsg->message == WM_WTSSESSION_CHANGE && winMsg->wParam == WTS_SESSION_UNLOCK)
    {
        if(isForceLock && myTimer->getState() == MyTimer::STATE_REST)
        {
            for(int i = 0; i < 60; i++) // 3 seconds for force quit
            {
                QThread::msleep(50);
                QApplication::processEvents();
            }
            MyTimer::Lock();
        }
    }
//    else if(winMsg->message == WM_DISPLAYCHANGE) //
//    {
//        qDebug() << "WM_DISPLAYCHANGE";
//        update();
//    }
    return false;
}
