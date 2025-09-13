#pragma
#include <exception>
#include <string>
#include <iostream>

class ConnectionException : public std::exception
{
public:

    ConnectionException(const std::string& message)
    {
        this->m_message = "Connection Error: " + message + "\n";
    }

    virtual char const* what() const throw()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};