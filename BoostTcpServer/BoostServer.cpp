#include <boost/asio.hpp>

#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include <memory>
#include <utility>
#include <cstdlib>
#include <iostream>

#include "protobuf.pb.h"

using boost::asio::ip::tcp;

class session
    : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        //выделение памяти под сообщение
        msg = new TestTask::Messages::WrapperMessage();
        socket_.async_read_some(boost::asio::buffer(msg, max_length),
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    do_write(length);
                }
                else
                {
                    //освобождение памяти в случае неудачи
                    delete msg;
                }
            });
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(msg, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    //освобождение памяти после ответа клиенту
                    delete msg;
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    //объявление сообщения как части сессии
    TestTask::Messages::WrapperMessage* msg = nullptr;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<session>(std::move(socket))->start();
                }

        do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main()
{
    try
    {
        //считывание порта из файла
        std::ifstream file("port.dat");
        std::string port;
        getline(file, port);
        file.close();

        //запуск сервера по нужному порту
        boost::asio::io_context io_context;

        server s(io_context, std::atoi(port.c_str()));

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}