#ifndef BOOSTTIMER_H
#define BOOSTTIMER_H

#include <boost/container/vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/timer.hpp>
#include <boost/asio.hpp>
#include <iostream>
template <typename Duration = boost::posix_time::milliseconds>
class BoostTimer
{
  public:
    BoostTimer() : work_(io_),
                   deadline_timer_(io_),
                   is_single_shot_(false)
    {
    }
    ~BoostTimer()
    {
        destroy();
    }

    void start(std::size_t duration)
    {
        if (io_.stopped())
        {
            return;
        }

        duration_ = duration;
        thread_ = boost::thread([this] { io_.run(); });
        start();
    }

    void start()
    {
        deadline_timer_.expires_from_now(Duration(duration_));
        deadline_timer_.async_wait([this](const boost::system::error_code &ec) {
            if (ec)
            {
                return;
            }
            for (auto &func : function_list_)
            {
                func();
            }

            if (!is_single_shot_)
            {
                start();
            }
        });
    }

    void stop()
    {
        deadline_timer_.cancel();
    }

    void destroy()
    {
        stop();
        io_.stop();
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

    void bind(const boost::function<void()> &func)
    {
        function_list_.emplace_back(func);
    }

    void setSingleShot(bool isSingleShot)
    {
        is_single_shot_ = isSingleShot;
    }

  private:
    boost::asio::io_service io_;
    boost::asio::io_service::work work_;
    boost::asio::deadline_timer deadline_timer_;
    // boost::function<void()> function_ = nullptr;
    boost::container::vector<boost::function<void()>> function_list_;
    boost::thread thread_;
    std::size_t duration_ = 0;
    boost::atomic<bool> is_single_shot_;
};

#endif