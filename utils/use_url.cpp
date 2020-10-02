#include "boost/url.hpp"
#include "boost/url/pct_string_view.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <string>
using namespace std::string_literals;

#include <vector>

auto
main(int argc, const char* argv[]) -> int {

    auto url = "https://www.example.com/path/to/file.txt?id=1001&name=John%20Doe&results=full"s;

    auto uv_result = boost::urls::parse_uri(url);
    fmt::print("{}\n", fmt::streamed(uv_result));

    const auto uv = uv_result.value();

    fmt::print("{}\n", std::string{uv.scheme()});
    fmt::print("{}\n", static_cast<int>(uv.scheme_id()));
    fmt::print("{}\n", std::string{uv.encoded_authority()});

    return 0;
}
