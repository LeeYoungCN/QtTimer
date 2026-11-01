#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QLabel>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPoint>
#include <QTimer>
#include <QWidget>
#include <qtmetamacros.h>

#include <origin/common/types/date_time_types.h>
#include <origin/logging/loggers/logger.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void setupUi();
    void setTimer();
    void updateLabel();
    static void appendTimeSpan(const TimestampMs start,
                               const TimestampMs end,
                               std::string& logContent);
    static void appendInterval(const DurationMs duration, std::string& logContent);

private slots:
    void updateTimerTimeout();
    void heartbeatTimerTimeout();
    void onCustomContextMenuRequested(const QPoint& pos);

private:
    std::shared_ptr<origin::logging::Logger> m_logger{nullptr};
    TimestampMs m_prevTime{0};

    QWidget* m_card{nullptr};
    QLabel* m_labelDate{nullptr};
    QLabel* m_labelTime{nullptr};
    QLabel* m_line{nullptr};

    QTimer* m_heartbeatTimer{nullptr};
    QTimer* m_updateTimer{nullptr};

    QPoint m_offset;
    bool m_dragging = false;
};
#endif  // MAINWINDOW_H
