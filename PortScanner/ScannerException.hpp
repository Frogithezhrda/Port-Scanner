#pragma
#include <exception>
#include <string>
#include <iostream>

class ScannerException : public std::exception
{
public:

    ScannerException(const std::string& message)
    {
        this->m_message = "Scanner Error: " + message + "\n";
    }

    virtual char const* what() const throw()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};