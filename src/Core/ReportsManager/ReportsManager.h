#pragma once

#include <chrono>

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

    void startNewReport(ReportOptions&& options = {});
    Report endCurrentReport();

    std::string formatTime(const std::chrono::system_clock::time_point& time) const;
    std::string minutesToFormattedTime(const std::chrono::minutes& minutes) const;
    std::string currentDate() const;

private:
    std::chrono::minutes calculateDuration() const;

    using time_point_t = std::chrono::system_clock::time_point;

    ReportOptions m_currentReportOptions;
    std::optional<Report> m_currentReport;
    std::optional<time_point_t> m_currentReportStartTime;

    size_t m_reportsCount = 0;
};
