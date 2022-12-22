#include "func.h"

string convert_lu_to_string(size_t value) {
    char buffer[5];
    snprintf(buffer, 5, "%04lu", value);
    return string(buffer);
}

string get_time() {
    using namespace std::chrono;

    int64_t ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    char buffer[sizeof("YYYYMMDDThhmmss")];
    time_t now = ms / 1000;
    strftime(buffer, sizeof(buffer), "%Y%m%dT%H%M%S", gmtime(&now));

    char converted_ms[4];
    snprintf(converted_ms, 4, "%03ld", ms % 1000);

    return string(buffer) + "." + string(converted_ms);
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
