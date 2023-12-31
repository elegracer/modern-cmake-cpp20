#include <fmt/chrono.h>
#include <fmt/core.h>


auto
main(int argc, const char* argv[]) -> int {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = time_point_cast<milliseconds>(now);

    fmt::print(
        "{:%Y-%m-%dT%H:%M:%S}\n"
        "{:%H:%M:%S}\n",
        now, time.time_since_epoch());

    fmt::print("{:%Y-%m-%dT}{:%H:%M:%S}\n", fmt::gmtime(time), time.time_since_epoch());

    return 0;
}
