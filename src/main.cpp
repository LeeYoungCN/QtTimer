#include <origin/logging/log_source.hpp>
#include <origin/logging/logging.hpp>

int main()
{
    origin::logging::error(LOG_SRC_LOCAL, "Hello world!");
    return 0;
}
