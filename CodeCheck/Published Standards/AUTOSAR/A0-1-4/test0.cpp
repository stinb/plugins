// AUTOSAR C++ 2014

#include <type_traits>
#include <string>
#include <cstdint>

//Logger.hpp
class Logger
{
public:
    struct console_t
    {
    };
    struct file_t
    {
    };

    constexpr static console_t console = console_t();
    constexpr static file_t file = file_t();

    void init(console_t);
    void init(file_t, const std::string &prefix);
};

//Logger.cpp
void Logger::init(console_t)
{
    //initialization for a console logger
}
void Logger::init(file_t, const std::string &prefix)  // UndCC_Violation
{
    //initialization for a file logger for a given prefix path
}

//Message.h
struct MessagePolicy
{
};
struct WriteMessagePolicy final : public MessagePolicy
{
};

template <typename T>
struct is_mutable : std::false_type
{
};
template <>
struct is_mutable<WriteMessagePolicy> : std::true_type
{
};

template <typename T, typename Policy = MessagePolicy>
class Message
{
public:
    static_assert(std::is_base_of<MessagePolicy, Policy>::value == true, "Given parameter is not derived from MessagePolicy");
    using value_type = T;

    template <typename U = void>
    void set(T &&u, typename std::enable_if<is_mutable<Policy>::value, U>::type * = 0)
    {
        v = u;
    }

private:
    value_type v;
};

int main(int, char **)
{
    Logger log;
    log.init(Logger::console);
    log.init(Logger::file, std::string("/tmp/"));

    Message<uint8_t> read;
    Message<uint8_t, WriteMessagePolicy> write;

    //read.set(uint8_t(12)); Compilation error
    write.set(uint8_t(12));

    return 0;
}
