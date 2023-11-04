#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>


auto
main(int argc, const char* argv[]) -> int {

    auto const logger = spdlog::daily_logger_format_st(
        "ftx-odc", fmt::format("{}/{}", "./data", "ftx-odc-%Y-%m-%d-%H-%M-%S.log"), 0, 0, true);
    // auto logger = spdlog::basic_logger_mt("qt", "logs/log.txt", true);
    // auto const logger = spdlog::daily_logger_format_st("qt", "./logs/qt-%Y-%m-%d-%H-%M-%S.log", 0, 0, true);
    // auto const logger = spdlog::daily_logger_st("qt", "./logs/qt.log", 0, 0, true);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l][%n][%s:%!:%#] %v");

    const auto* astring = "This is a string";

    spdlog::info("???");

    // SPDLOG_INFO("{}", astring);

    return 0;
}
