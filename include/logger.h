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

// See logger.cc for implementation details
class ServerLogger {
  public:
    ServerLogger();
    static ServerLogger *server_logger;
    static ServerLogger *get_server_logger();
    
    // severity level logger
    src::severity_logger<severity_level> sev_logger;
    
    // Called when logging an error message
    // std::string add_line_num(int line_num, std::string message);
    
    // Called when logging info about each request
    // received by the server
    void log_request(request http_request, tcp::socket &socket);

    // Called when logging any info about the server
    // Severity level goes from best -> worst:
    // trace, debug, info, warning, error, fatal
    void log_trace(std::string message);
    void log_debug(std::string message);
    void log_info(std::string message);
    void log_warning(std::string message);
    void log_error(std::string message);
    void log_fatal(std::string message);

};
