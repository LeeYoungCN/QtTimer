#include "mainwindow.h"

#include <cstdint>

#include <QColor>
#include <QDateTime>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <origin/common/constants/date_time_constants.h>
#include <origin/common/types/date_time_types.h>
#include <origin/logging/log_source.hpp>
#include <origin/logging/logging.hpp>
#include <origin/utils/date_time_utils.h>


using namespace origin;

namespace {

constexpr int LABEL_W = 200;
constexpr int CARD_PAD_X = 16;
constexpr int CARD_PAD_TOP = 18;
constexpr int CARD_PAD_BOTTOM = 16;
constexpr int SHADOW_MARGIN = 28;  // 窗口四周为阴影预留的透明边距，必须 >= 模糊半径 + 偏移
constexpr int UPDATE_INTERVAL_MS = 100;
constexpr int HEARTBEAT_INTERVAL_MS = 10000;

constexpr char UI_FONT[] = "Microsoft YaHei UI";

}  // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_logger(logging::get_logger("Heartbeat")),
      m_prevTime(date_time::get_now_timestamp_ms())
{
    logging::info(LOG_SRC_LOCAL, "Start.");
    m_logger->info(LOG_SRC_LOCAL, "Start.");

    // 透明背景只是"不画窗口底色"，卡片轮廓要自己给。
    // 想让它不进任务栏 / Alt+Tab，再或上 Qt::Tool。
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setupUi();
    setTimer();
}

MainWindow::~MainWindow()
{
    if (m_updateTimer) {
        m_updateTimer->stop();
    }
    if (m_heartbeatTimer) {
        m_heartbeatTimer->stop();
    }
    logging::info(LOG_SRC_LOCAL, "Stop.");
    m_logger->info(LOG_SRC_LOCAL, "Stop.");
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_offset = event->globalPosition().toPoint() - frameGeometry().topLeft();
        m_card->setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_offset);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        m_card->setCursor(Qt::OpenHandCursor);
        event->accept();
    }
}

void MainWindow::setupUi()
{
    auto* central = new QWidget(this);
    setCentralWidget(central);

    auto* outerLayout = new QVBoxLayout(central);
    outerLayout->setContentsMargins(SHADOW_MARGIN, SHADOW_MARGIN, SHADOW_MARGIN, SHADOW_MARGIN);
    outerLayout->setSpacing(0);

    m_card = new QWidget(central);
    m_card->setObjectName("card");
    m_card->setAttribute(Qt::WA_StyledBackground, true);  // 保证 QSS 背景在普通 QWidget 上生效
    m_card->setCursor(Qt::OpenHandCursor);

    // 1px 描边定边界 + 阴影给厚度：白底上单靠任何一样都会显得"飘"
    m_card->setStyleSheet(R"(
        QWidget#card {
            background-color: #FFFFFF;
            border: 1px solid #E2E6EC;
            border-radius: 20px;
        }
    )");

    auto* shadow = new QGraphicsDropShadowEffect(m_card);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 6);
    shadow->setColor(QColor(15, 23, 42, 48));  // 约 19% 蓝黑，比纯黑柔和
    m_card->setGraphicsEffect(shadow);

    m_card->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_card,
            &QWidget::customContextMenuRequested,
            this,
            &MainWindow::onCustomContextMenuRequested);

    auto* cardLayout = new QVBoxLayout(m_card);
    cardLayout->setContentsMargins(CARD_PAD_X, CARD_PAD_TOP, CARD_PAD_X, CARD_PAD_BOTTOM);
    cardLayout->setSpacing(8);
    cardLayout->setAlignment(Qt::AlignCenter);

    // 日期：只约束宽度，高度交给布局按字体度量算，避免 30px 装不下
    m_labelDate = new QLabel(m_card);
    m_labelDate->setAlignment(Qt::AlignCenter);
    m_labelDate->setFixedWidth(LABEL_W);
    m_labelDate->setStyleSheet("QLabel { color: #5B6472; }");

    QFont dateFont(QString::fromUtf8(UI_FONT));
    dateFont.setPixelSize(19);
    dateFont.setWeight(QFont::Medium);
    m_labelDate->setFont(dateFont);
    cardLayout->addWidget(m_labelDate);

    // 时间：字体属性全部走 QFont，QSS 里只留颜色
    m_labelTime = new QLabel(m_card);
    m_labelTime->setAlignment(Qt::AlignCenter);
    m_labelTime->setFixedWidth(LABEL_W);
    m_labelTime->setStyleSheet("QLabel { color: #0F172A; }");

    QFont timeFont(QString::fromUtf8(UI_FONT));
    timeFont.setPixelSize(32);
    timeFont.setWeight(QFont::DemiBold);
    timeFont.setLetterSpacing(QFont::AbsoluteSpacing,
                              1.5);  // Qt 认这个，QSS 的 letter-spacing 不认
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
    timeFont.setFeature(QFont::Tag("tnum"), 1);  // 等宽数字，秒跳动时整行不左右抖
#endif
    m_labelTime->setFont(timeFont);
    cardLayout->addWidget(m_labelTime);

    // 红色短线
    m_line = new QLabel(m_card);
    m_line->setFixedSize(150, 3);
    m_line->setStyleSheet(R"(
        QLabel {
            background-color: #FF3B30;
            border-radius: 2px;
        }
    )");
    cardLayout->addWidget(m_line, 0, Qt::AlignHCenter);

    outerLayout->addWidget(m_card);

    updateLabel();
    adjustSize();  // 让窗口按"卡片 + 阴影边距"收缩，否则阴影会被窗口裁成硬边
}

void MainWindow::setTimer()
{
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(UPDATE_INTERVAL_MS);
    connect(m_updateTimer, &QTimer::timeout, this, &MainWindow::updateTimerTimeout);

    m_heartbeatTimer = new QTimer(this);
    m_heartbeatTimer->setInterval(HEARTBEAT_INTERVAL_MS);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &MainWindow::heartbeatTimerTimeout);

    m_updateTimer->start();
    m_heartbeatTimer->start();
}

void MainWindow::updateLabel()
{
    const QDateTime now = QDateTime::currentDateTime();

    const QString dateText = now.toString(QStringLiteral("yyyy-MM-dd ddd"));
    const QString timeText = now.toString(QStringLiteral("HH:mm:ss"));

    // 100ms 刷一次，但只在文本真的变了才 setText，避免无谓的整卡重绘（有阴影后更明显）
    if (m_labelDate->text() != dateText) {
        m_labelDate->setText(dateText);
    }
    if (m_labelTime->text() != timeText) {
        m_labelTime->setText(timeText);
    }
}

void MainWindow::onCustomContextMenuRequested(const QPoint& pos)
{
    QMenu menu(this);
    QAction const* closeAction = menu.addAction(tr("Close."));
    if (menu.exec(m_card->mapToGlobal(pos)) == closeAction) {
        logging::info(LOG_SRC_LOCAL, "Close window.");
        close();
    }
}

void MainWindow::updateTimerTimeout()
{
    updateLabel();
}

void MainWindow::heartbeatTimerTimeout()
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
