#include <cstdio>

#include "boost-ext/sml2"

struct connect {};
struct established {};
struct ping {};
struct disconnect {};
struct timeout {};

constexpr auto establish = [] {
    std::puts("establish");
};
constexpr auto close = [] {
    std::puts("close");
};
constexpr auto is_valid = [](auto const&) {
    return true;
};
constexpr auto reset_timeout = [] {
    std::puts("reset_timeout");
};

struct Connection {
    auto
    operator()() const {
        using namespace sml::dsl;
        return transition_table {
            *"Disconnected"_s + event<connect> / establish = "Connecting"_s,
            "Connecting"_s + event<established> = "Connected"_s,
            "Connected"_s + event<ping>[is_valid] / reset_timeout,
            "Connected"_s + event<timeout> / establish = "Connecting"_s,
            "Connected"_s + event<disconnect> / close = "Disconnected"_s,
        };
    }
};

auto
main(int argc, const char* argv[]) -> int {

    sml::sm connection {Connection {}};
    connection.process_event(connect {});
    connection.process_event(established {});
    connection.process_event(ping {});
    connection.process_event(disconnect {});
    connection.process_event(connect {});
    connection.process_event(established {});
    connection.process_event(ping {});

    return 0;
}
