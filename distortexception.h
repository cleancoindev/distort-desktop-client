#ifndef DISTORTEXCEPTION_H
#define DISTORTEXCEPTION_H

#include <exception>
#include <QString>

class DistortException : public std::exception
{
public:
    DistortException(long responseCode, QString error);

    const char* what() const noexcept;
    long getResponseCode() const;

private:
    QString error;
    long responseCode;
};

#endif // DISTORTEXCEPTION_H
