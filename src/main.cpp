#include <origin/common/debug/debug_logger.h>
#include <origin/utils/filesystem_utils.h>
#include <origin/utils/process_utils.h>
#include <qcontainerfwd.h>
#include <qcoreapplication.h>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <origin/logging/log_level.hpp>
#include <origin/logging/log_source.hpp>
#include <origin/logging/loggers/sync_logger.hpp>
#include <origin/logging/logging.hpp>
#include <origin/logging/sinks/basic_file_sink.hpp>
#include <origin/logging/sinks/daily_file_sink.hpp>
#include <origin/logging/sinks/rotating_file_sink.hpp>

#include "mainwindow.h"

using namespace origin;

int main(int argc, char *argv[])
{
    const auto logDir = filesystem::join_paths({process::get_proc_directory(), "logs"});

    const auto runLogFile = filesystem::join_paths({logDir, "Timer.log"});
    const auto heartbeatLogFile = filesystem::join_paths({logDir, "Heartbeat.log"});

    const auto runLogger = logging::create_logger<logging::SyncLogger, logging::RotatingFileSink>(
        "Timer", runLogFile, 100 * 1024, 5, false);
    runLogger->flush_on(logging::LogLevel::INFO);
    logging::set_root_logger(runLogger);

    const auto heartLogger = logging::create_logger<logging::SyncLogger, logging::DailyFileSink>(
        "Heartbeat", heartbeatLogFile, false);
    heartLogger->flush_on(logging::LogLevel::INFO);
    logging::register_logger(heartLogger);

    info(LOG_SRC_LOCAL, "Process: \"{}\"", process::get_proc_path());
    info(LOG_SRC_LOCAL, "Log directory: \"{}\"", logDir);
    info(LOG_SRC_LOCAL, "Run log: \"{}\"", runLogFile);
    info(LOG_SRC_LOCAL, "Heartbeat log: \"{}\"", heartbeatLogFile);

    QApplication const a(argc, argv);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
