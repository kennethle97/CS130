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

#include "logger.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace boost::log::trivial;
using boost::asio::ip::tcp;

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
    keywords::file_name = "../logs/trace_%N.log",
    keywords::rotation_size = 10 * 1024 * 1024,
    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
    keywords::format = "[%TimeStamp%]:[%ThreadID%]:[%Severity%]: %Message%",
    keywords::auto_flush = true
  );
  logging::add_common_attributes();
  logging::record rec = sev_logger.open_record();
  logging::add_console_log (
    std::cout,
    keywords::format = "[%TimeStamp%]:[%ThreadID%]:[%Severity%]: %Message%"
  );
}

// Called when logging info about each request
// received by the server
// TODO: implement after request handling 
void ServerLogger::log_request(tcp::socket &socket) {
  std::stringstream sstream;

  std::string remote_address = socket.remote_endpoint().address().to_string();
  sstream << "IP Address: " << remote_address;
  BOOST_LOG_SEV(sev_logger, trace) << sstream.str();
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

// trivial logging -- no longer needed
// void ServerLogger::log_trivial_trace() {
//   BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
// }

// void ServerLogger::log_trivial_debug() {
//   BOOST_LOG_TRIVIAL(debug) << "A debug severity message";
// }

// void ServerLogger::log_trivial_info() {
//   BOOST_LOG_TRIVIAL(info) << "An informational severity message";
// }

// void ServerLogger::log_trivial_warning() {
//   BOOST_LOG_TRIVIAL(warning) << "A warning severity message";
// }

// void ServerLogger::log_trivial_error() {
//   BOOST_LOG_TRIVIAL(error) << "An error severity message";
// }

// void ServerLogger::log_trivial_fatal() {
//   BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";
// }
