#pragma once

#include "DataBaseRequestData.h"

#include <sqlite3.h>

namespace rg {
namespace database {

template<SQLiteCompatible FieldType>
void bindValue(sqlite3_stmt* stmt, int index, const FieldType& value);

template<SQLiteCompatible FieldType>
FieldType extractValue(sqlite3_stmt* stmt, int column);

namespace {
    void bindValueImpl(sqlite3_stmt* stmt, int index, int value) {
        sqlite3_bind_int64(stmt, index, static_cast<sqlite3_int64>(value));
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, long value) {
        sqlite3_bind_int64(stmt, index, static_cast<sqlite3_int64>(value));
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, long long value) {
        sqlite3_bind_int64(stmt, index, value);
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, double value) {
        sqlite3_bind_double(stmt, index, value);
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, float value) {
        sqlite3_bind_double(stmt, index, static_cast<double>(value));
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, const std::string& value) {
        sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, const char* value) {
        sqlite3_bind_text(stmt, index, value, -1, SQLITE_TRANSIENT);
    }

    void bindValueImpl(sqlite3_stmt* stmt, int index, std::nullptr_t) {
        sqlite3_bind_null(stmt, index);
    }
}

namespace {
    int extractValueImpl(sqlite3_stmt* stmt, int column, int) {
        return sqlite3_column_int(stmt, column);
    }

    long extractValueImpl(sqlite3_stmt* stmt, int column, long) {
        return static_cast<long>(sqlite3_column_int64(stmt, column));
    }

    long long extractValueImpl(sqlite3_stmt* stmt, int column, long long) {
        return sqlite3_column_int64(stmt, column);
    }

    double extractValueImpl(sqlite3_stmt* stmt, int column, double) {
        return sqlite3_column_double(stmt, column);
    }

    float extractValueImpl(sqlite3_stmt* stmt, int column, float) {
        return static_cast<float>(sqlite3_column_double(stmt, column));
    }

    std::string extractValueImpl(sqlite3_stmt* stmt, int column, const std::string&) {
        const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, column));
        return text ? std::string(text) : std::string();
    }

    const char* extractValueImpl(sqlite3_stmt* stmt, int column, const char*) {
        return reinterpret_cast<const char*>(sqlite3_column_text(stmt, column));
    }
}

template<SQLiteCompatible FieldType>
void bindValue(sqlite3_stmt* stmt, int index, const FieldType& value) {
    bindValueImpl(stmt, index, value);
}

template<SQLiteCompatible FieldType>
FieldType extractValue(sqlite3_stmt* stmt, int column) {
    return extractValueImpl(stmt, column, FieldType());
}

}
}
