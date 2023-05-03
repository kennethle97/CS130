#include "gtest/gtest.h"
#include "logger.h"
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
using namespace std;

struct LogEntry {
  std::string severity_lvl;
  std::string message;
};

class LoggerTest : public ::testing::Test {
  protected:
    ServerLogger *server_logger = ServerLogger::get_server_logger();
    std::ifstream file;
    std::string log_filename = "log/SYSLOG_0.log";
    std::string expected_severity_lvl;
    std::string expected_message;

    // Returns the last line of the log file
    LogEntry getLogEntry() {
      file.open(log_filename);

      std::vector<std::string> log_lines;
        
      std::string line = "";
      while(std::getline(file, line)) {
        log_lines.push_back(line);
      }

      file.close();

      if (log_lines.size() == 0) {
        LogEntry log_entry;
        log_entry.severity_lvl = "";
        log_entry.message = "";
        return log_entry;
      } else {
        line = log_lines.back();
      }

      std::size_t severity_lvl_start = line.find_last_of("[") + 1;
      std::size_t severity_lvl_end = line.find_last_of("]");

      std::string severity_lvl = line.substr(severity_lvl_start, severity_lvl_end - severity_lvl_start);
      std::string message = line.substr(severity_lvl_end + 3);

      LogEntry log_entry;
      log_entry.severity_lvl = severity_lvl;
      log_entry.message = message;

      return log_entry;
    }
};

TEST_F(LoggerTest, LogTrace) {
  server_logger->log_trace("Logged trace severity message");
  expected_severity_lvl = "trace";
  expected_message = "Logged trace severity message";

  LogEntry log_entry = getLogEntry();

  EXPECT_EQ(expected_severity_lvl, log_entry.severity_lvl);
  EXPECT_EQ(expected_message, log_entry.message);
}

TEST_F(LoggerTest, LogDebug) {
  server_logger->log_debug("Logged debug severity message");
  expected_severity_lvl = "debug";
  expected_message = "Logged debug severity message";

  LogEntry log_entry = getLogEntry();

  EXPECT_EQ(expected_severity_lvl, log_entry.severity_lvl);
  EXPECT_EQ(expected_message, log_entry.message);
}

TEST_F(LoggerTest, LogInfo) {
  server_logger->log_info("Logged info severity message");
  expected_severity_lvl = "info";
  expected_message = "Logged info severity message";

  LogEntry log_entry = getLogEntry();

  EXPECT_EQ(expected_severity_lvl, log_entry.severity_lvl);
  EXPECT_EQ(expected_message, log_entry.message); 
}

TEST_F(LoggerTest, LogWarning) {
  server_logger->log_warning("Logged warning severity message");
  expected_severity_lvl = "warning";
  expected_message = "Logged warning severity message";

  LogEntry log_entry = getLogEntry();

  EXPECT_EQ(expected_severity_lvl, log_entry.severity_lvl);
  EXPECT_EQ(expected_message, log_entry.message); 
}

TEST_F(LoggerTest, LogError) {
  server_logger->log_error("Logged error severity message");
  expected_severity_lvl = "error";
  expected_message = "Logged error severity message";

  LogEntry log_entry = getLogEntry();

  EXPECT_EQ(expected_severity_lvl, log_entry.severity_lvl);
  EXPECT_EQ(expected_message, log_entry.message); 
}

TEST_F(LoggerTest, LogFatal) {
  server_logger->log_fatal("Logged fatal severity message");
  expected_severity_lvl = "fatal";
  expected_message = "Logged fatal severity message";

  LogEntry log_entry = getLogEntry();

  EXPECT_EQ(expected_severity_lvl, log_entry.severity_lvl);
  EXPECT_EQ(expected_message, log_entry.message); 
}
