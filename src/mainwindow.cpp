#include "mainwindow.h"

#include <origin/common/constants/date_time_constants.h>
#include <origin/common/types/date_time_types.h>
#include <origin/utils/date_time_utils.h>
#include <qlocale.h>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qvariant.h>
#include <qwidget.h>

#include <QDateTime>
#include <QLocale>
#include <QTimer>
#include <QVBoxLayout>
#include <cstdint>
#include <origin/logging/log_source.hpp>
#include <origin/logging/logging.hpp>

using namespace origin;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      //   m_loc(QLocale::Chinese, QLocale::China),
      m_prevTime(date_time::get_now_timestamp_ms()),
      m_logger(logging::get_logger("Heartbeat"))
{
    setWindowTitle("Timer");
    setFixedSize(120, 60);

    // ---------- 中心部件 + 布局（自适应窗口大小） ----------
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_timeButton = new QPushButton("time button", central);
    m_timeButton->setGeometry(0, 0, 120, 30);
    m_timeButton->setStyleSheet("color: red;");

    m_dateButton = new QPushButton("date button", central);
    m_dateButton->setGeometry(0, 0, 120, 30);

    connect(m_timeButton, &QPushButton::clicked, this, &MainWindow::on_time_button_clicked);
    layout->addWidget(m_dateButton);
    layout->addWidget(m_timeButton);

    update_button();

    m_updateTimer = new QTimer(this);  // this作为父，自动释放
    m_updateTimer->setInterval(m_refreshInterval);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::update_button);
    m_updateTimer->start();

    m_heartTimer = new QTimer(this);  // this作为父，自动释放
    m_heartTimer->setInterval(m_heartInterval);
    connect(m_heartTimer, &QTimer::timeout, this, &MainWindow::heart_timer_timeout);
    m_heartTimer->start();

    logging::info(LOG_SRC_LOCAL, "Start.");
    m_logger->info(LOG_SRC_LOCAL, "Start.");
    //   ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // delete ui;
    m_updateTimer->stop();
    m_heartTimer->stop();
    logging::info(LOG_SRC_LOCAL, "Stop.");
    m_logger->info(LOG_SRC_LOCAL, "Stop.");
}

void MainWindow::on_time_button_clicked()
{
    m_flag = !m_flag;
    if (m_flag) {
        logging::info(LOG_SRC_LOCAL, "Push button red.");
        m_timeButton->setStyleSheet("color: red;");
    } else {
        logging::info(LOG_SRC_LOCAL, "Push button green.");
        m_timeButton->setStyleSheet("color: green;");
    }
}

void MainWindow::update_button()
{
    auto now = QDateTime::currentDateTime();
    m_timeButton->setText(m_loc.toString(now, "HH:mm:ss"));
    m_dateButton->setText(m_loc.toString(now, "yyyy-MM-dd ddd"));
}

void MainWindow::heart_timer_timeout()
{
    const TimestampMs now = date_time::get_now_timestamp_ms();

    const DurationMs duration = date_time::diff_timestamp(now, m_prevTime);
    int64_t rem = duration;
    const int64_t days = rem / date_time::MILLIS_PER_DAY;
    rem = rem % date_time::MILLIS_PER_DAY;
    const int64_t hours = rem / date_time::MILLIS_PER_HOUR;
    rem = rem % date_time::MILLIS_PER_HOUR;
    const int64_t minutes = rem / date_time::MILLIS_PER_MINUTE;
    rem = rem % date_time::MILLIS_PER_MINUTE;
    const int64_t seconds = rem / date_time::MILLIS_PER_SEC;
    const int64_t millis = rem % date_time::MILLIS_PER_SEC;

    if (days > 0) {
        m_logger->info(LOG_SRC_LOCAL,
                       "Timeout. Interval: [{}-{:02}:{:02}:{:02}.{:03}]",
                       days,
                       hours,
                       minutes,
                       seconds,
                       millis);
    } else {
        m_logger->info(LOG_SRC_LOCAL,
                       "Timeout. Interval: [{:02}:{:02}:{:02}.{:03}]",
                       hours,
                       minutes,
                       seconds,
                       millis);
    }
    m_prevTime = now;
}
