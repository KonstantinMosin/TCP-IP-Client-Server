#include "server.h"

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket)
    : socket_(std::move(socket))
{
    //добавить увеличение количества подключенных клиентов при создании сессии
};

Session::~Session()
{
    //добавить уменьшение количества подключенных клиентов при окончании сессии
};

void Session::start()
{
    do_read();
}

void Session::time_delay(TestTask::Messages::WrapperMessage* msg)
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
                const char* data = nullptr;
                //выделение памяти под сообщение
                TestTask::Messages::WrapperMessage* msg = nullptr;
                msg = new TestTask::Messages::WrapperMessage();
                //deSerialize для сообщения
                msg->ParseFromString(data_);
                //асинхронно спим перед отправкой сообщения
                if (msg->has_request_for_slow_response())
                {
                    time_delay(msg);
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

void Session::do_write(std::size_t length, const char* data)
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

Server::Server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
    //добавить обнуление подключенных клиентов при создании сервера
    do_accept();
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket))->start();
            }

            do_accept();
        });
}
