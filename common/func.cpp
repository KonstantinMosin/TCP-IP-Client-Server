#include "func.h"

string convert_lu_to_string(size_t value) {
    char buffer[5];
    snprintf(buffer, 5, "%04lu", value);
    return string(buffer);
}

string get_time() {
    using namespace std::chrono;

    time_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    time_t s = ms / 1000;
    
    auto tm = *std::localtime(&s);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%dT%H%M%S");

    char buffer[4];
    snprintf(buffer, 4, "%03ld", ms % 1000);

    return oss.str() + "." + string(buffer);
}

string fast_response() {
    string response;
    WrapperMessage * message = new WrapperMessage();
    message->mutable_fast_response()->set_current_date_time(get_time().c_str());
    message->SerializeToString(&response);
    delete message;
    return convert_lu_to_string(response.size()) + response;
}

string slow_response(size_t value) {
    string response;
    WrapperMessage * message = new WrapperMessage();
    message->mutable_slow_response()->set_connected_client_count(value);
    message->SerializeToString(&response);
    delete message;
    return convert_lu_to_string(response.size()) + response;
}
