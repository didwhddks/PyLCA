#ifndef LCA_OPTIONS_HPP
#define LCA_OPTIONS_HPP
#pragma once

#include <cstddef>

enum class QueryMode {
    ONLINE, 
    OFFLINE
};

enum class Preference {
    BUILD_TIME,
    QUERY_TIME,
    MEMORY
};

struct LCAOptions {
    size_t n = 0;
    size_t q = 0;
    QueryMode mode = QueryMode::ONLINE;
    Preference pref = Preference::QUERY_TIME;
};

#endif // LCA_OPTIONS_HPP