#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "authparams.h"

#include <curlpp/Easy.hpp>
#include <map>

namespace RestClient
{
    const static std::map<std::string, std::string> emptyParams;

    // Create and send request to given url using specified authentication
    void sendDistortAuthRequest(std::string method, std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams);
    void sendDistortUnauthRequest(std::string method, std::string url, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams);
    void sendDistortAuthGet(std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams);
    void sendDistortUnauthGet(std::string url, const std::map<std::string, std::string>& queryParams);
};

#endif // RESTCLIENT_H
