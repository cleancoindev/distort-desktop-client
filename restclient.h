#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "authparams.h"

#include <curlpp/Easy.hpp>
#include <map>

extern "C"
{
    #include "yasl/yasl.h"
}

namespace RestClient
{
    const static std::map<std::string, std::string> emptyParams;

    // Base-case function for Distort REST operations
    QString send(std::string method, std::string request, std::string postFieldBody, const AuthParams* authParams = nullptr);

    // Create and send request to given url using specified authentication
    QString sendDistortAuthRequest(std::string method, std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams);
    QString sendDistortUnauthRequest(std::string method, std::string url, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams);
    QString sendDistortAuthGet(std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams);
    QString sendDistortUnauthGet(std::string url, const std::map<std::string, std::string>& queryParams);

    // Decode responses
    QString getError(QString response);
    QString getMessage(QString response);
};

#endif // RESTCLIENT_H
