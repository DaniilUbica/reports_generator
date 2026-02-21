#pragma once

#include <map>
#include <vector>
#include <variant>
#include <string>
#include <type_traits>
#include <concepts>

namespace rg {
namespace database {

template<typename T>
concept SQLiteCompatible = std::is_same_v<T, int> ||
                          std::is_same_v<T, double> ||
                          std::is_same_v<T, std::string> ||
                          std::is_same_v<T, const char*> ||
                          std::is_same_v<T, std::nullptr_t> ||
                          std::is_same_v<T, long> ||
                          std::is_same_v<T, long long> ||
                          std::is_same_v<T, float>;

using SQLiteValueVariant = std::variant<
    int,
    double,
    std::string,
    const char*,
    std::nullptr_t,
    long,
    long long,
    float
>;

enum class DBRequestType {
    NONE = -1,
    CREATE_RELATION = 0,
    INSERT,
    INSERT_OR_REPLACE,
    SELECT
};

struct DBRequestData {
    std::string databaseName = "";
    std::string relationName = "";

    virtual ~DBRequestData() {};
};

struct DBCreateRelationData : DBRequestData {
    std::map<std::string, std::string> attributes;
};

template<SQLiteCompatible FieldType>
struct DBSingleInsertData : DBRequestData {
    std::string attributeName;
    FieldType value;
};

struct DBMultiInsertData : DBRequestData {
    std::vector<std::string> attributesNames;
    std::vector<SQLiteValueVariant> values;
};

struct DBSingleSelectData : DBRequestData {
    std::string attributeToSelect;
    std::string whereAttributeName = "";
    std::string whereAttributeValue = "";
};

struct DBMultiSelectData : DBRequestData {
    std::vector<std::string> attributesToSelect;
    std::string whereClause = "";
    bool selectAll = false;
};

}
}
