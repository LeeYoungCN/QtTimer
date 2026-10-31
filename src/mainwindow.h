#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QLocale>
#include <QMainWindow>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qtconfigmacros.h>
#include <qtmetamacros.h>
#include <qwidget.h>

#include <origin/common/types/date_time_types.h>
#include <origin/logging/loggers/logger.hpp>


QT_BEGIN_NAMESPACE
    namespace Ui {
    class MainWindow;
    }
QT_END_NAMESPACE

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
