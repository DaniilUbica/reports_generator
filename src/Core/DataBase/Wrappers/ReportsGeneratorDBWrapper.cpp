#include "ReportsGeneratorDBWrapper.h"

using namespace rg::database;

std::shared_ptr<ReportsGeneratorDBWrapper> ReportsGeneratorDBWrapper::instance() {
    if (const auto sp = s_instance.lock()) {
        return sp;
    }

    const auto sp = std::shared_ptr<ReportsGeneratorDBWrapper>(new ReportsGeneratorDBWrapper());
    s_instance = sp;

    return sp;
}

ReportsGeneratorDBWrapper::ReportsGeneratorDBWrapper() {
    m_dbManager = std::make_unique<DataBaseManager>();

    assert(initDataBase());
}

bool ReportsGeneratorDBWrapper::initDataBase() {
    DBCreateRelationData relationRequestData;
    relationRequestData.databaseName = DB_NAME;
    relationRequestData.relationName = RELATION_NAME;
    relationRequestData.attributes = { ATTRIBUTE_NAME_DATA, ATTRIBUTE_VALUE_DATA };

    bool result = false;
    result |= m_dbManager->openDatabase(DB_NAME);
    result |= m_dbManager->createRelation(relationRequestData);

    return result;
}
