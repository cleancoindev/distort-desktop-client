#include "distortexception.h"

DistortException::DistortException(long rc, QString e) :
    error(e), responseCode(rc)
{
}

const char* DistortException::what() const noexcept
{
    return error.toUtf8();
}

long DistortException::getResponseCode() const
{
    return responseCode;
}
