#pragma once

#include <chrono>

namespace rg {
    class ExcelEditor;

    namespace database {
        class DataBaseManager;
    }
}


struct ReportOptions {
    bool writeToFile = false;
};

struct Report {
    std::string date;
    std::string startTime;
    std::string endTime;
    std::string duration;
    std::string message;

    std::string filePath;
};

class ReportsManager {
public:
    ReportsManager();
    ~ReportsManager();

    void startNewReport(ReportOptions&& options = {});
    Report endCurrentReport();

    [[nodiscard]] std::string formatTime(const std::chrono::system_clock::time_point& time) const;
    [[nodiscard]] std::string minutesToFormattedTime(const std::chrono::minutes& minutes) const;
    [[nodiscard]] std::string currentDate() const;

    [[nodiscard]] size_t reportsCount() const { return m_reportsCount; };
    [[nodiscard]] std::string reportsDirPath() const { return m_reportsDirPath; };

private:
    std::chrono::minutes calculateDuration() const;
    void initExcelHeaders();

    struct ExcelCellsData { // todo: it's taken from my reports, user will have to input this value later
        const std::string hospitalDaysValueCell   = "G2";
        const std::string holidayDaysValueCell    = "G5";
        const std::string summaryValueCell        = "I2";
        const std::string socialsValueCell        = "I5";
        const std::string summarySocialsValueCell = "I7";
    };

    using time_point_t = std::chrono::system_clock::time_point;

    ReportOptions m_currentReportOptions;
    std::optional<Report> m_currentReport;
    std::optional<time_point_t> m_currentReportStartTime;

    int m_reportsCount = 0;

    std::string m_reportsDirPath;
    std::unique_ptr<rg::ExcelEditor> m_excelEditor;
};
