#include "core.h"

#include "version.h"

namespace proj {

auto
getProjectVersionString() -> std::string {
    return PROJ_VERSION_STRING;
}

} // namespace proj
