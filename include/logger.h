#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <string>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
using namespace boost::log::trivial;
using boost::asio::ip::tcp;

class ServerLogger {
  public:
    ServerLogger();
    static ServerLogger *server_logger;
    static ServerLogger *get_server_logger();
    
    src::severity_logger<severity_level> sev_logger;
    
    // Called when logging info about each request
    // received by the server
    void log_request(tcp::socket &socket);

    // Called when logging any info about the server
    // Severity level goes from best -> worst:
    // trace, debug, info, warning, error, fatal
    void log_trace(std::string message);
    void log_debug(std::string message);
    void log_info(std::string message);
    void log_warning(std::string message);
    void log_error(std::string message);
    void log_fatal(std::string message);

    // trivial logging -- no longer needed
    // static void log_trivial_trace();
    // static void log_trivial_debug();
    // static void log_trivial_info();
    // static void log_trivial_warning();
    // static void log_trivial_error();
    // static void log_trivial_fatal();

};
