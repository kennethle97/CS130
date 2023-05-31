#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/signals2.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <string>
#include <sstream>
#include <iostream>

#include "logger.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace boost::log::trivial;
using boost::asio::ip::tcp;
using request = boost::beast::http::request<boost::beast::http::string_body>;

// initialize blank server_logger instance
ServerLogger *ServerLogger::server_logger = 0;

// Returns a static object server_loger so that
// there is only one instance of the logger for 
// the entire server
ServerLogger *ServerLogger::get_server_logger() {
  if (server_logger == 0) {
    server_logger = new ServerLogger();
  }
  return server_logger;
}

// Sets up logging options for both log file and console outputs
// - log file is rotated every 10 MB and at midnight
ServerLogger::ServerLogger() {
  logging::add_file_log (
    keywords::file_name = "log/SYSLOG_%N.log",
    keywords::rotation_size = 10 * 1024 * 1024,
    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
    keywords::format = "[%TimeStamp%]:[%ThreadID%]:[%ProcessID%]:[%Severity%]: %Message%",
    keywords::auto_flush = true
  );
  logging::add_common_attributes();
  logging::record rec = sev_logger.open_record();
  logging::add_console_log (
    std::cout,
    keywords::format = "[%TimeStamp%]:[%ThreadID%]:[%ProcessID%]:[%Severity%]: %Message%",
    keywords::auto_flush = true
  );
}

// Takes in an error message, then adds the line number of the
// server_logger logcall to the beginning of the message.
// NOTE: this DOES NOT add the line number of the error, so it's only
// somewhat useful for debugging.
// std::string ServerLogger::add_line_num(int line_num, std::string message) {
//   std::stringstream sstream;
//   sstream << "Line " << line_num << ": " << message;
//   return sstream.str();
// }

// Called when logging info about each request
// received by the server
void ServerLogger::log_request(request http_request, tcp::socket &socket) {
  std::stringstream sstream;
  sstream << "-Base:\r\n" << http_request.base();
  // get ip address of the caller
  std::string remote_address = socket.remote_endpoint().address().to_string();
  sstream << "- IP Address: " << remote_address;
  BOOST_LOG_SEV(sev_logger, trace) << sstream.str();
  BOOST_LOG_SEV(sev_logger, info) << "[IPMetrics] "+remote_address;
  BOOST_LOG_SEV(sev_logger, info) << "[RequestPathMetrics] "+ http_request.target().to_string();
  
}

// Called when logging any info about the server
// Severity level goes from best -> worst:
// trace, debug, info, warning, error, fatal
void ServerLogger::log_trace(std::string message) {
  BOOST_LOG_SEV(sev_logger, trace) << message;
}

void ServerLogger::log_debug(std::string message) {
  BOOST_LOG_SEV(sev_logger, debug) << message;
}

void ServerLogger::log_info(std::string message) {
  BOOST_LOG_SEV(sev_logger, info) << message;
}

void ServerLogger::log_warning(std::string message) {
  BOOST_LOG_SEV(sev_logger, warning) << message;
}

void ServerLogger::log_error(std::string message) {
  BOOST_LOG_SEV(sev_logger, error) << message;
}

void ServerLogger::log_fatal(std::string message) {
  BOOST_LOG_SEV(sev_logger, fatal) << message;
}
