#include "ExcelEditor.h"

using namespace rg;

ExcelEditor::~ExcelEditor() {
    save();
}

bool ExcelEditor::create(const std::string& filePath) {
    try {
        m_workbook = xlnt::workbook();
        m_currentFilePath = filePath;

        m_worksheet = m_workbook.active_sheet();

        return true;
    }
    catch (const std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

bool ExcelEditor::open(const std::string& filePath) {
    try {
        m_workbook.load(filePath);
        m_currentFilePath = filePath;
        m_worksheet = m_workbook.active_sheet();

        return true;
    }
    catch (const std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

bool ExcelEditor::save() {
    try {
        m_workbook.save(m_currentFilePath);
        return true;
    }
    catch (const std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

bool ExcelEditor::setCellFormula(const std::string& cellRef, const std::string& formula) {
    try {
        m_worksheet.cell(cellRef).formula(formula);
        return true;
    }
    catch (const std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

bool ExcelEditor::setCellNumberFormat(const std::string& cellRef) {
    try {
        m_worksheet.cell(cellRef).number_format(xlnt::number_format::number());
        return true;
    }
    catch (const std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

std::string rg::ExcelEditor::createCellRef(char columnChar, int rowNum) const {
    return std::string(1, columnChar) + std::to_string(rowNum);
}

xlnt::cell_reference ExcelEditor::firstEmptyCellInColumn(char column) {
    int row = 1;

    while (true) {
        const auto cellRef = createCellRef(column, row);
        const auto cell = m_worksheet.cell(cellRef);

        if (cell.data_type() == xlnt::cell_type::empty) {
            return xlnt::cell_reference(cellRef);
        }

        if (cell.value<std::string>().empty()) {
            return xlnt::cell_reference(cellRef);
        }

        row++;
    }
}
