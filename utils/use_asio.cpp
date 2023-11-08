#include <boost/asio/as_tuple.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <fmt/core.h>


using namespace boost::asio::experimental::awaitable_operators;
constexpr auto use_nothrow_awatable = boost::asio::as_tuple(boost::asio::use_awaitable);

using namespace std::chrono_literals;
using namespace std::string_literals;

boost::asio::io_context ioctx;

boost::asio::steady_timer timer {ioctx.get_executor()};

auto
run() -> boost::asio::awaitable<void> {
    auto timeout = 10ms;
    auto hostname = "ftx.com"s;
    auto port = "https"s;

    auto error = boost::system::error_code {};
    auto results = boost::asio::ip::tcp::resolver ::results_type {};

    auto exec = co_await boost::asio::this_coro::executor;

    auto resolver = boost::asio::ip::tcp::resolver {exec};
    auto timer = boost::asio::steady_timer {exec};
    timer.expires_after(timeout);

    auto stop_timer = boost::asio::steady_timer {exec};
    stop_timer.expires_after(2ms);
    stop_timer.async_wait([&](boost::system::error_code /*unused*/) {
        timer.cancel();
        resolver.cancel();
    });

    auto result = co_await (
        resolver.async_resolve(hostname, port, use_nothrow_awatable) || timer.async_wait(use_nothrow_awatable));
    if (result.index() == 1) {
        auto [ec] = std::get<1>(result);
        fmt::print("timer error: {}\n", ec.message());
    } else {
        auto [ec, results] = std::get<0>(result);
        fmt::print("resolve error: {}\n", ec.message());
        for (const auto& result : results) {

            fmt::print(
                "{}, {}, {}\n", result.service_name(), result.endpoint().address().to_string(), result.host_name());
        }
    }

    // // Use the timer cv as an asynchronous condition variable.
    // auto cv = boost::asio::steady_timer {exec};
    // cv.expires_at(boost::asio::steady_timer::clock_type::time_point::max());
    // bool pending_timer = true;
    // bool pending_resolve = true;
    // resolver.async_resolve(hostname, port, [&](error_code ec, boost::asio::ip::tcp::resolver::results_type rslts) {
    //     timer.cancel();
    //     if (pending_timer) {
    //         error = ec;
    //         results = std::move(rslts);
    //     }
    //     pending_resolve = false;
    //     cv.cancel_one();
    // });
    // timer.async_wait([&](error_code /*unused*/) {
    //     resolver.cancel();
    //     if (pending_resolve) {
    //         error = boost::asio::error::timed_out;
    //     }
    //     pending_timer = false;
    //     cv.cancel_one();
    // });

    // while (pending_timer || pending_resolve) {
    //     error_code ec;
    //     co_await cv.async_wait(boost::asio::redirect_error(boost::asio::use_awaitable, ec));
    // }

    // fmt::print("error: {}\n", error.message());
    // for (const auto& result : results) {
    //     fmt::print("{}, {}, {}\n", result.service_name(), result.endpoint(), result.host_name());
    // }

    co_return;
}

auto
main(int argc, char const* argv[]) -> int {

    /// run app
    boost::asio::co_spawn(ioctx.get_executor(), run(), boost::asio::detached);
    ioctx.run();

    return 0;
}
