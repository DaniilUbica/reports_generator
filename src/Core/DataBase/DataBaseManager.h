#pragma once

#include <sqlite3.h>

#include <map>
#include <optional>
#include <cassert>

#include "DataBaseRequestData.h"

#include "sqlite3_pointer.h"
#include "sqlite3_helpers.h"

#include "Core/cpplib/out_ptr.hpp"

namespace rg {
namespace database {

namespace {
    using multi_select_return_t = std::optional<std::map<std::string, SQLiteValueVariant>>;
}

const std::string UNSUPPORTED_TYPE = "UNSUPPORTED_TYPE";

class DataBaseManager {
public:
    DataBaseManager();
    ~DataBaseManager();

    [[nodiscard]] bool openDatabase(const std::string& dbName);
    [[nodiscard]] bool createRelation(const DBCreateRelationData& data);

    template<SQLiteCompatible FieldType>
    [[nodiscard]] bool insertValue(const DBSingleInsertData<FieldType>& data);
    [[nodiscard]] bool insertValues(const DBMultiInsertData& data);

    template<SQLiteCompatible FieldType>
    [[nodiscard]] std::optional<FieldType> getValue(const DBSingleSelectData& data);
    [[nodiscard]] multi_select_return_t getValues(const DBMultiSelectData& data);

private:
    sqlite3_pointer<sqlite3> m_db;
};

template<SQLiteCompatible FieldType>
[[nodiscard]] bool DataBaseManager::insertValue(const DBSingleInsertData<FieldType>& data) {
    if (data.relationName.empty() || data.databaseName.empty()) {
        assert(false);
        return false;
    }

    std::string sql_request = "INSERT OR REPLACE INTO " + data.relationName + " (" + data.attributeName + ") VALUES (?);";
    if (!data.whereAttributeName.empty() && !data.whereAttributeValue.empty()) {
        sql_request += " WHERE " + data.whereAttributeName + " = '" + data.whereAttributeValue + '\'';
    }
    sql_request += ';';

    sqlite3_pointer<sqlite3_stmt> stmt;
    const auto rc = sqlite3_prepare_v2(m_db.get(), sql_request.c_str(), -1, cpplib::out_ptr(stmt), nullptr);
    if (rc != SQLITE_OK) {
        assert(false);
        return false;
    }

    bindValue(stmt.get(), 1, data.value);

    return sqlite3_step(stmt.get()) == SQLITE_DONE;
}

template<SQLiteCompatible FieldType>
[[nodiscard]] std::optional<FieldType> DataBaseManager::getValue(const DBSingleSelectData& data) {
    std::optional<FieldType> result = std::nullopt;
    if (std::is_same_v<FieldType, std::nullptr_t> || data.databaseName.empty() || data.relationName.empty()) {
        assert(false);
        return result;
    }

    std::string sql_request = "SELECT " + data.attributeToSelect + " FROM " + data.relationName;
    if (!data.whereAttributeName.empty() && !data.whereAttributeValue.empty()) {
        sql_request += " WHERE " + data.whereAttributeName + " = '" + data.whereAttributeValue + '\'';
    }
    sql_request += ';';

    sqlite3_pointer<sqlite3_stmt> stmt;
    const auto rc = sqlite3_prepare_v2(m_db.get(), sql_request.c_str(), -1, cpplib::out_ptr(stmt), nullptr);

    if (rc != SQLITE_OK) {
        assert(false);
        return result;
    }

    if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        result = extractValue<FieldType>(stmt.get(), 0);
    }

    return result;
}

}
}
