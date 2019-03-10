//
// Created by Kirill on 2019-02-27.
//

#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H


#include <string>
#include <unistd.h>

namespace utils {
    class logger {
        std::string storage;
        bool store_flag;
        int fd; //no ownership
    public:
        ~logger() = default;

        logger();

        explicit logger(int fd);

        logger(const logger &log) = delete;

        logger &operator=(const logger &log) = delete;

        logger(logger &&log) noexcept;

        logger &operator=(logger &&log) noexcept;

        void add_log_entity(const char *str);

        void add_log_entity(const char *str, size_t size);

        void add_log_entity(const std::string &str);

        std::string get_log_copy() const;

        bool is_stored() const noexcept;

        bool is_empty() const noexcept;

        void store() noexcept;

        void do_not_store() noexcept;

        void set_log_point(int fd) noexcept;

        void erase();
    };

    extern utils::logger log;
}



#define F_NEW_LOG_ENTRY [func_name=__PRETTY_FUNCTION__](const std::string& msg) -> void{\
                                utils::log.add_log_entity(std::string("pid " + std::to_string(getpid()) +" logging: ") +\
                                std::string("in ") +  std::string(func_name) + std::string(": ")+ \
                                msg + std::string("\n"));}


/*#define T_NEW_LOG_ENTRY [this, func_name=__PRETTY_FUNCTION__](const std::string& msg) -> void{ \
                                        utils::log.add_log_entity(std::to_string(((void *)(this))) + " logging: " + \
                                        "in " +  func_name + " " + \
                                        msg + "\n");}
*/
#endif //LOGGER_LOGGER_H
