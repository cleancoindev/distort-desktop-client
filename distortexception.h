#ifndef DISTORTEXCEPTION_H
#define DISTORTEXCEPTION_H

#include <exception>
#include <string>

class DistortException : public std::exception
{
public:
    DistortException(long responseCode, std::string error);

    const char* what() const noexcept;
    long getResponseCode() const;

private:
    std::string error;
    long responseCode;
};

#endif // DISTORTEXCEPTION_H
