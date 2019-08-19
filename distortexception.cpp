#include "distortexception.h"

DistortException::DistortException(long rc, std::string e) :
    error(e), responseCode(rc)
{
}

const char* DistortException::what() const noexcept
{
    return error.c_str();
}

long DistortException::getResponseCode() const
{
    return responseCode;
}
