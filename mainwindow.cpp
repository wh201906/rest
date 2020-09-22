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
    connect(this, &MainWindow::skip, myTimer, &MyTimer::setState);
    connect(this, &MainWindow::pause, myTimer, &MyTimer::enableTimer);
    connect(this, &MainWindow::lockStateChanged, myTimer, &MyTimer::onLockStateChanged);
    onSettingChanged(settings->getCurrent());
    QFont labelFont;
    labelFont = ui->ctdnLabel->font();
    labelFont.setPixelSize(15);
    ui->ctdnLabel->setFont(labelFont);
    ui->lockButton->setFont(labelFont);
    ui->pauseButton->setFont(labelFont);
    ui->skipButton->setFont(labelFont);
    ui->closeButton->setFont(labelFont);

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
    pauseAction = menu->addAction("Pause", [ = ]()
    {
        on_pauseButton_clicked(!(ui->pauseButton->isChecked()));
    });
    menu->addAction("Skip", [ = ]()
    {
        on_skipButton_clicked();
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
    this->setWindowFlag(Qt::Tool, !settings->value("hasTaskbarItem").toBool());
    // after the window is created, use setWindowFlag will hide the window, and if I attempt to use show() after setWindowFlag(), the window will be strange.
    // So I have to only use it before the window is created.
    this->setAttribute(Qt::WA_TranslucentBackground);

    showRect.setSize(QSize(this->width(), 30)); // set the showRect first, then resize the window and widgets.
    showRect.moveTo(
        settings->value("lastPositionX", (QApplication::screenAt(QCursor().pos())->geometry().width() - this->geometry().width()) / 2).toInt(),
        settings->value("lastPositionY", 0).toInt()); // don't use setTopLeft()
    this->setFixedSize(showRect.size());
    this->move(showRect.topLeft());
    edgeDetect();
    hideWindow();// If the window is at the edge, then hide it.

    QDateTime dateTime = QDateTime::currentDateTime();
    reportThread = new QThread(this);
    report = new ReportWriter(this);
    report->setRecording(settings->value("isRecordingUsage").toBool());
    report->setFileName("rest-" + dateTime.date().toString(Qt::ISODate) + ".log");
    report->moveToThread(reportThread);
    connect(myTimer, &MyTimer::writeMsg, report, &ReportWriter::write);
    connect(this, &MainWindow::writeMsg, report, &ReportWriter::write);

    WTSRegisterSessionNotification((HWND)this->winId(), NOTIFY_FOR_ALL_SESSIONS);

    emit writeMsg(dateTime.date().toString(Qt::ISODate) + "-" + dateTime.time().toString(Qt::ISODate) + ":start");
}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow destroyed";
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    WTSUnRegisterSessionNotification((HWND)this->winId());
    QDateTime dateTime = QDateTime::currentDateTime();
    emit writeMsg(dateTime.date().toString(Qt::ISODate) + "-" + dateTime.time().toString(Qt::ISODate) + ":stop");
    event->accept();
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
    for(QRect item : screenList) //detect whether the screens contains the 4 corners(for every screen)
    {
        if(this->geometry().top() <= item.top()) // the contains() will not handle the situation when this->geometry().top() == item.top()
            edges[0] = true;
        if(this->geometry().bottom() >= item.bottom())
            edges[1] = true;
        if(this->geometry().left() <= item.left())
            edges[2] = true;
        if(this->geometry().right() >= item.right())
            edges[3] = true;

        if(item.intersects(showRect)) // detect the current screen
        {
            currScreen = QApplication::screenAt(item.center())->availableGeometry();
        }
    }

    if(!edges[0] && !edges[1] && !edges[2] && !edges[3]) //detect which edge is in the screen area
        edgeSide = SIDE_NONE;
    else if(edges[0])
    {
        edgeSide = SIDE_UP;
        showRect.moveTo(this->geometry().left(), 0);
    }
    else if(edges[1])
    {
        edgeSide = SIDE_DOWN;
        showRect.moveTo(this->geometry().left(), currScreen.top() + currScreen.height() - showRect.height());
    }
    else if(edges[2])
    {
        edgeSide = SIDE_LEFT;
        showRect.moveTo(0, this->geometry().top());
    }
    else if(edges[3])
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
    close();
    QApplication::exit();
}

void MainWindow::on_pauseButton_clicked(bool checked)
{
    if(checked)
    {
        pauseAction->setText("Resume");
    }
    else
    {
        pauseAction->setText("Pause");
    }
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
        ui->skipButton->setVisible(false);
        ui->ctdnLabel->adjustSize(); // Essential, otherwise the width of ctdnLabel will be unexpected.
        showRect.setWidth(5 * 2 + ui->ctdnLabel->width());
        qDebug() << "width:" << showRect.width() << ui->ctdnLabel->width() << ui->lockButton->width() << ui->pauseButton->width() << ui->closeButton->width() << ui->skipButton->width();
    }
    else
    {
        ui->lockButton->setVisible(true);
        ui->pauseButton->setVisible(true);
        ui->closeButton->setVisible(true);
        ui->skipButton->setVisible(true);
        ui->ctdnLabel->adjustSize();
        showRect.setWidth(5 * 6 + ui->ctdnLabel->width() + ui->lockButton->width() + ui->pauseButton->width() + ui->closeButton->width() + ui->skipButton->width());
        qDebug() << "width:" << showRect.width() << ui->ctdnLabel->width() << ui->lockButton->width() << ui->pauseButton->width() << ui->closeButton->width() << ui->skipButton->width();
    }
    myTimer->setCtdnTime(items["Wh"].toInt() * 3600 + items["Wm"].toInt() * 60 + items["Ws"].toInt());
    myTimer->setRestTime(items["Rh"].toInt() * 3600 + items["Rm"].toInt() * 60 + items["Rs"].toInt());
    myTimer->setForceLock(items["isForceLock"].toBool());
    this->setFixedWidth(showRect.width()); // resize() dosen't work there.
    ui->centralwidget->setFixedWidth(showRect.width()); // resize() dosen't work there.
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) // used for force lock
{
    MSG* winMsg = static_cast<MSG *>(message);
    if(winMsg->message == WM_WTSSESSION_CHANGE)
    {
        // when the computer is turned into sleep mode, it will receive the WTS_SESSION_LOCK too.
        if(winMsg->wParam == WTS_SESSION_LOCK)
        {
            emit lockStateChanged(true);
        }
        else if(winMsg->wParam == WTS_SESSION_UNLOCK)
        {
            emit lockStateChanged(false);
        }
        qDebug() << "lockState:" << (winMsg->wParam == WTS_SESSION_LOCK);
    }
//    else if(winMsg->message == WM_DISPLAYCHANGE) //
//    {
//        qDebug() << "WM_DISPLAYCHANGE";
//        QApplication::processEvents();
//        repaint();
//    }
    return false;
}

void MainWindow::on_skipButton_clicked()
{
    emit skip();
}
