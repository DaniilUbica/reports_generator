#include "DataBaseManager.h"

#include <sstream>

using namespace rg::database;

DataBaseManager::DataBaseManager() {}

DataBaseManager::~DataBaseManager() {}

bool DataBaseManager::openDatabase(const std::string& dbName) {
    const auto rc = sqlite3_open(dbName.c_str(), cpplib::out_ptr(m_db));
    if (rc) {
        return false;
    }

    return true;
}

bool DataBaseManager::createRelation(const DBCreateRelationData& data) {
    if (!m_db || data.databaseName.empty()) {
        return false;
    }

    std::stringstream sql_request;
    sql_request << "CREATE TABLE IF NOT EXISTS " << data.relationName << " (";

    bool firstAttribute = true;
    for (const auto& [attributeName, attributeType] : data.attributes) {
        if (!firstAttribute) {
            sql_request << ", ";
        }

        sql_request << attributeName << " " << attributeType;
        firstAttribute = false;
    }

    sql_request << ");";

    int rc = sqlite3_exec(m_db.get(), sql_request.str().c_str(), nullptr, nullptr, nullptr);

    if (rc != SQLITE_OK) {
        return false;
    }

    return true;
}

bool DataBaseManager::insertValues(const DBMultiInsertData& data) {
    if (data.relationName.empty() || data.databaseName.empty() || data.values.size() != data.attributesNames.size()) {
        assert(false);
        return false;
    }

    std::string sql_request = "INSERT OR REPLACE INTO " + data.relationName + " (";

     for (int i = 0; i < data.attributesNames.size(); ++i) {
         if (i > 0) {
             sql_request += ", ";
         }
         sql_request += data.attributesNames[i];
     }

     sql_request += ") VALUES (";

     for (int i = 0; i < data.values.size(); ++i) {
         if (i > 0) {
             sql_request += ", ";
         }
         sql_request += "?";
     }
     sql_request += ");";

     sqlite3_pointer<sqlite3_stmt> stmt;
     const auto rc = sqlite3_prepare_v2(m_db.get(), sql_request.c_str(), -1, cpplib::out_ptr(stmt), nullptr);
     if (rc != SQLITE_OK) {
         assert(false);
         return false;
     }

     for (int i = 0; i < data.values.size(); i++) {
         std::visit([&](auto&& arg) {
             bindValue(stmt.get(), i + 1, arg);
         }, data.values[i]);
     }

     return sqlite3_step(stmt.get()) == SQLITE_DONE;
}

multi_select_return_t DataBaseManager::getValues(const DBMultiSelectData& data) {
    multi_select_return_t result = std::nullopt;
    if (data.relationName.empty() || data.databaseName.empty() || (data.attributesToSelect.empty() && !data.selectAll)) {
        assert(false);
        return result;
    }

    std::string sql_request = "SELECT ";
    if (!data.selectAll) {
        for (const auto& attribute : data.attributesToSelect) {
            if (attribute == data.attributesToSelect.back()) {
                sql_request += attribute;
                break;
            }
            sql_request += attribute + ", ";
        }
    }
    else {
        sql_request += '*';
    }

    sql_request += " FROM " + data.relationName + (data.whereClause.empty() ? "" : data.whereClause) + ';';

    sqlite3_pointer<sqlite3_stmt> stmt;
    const auto rc = sqlite3_prepare_v2(m_db.get(), sql_request.c_str(), -1, cpplib::out_ptr(stmt), nullptr);

    if (rc != SQLITE_OK) {
        assert(false);
        return result;
    }

    std::map<std::string, SQLiteValueVariant> row_data;
    if (sqlite3_step(stmt.get()) == SQLITE_ROW) {
        int columnCount = sqlite3_column_count(stmt.get());

        for (int i = 0; i < columnCount; ++i) {
            const char* columnName = sqlite3_column_name(stmt.get(), i);
            if (!columnName) {
                continue;
            }

            int columnType = sqlite3_column_type(stmt.get(), i);

            switch (columnType) {
                case SQLITE_INTEGER:
                    row_data[columnName] = extractValue<long long>(stmt.get(), i);
                    break;
                case SQLITE_FLOAT:
                    row_data[columnName] = extractValue<double>(stmt.get(), i);
                    break;
                case SQLITE_TEXT:
                    row_data[columnName] = extractValue<std::string>(stmt.get(), i);
                    break;
                case SQLITE_NULL:
                    row_data[columnName] = nullptr;
                    break;
                default:
                    row_data[columnName] = UNSUPPORTED_TYPE;
                    break;
            }
        }
    }

    result = row_data;

    return result;
}
