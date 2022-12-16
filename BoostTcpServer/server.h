#pragma once

#include "protobuf.pb.h"

#include <boost/asio.hpp>
//для асинхронной задержки
#include <chrono>
#include <thread>

#include <memory>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <time.h>

using boost::asio::ip::tcp;

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono;
using std::chrono::system_clock;

class Session
    : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket);
    ~Session();
    void start();
    void time_delay(TestTask::Messages::WrapperMessage* msg);
private:
    void do_read();

    void do_write(std::size_t length, const char* data);

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server
{
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void do_accept();

    tcp::acceptor acceptor_;
};