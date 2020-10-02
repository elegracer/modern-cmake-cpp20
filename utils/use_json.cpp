#include "boost/json.hpp"

#include "spdlog/spdlog.h"

auto
main(int argc, const char* argv[]) -> int {

    auto jv = boost::json::value {{"timestamp", 0.06}};

    spdlog::info("{}", boost::json::serialize(jv));

    return 0;
}
