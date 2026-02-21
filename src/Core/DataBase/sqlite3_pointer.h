#pragma once

#include <memory>
#include <sqlite3.h>

struct sqlite3;
struct sqlite3_stmt;

namespace rg {
namespace database {

template<typename T>
constexpr inline void(*release)(T*);

template<>
constexpr inline auto release<sqlite3> = sqlite3_close;
template<>
constexpr inline auto release<sqlite3_stmt> = sqlite3_finalize;

template<typename T>
struct deleter {
    void operator()(T* target) const {
        release<T>(target);
    }
};

template<typename T>
using sqlite3_pointer = std::unique_ptr<T, deleter<T>>;

}
}
