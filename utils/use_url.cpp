#include <boost/url/parse.hpp>

#include <fmt/core.h>
#include <fmt/ostream.h>

using namespace std::string_literals;

auto
main(int argc, const char* argv[]) -> int {

    const auto url = "https://www.example.com/path/to/file.txt?id=1001&name=John%20Doe&results=full"s;

    const auto uv_result = boost::urls::parse_uri(url);
    fmt::print("{}\n", fmt::streamed(uv_result));

    const auto uv = uv_result.value();

    fmt::print("{}\n", fmt::streamed(uv.scheme()));
    fmt::print("{}\n", fmt::underlying(uv.scheme_id()));
    fmt::print("{}\n", fmt::streamed(uv.encoded_authority()));

    return 0;
}
