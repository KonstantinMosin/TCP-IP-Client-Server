#include <boost/asio.hpp>

#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include <memory>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <time.h>

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
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    //массив c_str куда запишем сериализованное сообщение
                    const char* data = nullptr;
                    //выделение памяти под сообщение
                    TestTask::Messages::WrapperMessage* msg = nullptr;
                    msg = new TestTask::Messages::WrapperMessage();
                    //deSerialize для сообщения
                    msg->ParseFromString(data_);
                    //асинхронно спим перед отправкой сообщения
                    if (msg->has_request_for_slow_response())
                    {
                        google::protobuf::uint32 time_for_sleep = msg->request_for_slow_response().time_in_seconds_to_sleep();
                        Sleep(time_for_sleep * 1000);
                        //подготовка данных к отправке при request_for_slow_response
                        TestTask::Messages::SlowResponse* msgResponse = new TestTask::Messages::SlowResponse;
                        msgResponse->set_connected_client_count(0); //ноль надо будет заменить на кол-во подключенных клиентов
                        data = msgResponse->SerializePartialAsString().c_str();
                        delete msgResponse;
                    }
                    else
                    {
                        //подготовка данных к отправке при request_for_fast_response
                        TestTask::Messages::FastResponse* msgResponse = new TestTask::Messages::FastResponse;
                        //msgResponse->set_current_date_time(0); //ноль надо будет заменить на текущее время на машине в нужном формате
                        data = msgResponse->SerializePartialAsString().c_str();
                        delete msgResponse;
                    }

                    //удаление WrapperMessage (данные оттуда мы уже использовали)
                    delete msg;
                    do_write(length, data);
                }
            });
    }

    void do_write(std::size_t length, const char* data)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                {
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
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