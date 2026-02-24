#pragma once

#include "Core/DataBase/DataBaseManager.h"

namespace rg {
namespace database {

const std::string REPORTS_COUNT_FIELD_NAME = "reports_count";
const std::string HAS_MONTH_REPORT_FIELD_NAME = "has_month_report";
const std::string REPORTS_PATH_FIELD_NAME = "reports_path";
const std::string TRACKING_ADDRESS_FIELD_NAME = "tracking_address";

namespace {
    const std::string DB_NAME = "reports_generator_db";
    const std::string RELATION_NAME = "data";

    const std::pair<std::string, std::string> ATTRIBUTE_NAME_DATA = { "attribute_name", "TEXT UNIQUE" };
    const std::pair<std::string, std::string> ATTRIBUTE_VALUE_DATA = { "attribute_value", "" };
}

class DataBaseManager;

class ReportsGeneratorDBWrapper final {
public:
    static std::shared_ptr<ReportsGeneratorDBWrapper> instance();

    template<SQLiteCompatible T>
    [[nodiscard]] bool setValue(const std::string& property, const T& value);

    template<SQLiteCompatible T>
    [[nodiscard]] std::optional<T> getValue(const std::string& valueName);

private:
    ReportsGeneratorDBWrapper();
    ReportsGeneratorDBWrapper(const ReportsGeneratorDBWrapper&) = delete;
    void operator=(const ReportsGeneratorDBWrapper&) = delete;

    bool initDataBase();

    inline static std::weak_ptr<ReportsGeneratorDBWrapper> s_instance;

    std::unique_ptr<DataBaseManager> m_dbManager;
};

template<SQLiteCompatible T>
bool ReportsGeneratorDBWrapper::setValue(const std::string& property, const T& value) {
    DBMultiInsertData insertRequestData;
    insertRequestData.databaseName = DB_NAME;
    insertRequestData.relationName = RELATION_NAME;
    insertRequestData.attributesNames = { ATTRIBUTE_NAME_DATA.first, ATTRIBUTE_VALUE_DATA.first };
    insertRequestData.values = { property, value };

    return m_dbManager->insertValues(insertRequestData);
}

template<SQLiteCompatible T>
std::optional<T> ReportsGeneratorDBWrapper::getValue(const std::string& valueName) {
    DBSingleSelectData selectRequestData;
    selectRequestData.databaseName = DB_NAME;
    selectRequestData.relationName = RELATION_NAME;
    selectRequestData.attributeToSelect = ATTRIBUTE_VALUE_DATA.first;
    selectRequestData.whereAttributeName = ATTRIBUTE_NAME_DATA.first;
    selectRequestData.whereAttributeValue = valueName;

    return m_dbManager->getValue<T>(selectRequestData);
}

}
}
