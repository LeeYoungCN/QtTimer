#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <origin/common/types/date_time_types.h>
#include <qobjectdefs.h>
#include <qpushbutton.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include <QLocale>
#include <QMainWindow>
#include <cstdint>
#include <memory>
#include <origin/logging/loggers/logger.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_time_button_clicked();
    void update_button();
    void heart_timer_timeout();

private:
    QLocale m_loc = QLocale(QLocale::Chinese, QLocale::China);
    TimestampMs m_prevTime{0};
    std::shared_ptr<origin::logging::Logger> m_logger{nullptr};

    QPushButton *m_dateButton;
    QPushButton *m_timeButton;

    QTimer *m_heartTimer;
    int32_t m_refreshInterval{100};

    QTimer *m_updateTimer;
    int32_t m_heartInterval{10000};

    bool m_flag{true};
};
#endif  // MAINWINDOW_H
