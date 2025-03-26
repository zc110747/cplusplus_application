/*


*/
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "asio.hpp"

#define PRINT_FUNCTION_NAME() do{std::cout << "======  " << __func__ << "  ======" << std::endl;}while(0);

// 创建endpoint
void create_endpoint(void)
{
    PRINT_FUNCTION_NAME();

    std::string raw_ip_addr = "127.0.0.1";
    unsigned short port = 8080;

    // 将原始IP地址转换为 asio::ip::address 类型
    asio::error_code ec; 
    asio::ip::address ip_addr = asio::ip::address::from_string(raw_ip_addr, ec);
    
    if (ec.value() != 0) {
        std::cout << "failed to convert the raw ip address: " << raw_ip_addr << std::endl;
        return;
    }
    std::cout << "succeed to convert the raw ip address: " << raw_ip_addr << std::endl;

    // 创建端点
    asio::ip::tcp::endpoint ep(ip_addr, port);
    std::cout << "succeed to create the endpoint: " << ep << std::endl;
}

// 创建基于IPv4 的主动TCP套接字
void create_active_socket(void)
{
    PRINT_FUNCTION_NAME();

    // 创建ASIO的I/O服务对象
    asio::io_service ios;

    // 创建TCP协议对象
    asio::ip::tcp protocol = asio::ip::tcp::v4();

    // 创建TCP套接字对象
    asio::ip::tcp::socket sock(ios);

    asio::error_code ec;

    // 打开套接字
    sock.open(protocol, ec);
    if (ec.value() != 0) {
        std::cout << "failed to create active socket" << std::endl;
        return;
    }

    std::cout << "succeed to create active socket" << std::endl;
    
    // 关闭套接字
    sock.close();
}

// 创建基于IPv6的被动TCP套接字（接受器）。
void create_passive_socket(void)
{
    PRINT_FUNCTION_NAME();

    // 创建ASIO的I/O服务对象
    asio::io_service ios;

    // 选择TCP/IPv6协议
    asio::ip::tcp protocol = asio::ip::tcp::v6();

    // 创建TCP接受器
    asio::ip::tcp::acceptor acceptor(ios);

    asio::error_code ec;

    // 打开接受器
    acceptor.open(protocol, ec);
    if (ec.value() != 0) {
        std::cout << "failed to create passive acceptor" << std::endl;
        return;
    }

    std::cout << "succeed to create passive acceptor" << std::endl;

    // 关闭接受器
    acceptor.close();
}

// 创建基于DNS的查询结果
int create_dns_query(void)
{
    PRINT_FUNCTION_NAME();

    // 定义域名和端口号 
    std::string host_name = "www.baidu.com";
    std::string port_num = "80";

    // 创建ASIO的I/O服务对象
    asio::io_service ios;

    // 创建一个 TCP 解析器的查询对象，并将其与域名和端口号关联
    asio::ip::tcp::resolver::query query(host_name, port_num, asio::ip::tcp::resolver::query::numeric_service);

    // 创建一个 TCP 解析器的对象，并将其与 I/O 服务对象关联
    asio::ip::tcp::resolver resolver(ios);

    // 解析域名并获取对应的IP地址和端口号
    asio::error_code ec;
    asio::ip::tcp::resolver::iterator iter = resolver.resolve(query, ec);
    if (ec.value()!= 0) {
        std::cout << "failed to resolve dns name: " << host_name << std::endl;
        return -1;
    }
    
    // 遍历解析结果并输出
    asio::ip::tcp::resolver::iterator it_end;
    for (auto it = iter; it != it_end; it++) {
        std::cout << "succeed to resolve dns name: " << host_name << " to ip address: " << it->endpoint() << std::endl;
    }

    return 0;
}

int connect_client_task(void)
{
    PRINT_FUNCTION_NAME();
    
    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 8080;

    // 创建endpoint端口对象
    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

        // 创建ASIO的I/O服务对象
        asio::io_service ios;
    
        asio::ip::tcp::socket sock(ios, ep.protocol());
        
        sock.connect(ep);

        sock.send(asio::buffer("hello world!"));
        std::cout << "succeed to connect server" << std::endl;

        sock.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    return 0;
}

void connect_server_task(void)
{
    PRINT_FUNCTION_NAME();

    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 8080;

    asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

    // 创建ASIO的I/O服务对象
    asio::io_service ios;

    // 创建endpoint端口对象
    try
    {
        asio::ip::tcp::acceptor acceptor(ios, ep.protocol());

        acceptor.bind(ep);

        acceptor.listen();

        asio::ip::tcp::socket sock(ios);

        while(1)
        {
            acceptor.accept(sock);

            std::cout << "succeed to connect client" << std::endl;

            std::vector<char> buf(1024);
            sock.receive(asio::buffer(buf));
            std::string recv_msg(buf.begin(), buf.end());
            std::cout << "recv msg: " << recv_msg << std::endl;

            sock.close();         
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
int main(int argc, char **argv)
{
    create_endpoint();

    create_active_socket();

    create_passive_socket();

    create_dns_query();

    std::thread t1(connect_server_task);
    t1.detach();

    do
    {
        connect_client_task();
        std::this_thread::sleep_for(std::chrono::seconds(5));
    } while (1);
    
    return 0;
}