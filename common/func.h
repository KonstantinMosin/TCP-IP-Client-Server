#ifndef FUNC
#define FUNC

#include <string>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include "message.pb.h"

using std::string;
using TestTask::Messages::WrapperMessage;
using TestTask::Messages::SlowResponse;

string convert_lu_to_string(size_t value);
string fast_response();
string slow_response(size_t value);

#endif