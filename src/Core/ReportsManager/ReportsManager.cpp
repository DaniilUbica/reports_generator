#include "ReportsManager.h"

#include "Core/DataBase/Wrappers/ReportsGeneratorDBWrapper.h"

#include <format>
#include <iomanip>
#include <sstream>
#include <cassert>
#include <fstream>
#include <filesystem>

const std::string REPORTS_FILE_DIR = "reports/";

ReportsManager::ReportsManager() {
    using namespace rg::database;

    const auto reportsDirPath = ReportsGeneratorDBWrapper::instance()->getValue<std::string>(REPORTS_PATH_FIELD_NAME);
    const auto reportsCount = ReportsGeneratorDBWrapper::instance()->getValue<int>(REPORTS_COUNT_FIELD_NAME);

    if (!reportsDirPath) {
        const char* home = getenv("HOME");
        m_reportsDirPath = std::string(home) + "/Documents/" + REPORTS_FILE_DIR; //todo: temp solution
    }
    else {
        m_reportsDirPath = reportsDirPath.value();
    }

    m_reportsCount = reportsCount.has_value() ? reportsCount.value() : 0;
}

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
        if (!std::filesystem::exists(m_reportsDirPath)) {
            std::filesystem::create_directory(m_reportsDirPath);
        }
        m_currentReport->filePath = m_reportsDirPath + std::format("rep_{}.txt", m_reportsCount + 1);
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
    assert(rg::database::ReportsGeneratorDBWrapper::instance()->setValue(rg::database::REPORTS_COUNT_FIELD_NAME, m_reportsCount));

    if (m_currentReportOptions.writeToFile) {
        std::ofstream reportFile(m_currentReport->filePath);

        reportFile << "Report for " << m_currentReport->date << "\n\n\n";
        reportFile << "Start time: " << m_currentReport->startTime << "\n\n";
        reportFile << "End time: " << m_currentReport->endTime << "\n\n\n\n";
        reportFile << "Done: " << m_currentReport->message;

        reportFile.close();
    }

    const auto ret = m_currentReport.value();
    m_currentReport = std::nullopt;

    return ret;
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
