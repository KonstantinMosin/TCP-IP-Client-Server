#pragma once

#include "protobuf.pb.h"

#include <boost/asio.hpp>

#include <memory>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <time.h>

using boost::asio::ip::tcp;

class Session
    : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket);

    void start();

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