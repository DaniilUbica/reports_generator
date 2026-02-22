#pragma once

#include <xlnt/xlnt.hpp>

namespace rg {

template<typename T>
concept ExcelCompatible = std::is_same_v<T, int> ||
                          std::is_same_v<T, unsigned int> ||
                          std::is_same_v<T, double> ||
                          std::is_same_v<T, float> ||
                          std::is_same_v<T, std::string> ||
                          std::is_same_v<T, const char*> ||
                          std::is_same_v<T, std::nullptr_t> ||
                          std::is_same_v<T, bool> ||
                          std::is_same_v<T, xlnt::time> ||
                          std::is_same_v<T, xlnt::date>;

class ExcelEditor final {
public:
    ~ExcelEditor();

    bool create(const std::string& filePath);
    bool open(const std::string& filePath);
    bool save();

    bool setCellFormula(const std::string& cellRef, const std::string& formula);
    bool setCellNumberFormat(const std::string& cellRef);

    template<ExcelCompatible T>
    bool writeToCell(const std::string& cellRef, T value);
    template<ExcelCompatible T>
    [[nodiscard]] std::optional<T> readCellValue(const std::string& cellRef) const;

    [[nodiscard]] bool hasActiveTable() const { return !m_currentFilePath.empty(); };
    [[nodiscard]] std::string lastError() const { return m_lastError; };
    [[nodiscard]] std::string createCellRef(char columnChar, int rowNum) const;

    [[nodiscard]] xlnt::cell_reference firstEmptyCellInColumn(char column);

private:
    xlnt::workbook m_workbook;
    xlnt::worksheet m_worksheet;

    std::string m_currentFilePath;
    std::string m_lastError;
};

template<ExcelCompatible T>
bool ExcelEditor::writeToCell(const std::string& cellRef, T value) {
    try {
        m_worksheet.cell(cellRef).value(value);
        return true;
    }
    catch(const std::exception& e) {
        m_lastError = e.what();
        return false;
    }
}

template<ExcelCompatible T>
std::optional<T> ExcelEditor::readCellValue(const std::string& cellRef) const {
    try {
        const auto cell = m_worksheet.cell(cellRef);
        if (cell.has_value()) {
            return cell.value<T>();
        }

        return std::nullopt;
    }
    catch (const std::exception& e) {
        return std::nullopt;
    }
}

}
