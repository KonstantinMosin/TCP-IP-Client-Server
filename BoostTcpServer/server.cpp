#include "server.h"

using boost::asio::ip::tcp;

size_t numClients;

Session::Session(tcp::socket socket)
    : socket_(std::move(socket))
{
};

void Session::start()
{
    do_read();
}

void time_delay(TestTask::Messages::WrapperMessage* msg)
{
    sleep_for(10ns);
    sleep_until(system_clock::now() + seconds(msg->request_for_slow_response().time_in_seconds_to_sleep()));
}

void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec)
            {
                //массив c_str куда запишем сериализованное сообщение
                string data = "";
                //выделение памяти под сообщение
                TestTask::Messages::WrapperMessage* msg = nullptr;
                msg = new TestTask::Messages::WrapperMessage();
                //deSerialize для сообщения
                msg->ParseFromString(data_);
                if (msg->has_request_for_slow_response())
                {
                    //std::cout << slow_response(numClients).data() << "\n";
                    data = slow_response(numClients);

                    //асинхронно спим перед отправкой сообщения
                    time_delay(msg);
                    //boost::thread time(time_delay, msg);
                }
                else
                {
                    //std::cout << fast_response().data() << "\n";
                    data = fast_response();
                }

                //удаление WrapperMessage (данные оттуда мы уже использовали)
                delete msg;
                do_write(data.size(), data.c_str());
            }
            else 
            {
                numClients--;
            }
        });
}

void Session::do_write(size_t length, const char * data)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/ )
        {
            if (!ec)
            {
                do_read();
            }
            else
            {
                numClients--;
            }
        });
}

Server::Server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    //обнуление подключенных клиентов при создании сервера
    numClients = 0;
    do_accept();
}

void Server::do_accept()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
    {
        if (!ec)
        {
            numClients++;
            std::make_shared<Session>(std::move(socket))->start();
        }
        do_accept();
    });
}
