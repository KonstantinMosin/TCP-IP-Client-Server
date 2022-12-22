#include "server.h"

#include <boost/asio.hpp>

#include <string> // ���������� ������
#include <fstream> // ���������� �����
#include <cstdlib>
#include <iostream>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        //���������� ����� �� �����
        std::ifstream file("port.dat");
        std::string port;
        getline(file, port);
        file.close();

        //������ ������� �� ������� �����
        boost::asio::io_context io_context;

        Server s(io_context, std::atoi(port.c_str()));

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}