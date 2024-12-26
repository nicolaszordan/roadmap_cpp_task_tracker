#pragma once

#include <string>
#include <vector>

class TaskTracker {
public:
    auto        interactive_mode() -> void;
    auto        batch_mode(const std::vector<std::string>& args) -> void;
};
