

#include "asio.hpp"
#include <iostream>
#include <atomic>
#include <set>

using asio::ip::tcp;

class session;
using share_session_ptr = std::shared_ptr<session>;
class session_set
{
public:
    void join(share_session_ptr session_) {
        std::lock_guard<std::mutex> lock_guard(mut);
        session_set_.insert(session_);
    }

    void leave(share_session_ptr session_) {
        std::lock_guard<std::mutex> lock_guard(mut);
        session_set_.erase(session_);
    }

    share_session_ptr get_session() {
        share_session_ptr cur_session;

        {
            std::lock_guard<std::mutex> lock_guard(mut);
            if (session_set_.size() == 0){
                cur_session = nullptr;
            }
            else {
                cur_session = *session_set_.begin();
            }
        }

        return cur_session;
    }

private:
    std::mutex mut;
    std::set<share_session_ptr> session_set_;
};

class session
: public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket, session_set& my_set) :
        socket_(std::move(socket)),
        set_(my_set) {
    }

    void start()
    {
        set_.join(shared_from_this());
        do_read();
    }

    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, max_length), [this, self](std::error_code ec, std::size_t length) {
            if (!ec) {
                //do_write(length);
                data_[length] = 0;
                std::cout<<data_<<"\n";
                do_write(length);  
                do_read();
            } else {
                set_.leave(shared_from_this());
                std::cout<<ec<<"\n";
            }
        });
    }

    void do_write(char *pdate, std::size_t length)
    {
        auto self(shared_from_this());

        asio::async_write(socket_, asio::buffer(pdate, length), [this, self](std::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                //do write callback
            } else {
                set_.leave(shared_from_this());
            }
        });
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(data_, length), [this, self](std::error_code ec, std::size_t /*length*/) {
            if(!ec) {
                //do write callback
            } else {
                set_.leave(shared_from_this());
            }
        });
    }

private:
    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length] = {0};
    session_set& set_;
};

class server
{
public:
    server(const server&) = delete;
    server& operator=(const server&) = delete;
    session_set session_set_;

    explicit server()
    :io_context_(1),
    acceptor_(io_context_) {
    }

    void run()
    {
        io_context_.run();
    }

    void init(const std::string& address, const std::string& port)
    {
        // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
        asio::ip::tcp::resolver resolver(io_context_);
        asio::ip::tcp::endpoint endpoint =
        *resolver.resolve(address, port).begin();
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        do_accept();
    }
private:
    void do_accept()
    {
        acceptor_.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
            if (!acceptor_.is_open())
            {
                return;
            }

            if (!ec)
            {
                std::make_shared<session>(std::move(socket), session_set_)->start();
            }

            std::cout<<"accept success, start session\n";
            do_accept();
        });
    }

    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
};

server s;

void run()
{
    while(1)
    {
        auto cur_session =  s.session_set_.get_session();
        if(cur_session != nullptr)
        cur_session->do_write("hello world!\n", strlen("hello world!\n"));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

#define IP_ADDRESS  "192.168.204.1"
#define PORT        PORT

int main(void)
{
    try {
        s.init(IP_ADDRESS, "8080");
        std::thread(run).detach();
        s.run();
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}
