#include "func.h"

string convert_lu_to_string(size_t value) {
    char buffer[4];
    sprintf(buffer, "%04lu", value);
    return string(buffer);
}

string fast_response() {
    using namespace std::chrono;
    
    string sample = "YYYYMMDDThhmmss";
    
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    
    char buffer[sizeof(sample)];
    time_t now = ms.count() / 1000;
    strftime(buffer, sizeof(buffer), "%Y%m%dT%H%M%S", gmtime(&now));
    
    string response = string(buffer) + "." + std::to_string(ms.count() % 1000);

    WrapperMessage * message = new WrapperMessage();
    message->mutable_fast_response()->set_current_date_time(response.c_str());
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