#include "origin/logging/log_source.hpp"
#include "origin/logging/logging.hpp"
#include "origin/utils/process_utils.h"
#include "origin/logging/loggers/sync_logger.hpp"
#include "origin/logging/sinks/basic_file_sink.hpp"
#include "origin/common/debug/debug_logger.h"

using namespace origin;

int main()
{
    origin_set_debug_logger_level(ORG_DBG_LVL_TRACE);
    const auto logger = logging::create_logger<logging::SyncLogger, logging::BasicFileSink>("main");
    logging::set_root_logger(logger);
    error(LOG_SRC_LOCAL, process::get_proc_path());
    return 0;
}
