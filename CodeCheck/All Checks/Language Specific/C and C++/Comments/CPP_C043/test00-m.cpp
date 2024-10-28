#include <cstdint>
#include <stdexcept>

/// @checkedException
class CommunicationError : public std::exception
{
    // Implementation
};

/// @checkedException
class BusError : public CommunicationError
{
    // Implementation
};

/// @checkedException
class Timeout : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
    // Implementation
};

/// @throw CommunicationError Communication error
/// @throw BusError Bus error
/// @throw Timeout On send timeout exception
void Send1( // UNDCC_Valid - All and only those checked exceptions that can be thrown are specified
    std::uint8_t* buffer,
    std::uint8_t bufferLength) noexcept(false)  
{
    // ...
    throw CommunicationError();
    // ...
    throw BusError();
    // ...
    throw Timeout("Timeout reached");
    // ...
}

/// @throw CommunicationError Communication error
void Send2(  // UNDCC_Violation - checked exceptions that can be thrown are missing from specification
    std::uint8_t* buffer,
    std::uint8_t bufferLength) noexcept(false)  
{
    // ...
    throw CommunicationError();
    // ...
    throw Timeout("Timeout reached");
    // ...
}

class MemoryPartitioningError : std::exception
{
    // Implementation
};

/// @throw CommunicationError Communication error
/// @throw BusError Bus error
/// @throw Timeout On send timeout exception
/// @throw MemoryPartitioningError Memory partitioning error prevents message
/// from being sent.
void Send3( // UNDCC_Violation - additional checked exceptions are specified
    std::uint8_t* buffer,
    std::uint8_t bufferLength) noexcept(false)  
{
    // ...
    throw CommunicationError();
    // ...
    throw Timeout("Timeout reached");
    // ...
}
