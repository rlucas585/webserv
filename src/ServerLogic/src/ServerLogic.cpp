#include "ServerLogic.hpp"

namespace ServerLogic {

const bool CONNECTION_CLOSE = true;
const bool CONNECTION_KEEP_OPEN = false;

bool generate_response(http::Request::Result& request_result, std::string& output) {
    if (request_result.is_err()) {
        return serve_error_page(request_result.unwrap_err(), output);
    }
    http::Request request = request_result.unwrap();
    http::Response::Builder response_builder =
        http::Response::Builder().version(request.get_version()).state(http::OK_200);

    std::cout << "Request: \n" << request;
    response_builder.body("<html>\n"
                          "<h1>WELCOME TO WEBSERV</h1>\n"
                          "</html>\n");
    http::Response response = response_builder.build();
    output.append(response.to_string());

    return CONNECTION_KEEP_OPEN;
}

bool serve_error_page(http::State error, std::string& output) {
    output.clear();
    output.append("<html>\n"
                  "<head><title>");
    output.append(error); // Implicitly converts to char*
    output.append("</title></head>\n");
    output.append("<body>\n");
    output.append("<center><h1>");
    output.append(error);
    output.append("</h1></center>\n");
    output.append("<hr><center>rlucas + sverschu WebServ</center>\n");
    output.append("</body>\n");
    output.append("</html>\n\n");
    return CONNECTION_CLOSE;
}

} // namespace ServerLogic
