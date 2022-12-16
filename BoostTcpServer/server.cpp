#include "server.h"

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket)
    : socket_(std::move(socket))
{
    //�������� ���������� ���������� ������������ �������� ��� �������� ������
};

Session::~Session()
{
    //�������� ���������� ���������� ������������ �������� ��� ��������� ������
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
                //������ c_str ���� ������� ��������������� ���������
                const char* data = nullptr;
                //��������� ������ ��� ���������
                TestTask::Messages::WrapperMessage* msg = nullptr;
                msg = new TestTask::Messages::WrapperMessage();
                //deSerialize ��� ���������
                msg->ParseFromString(data_);
                //���������� ���� ����� ��������� ���������
                if (msg->has_request_for_slow_response())
                {
                    time_delay(msg);
                    //���������� ������ � �������� ��� request_for_slow_response
                    TestTask::Messages::SlowResponse* msgResponse = new TestTask::Messages::SlowResponse;
                    msgResponse->set_connected_client_count(0); //���� ���� ����� �������� �� ���-�� ������������ ��������
                    data = msgResponse->SerializePartialAsString().c_str();
                    delete msgResponse;
                }
                else
                {
                    //���������� ������ � �������� ��� request_for_fast_response
                    TestTask::Messages::FastResponse* msgResponse = new TestTask::Messages::FastResponse;
                    //msgResponse->set_current_date_time(0); //���� ���� ����� �������� �� ������� ����� �� ������ � ������ �������
                    data = msgResponse->SerializePartialAsString().c_str();
                    delete msgResponse;
                }

                //�������� WrapperMessage (������ ������ �� ��� ������������)
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
    //�������� ��������� ������������ �������� ��� �������� �������
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
