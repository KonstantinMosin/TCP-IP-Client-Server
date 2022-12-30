#ifndef API_H
#define API_H

#include <string>
#include <chrono>
#include <stdio.h>

#include "message.pb.h"

using std::string;
using TestTask::Messages::WrapperMessage;

string convert_lu_to_string(size_t);
size_t convert_string_to_lu(string s);
string convert_wm_to_string(WrapperMessage*);

string fast_response();
string slow_response(size_t);

#endif
