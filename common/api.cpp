#include "api.h"

string convert_lu_to_string(size_t value) {
    char buffer[5];
    snprintf(buffer, 5, "%04lu", value);
    return string(buffer);
}

size_t convert_string_to_lu(string s) {
    return std::stoul(s, nullptr, 0);
}

string convert_wm_to_string(WrapperMessage * message) {
    string response;
    message->SerializeToString(&response);
    delete message;
    return convert_lu_to_string(response.size()) + response;
}

string get_time() {
    using namespace std::chrono;

    time_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    time_t s = ms / 1000;

    std::tm * tm = std::localtime(&s);
    
    char buffer[sizeof("YYYYMMDDThhmmss.fff")];
    strftime(buffer, sizeof(buffer) - 3, "%Y%m%dT%H%M%S.", tm);
    snprintf(&buffer[16], 4, "%03ld", ms % 1000);

    return string(buffer);
}

string fast_response() {
    WrapperMessage * message = new WrapperMessage();
    message->mutable_fast_response()->set_current_date_time(get_time().c_str());
    return convert_wm_to_string(message);
}

string slow_response(size_t value) {
    WrapperMessage * message = new WrapperMessage();
    message->mutable_slow_response()->set_connected_client_count(value);
    return convert_wm_to_string(message);
}
