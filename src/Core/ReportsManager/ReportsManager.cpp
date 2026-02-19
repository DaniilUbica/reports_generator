#include "ReportsManager.h"

#include <format>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <fstream>

const std::string REPORTS_FILE_DIR = "reports/";

ReportsManager::ReportsManager() {}

void ReportsManager::startNewReport(ReportOptions&& options) {
    if (m_currentReport) {
        assert(false);
        return;
    }

    m_currentReportOptions = std::move(options);
    m_currentReportStartTime = std::chrono::system_clock::now();
    m_currentReport = Report {
            .date = currentDate(),
            .startTime = formatTime(m_currentReportStartTime.value())
    };

    if (m_currentReportOptions.writeToFile) {
        m_currentReport->filePath = REPORTS_FILE_DIR + std::format("rep_{}.txt", m_reportsCount + 1);
    }
}

Report ReportsManager::endCurrentReport() {
    if (!m_currentReport) {
        assert(false);
        return {};
    }

    m_currentReport->endTime = formatTime(std::chrono::system_clock::now());
    m_currentReport->duration = minutesToFormattedTime(calculateDuration());

    m_reportsCount++;

    if (m_currentReportOptions.writeToFile) {
        std::ofstream reportFile(m_currentReport->filePath);

        reportFile << "Report for " << m_currentReport->date << "\n\n\n";
        reportFile << "Start time: " << m_currentReport->startTime << "\n\n";
        reportFile << "End time: " << m_currentReport->endTime << "\n\n\n\n";
        reportFile << "Done: " << m_currentReport->message;

        reportFile.close();
    }

    return std::move(m_currentReport.value());
}

std::string ReportsManager::formatTime(const std::chrono::system_clock::time_point& time) const {
    const auto time_t = std::chrono::system_clock::to_time_t(time);
    const auto localTime = std::localtime(&time_t);

    std::ostringstream oss;
    oss << std::put_time(localTime, "%H:%M");
    return oss.str();
}

std::string ReportsManager::minutesToFormattedTime(const std::chrono::minutes& minutes) const {
    const auto hours = minutes.count() / 60;
    const auto fixedMinutes = minutes.count() % 60;

    std::ostringstream oss;
    oss << hours << ":" << std::setw(2) << std::setfill('0') << fixedMinutes;
    return oss.str();
}

std::string ReportsManager::currentDate() const {
    const auto now = std::chrono::system_clock::now();
    const auto today = std::chrono::floor<std::chrono::days>(now);
    const std::chrono::year_month_day ymd{ today };

    return std::format("{:02d}.{:02d}.{:04d}", static_cast<unsigned>(ymd.day()),
                       static_cast<unsigned>(ymd.month()),
                       static_cast<int>(ymd.year()));
}

std::chrono::minutes ReportsManager::calculateDuration() const {
    if (!m_currentReportStartTime.has_value()) {
        return std::chrono::minutes(0);
    }

    const auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::minutes>(now - m_currentReportStartTime.value());
}
