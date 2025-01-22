#include "echo.hpp"

#include <print>

namespace cmds {

auto Echo::run(const std::vector<std::string>& args) -> std::expected<void, CommandError>
{
    for (const auto& arg : args) {
        std::println("{}", arg);
    }

    return {};
}

} // namespace cmds
