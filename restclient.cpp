#include "restclient.h"
#include "distortexception.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <sstream>

extern "C"
{
    #include "yasl/yasl.h"
    #include "json.yasl/json.h"
}

std::string encodeParams(const std::map<std::string, std::string>& params)
{
    std::stringstream r("");
    if(params.size() > 0)
    {
        // Set first parameter
        auto i = params.begin();
        r << cURLpp::escape(i->first) << "=" << cURLpp::escape(i->second);
        ++i;

        // Set remaining params, prepending '&'
        for (; i!=params.end(); ++i) {
            r << "&" << cURLpp::escape(i->first) << "=" << cURLpp::escape(i->second);
        }
    }
    return r.str();
}

QString RestClient::send(std::string method, std::string request, std::string postFieldBody, const AuthParams* authParams)
{
    curlpp::Easy req;
    req.setOpt<curlpp::options::Timeout>(3);
    req.setOpt(new curlpp::options::CustomRequest{method});

    req.setOpt<curlpp::options::Url>(request);

    // Set the header with auth params
    std::list<std::string> header;
    header.push_back("user-agent: distort-desktop-v0.1");
    header.push_back("Content-Type: application/x-www-form-urlencoded");
    if(authParams != nullptr)
    {
        header.push_back(("peerid: " + authParams->getPeerId()).toStdString());
        header.push_back(("accountname: " + authParams->getAccount()).toStdString());
        header.push_back(("authtoken: " + authParams->getAuthToken()).toStdString());
    }

    req.setOpt<curlpp::options::HttpHeader>(header);

    if(postFieldBody.size() > 0)
    {
        req.setOpt<curlpp::options::PostFields>(postFieldBody);
    }

    // Send request and get a result.
    std::stringstream result;
    req.setOpt(cURLpp::Options::WriteStream(&result));
    req.perform();

    long httpCode = cURLpp::Infos::ResponseCode::get(req);
    if(httpCode != 200)
    {
        throw(DistortException(httpCode, QString::fromStdString(result.str())));
    }

    return QString::fromStdString(result.str());
}

QString RestClient::sendDistortAuthRequest(std::string method, std::string requestPath, const AuthParams& authParams,
                                           const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams)
{
    // Set the URL, including any query parameters
    std::string url = authParams.getHomeserver().toStdString() + requestPath;
    if(queryParams.size() > 0)
    {
        url += "?" + encodeParams(queryParams);
    }

    return send(method, url, encodeParams(bodyParams), &authParams);
}

QString RestClient::sendDistortUnauthRequest(std::string method, std::string url, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams)
{
    // Set the URL, including any query parameters
    if(queryParams.size() > 0)
    {
        url += "?" + encodeParams(queryParams);
    }

    return send(method, url, encodeParams(bodyParams));
}

QString RestClient::sendDistortAuthGet(std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams)
{
    return sendDistortAuthRequest("GET", requestPath, authParams, queryParams, emptyParams);
}

QString RestClient::sendDistortUnauthGet(std::string url, const std::map<std::string, std::string>& queryParams)
{
    return sendDistortUnauthRequest("GET", url, queryParams, emptyParams);
}



// TODO: DELETE THIS
#include <cstring>
namespace
{
    void patch_pushcstring(YASL_State* S, const char* cstr)
    {
        size_t len = strlen(cstr) + 1;
        char* c = (char*)malloc(len);
        memcpy(c, cstr, len);

        YASL_pushszstring(S, c);
    }
}

QString RestClient::getError(QString response)
{
    // Retrieve Distort error from JSON using new YASL state
    YASL_State *S = YASL_newstate("scripts/geterror.yasl");
    if (!S) {
        std::cerr << "scripts/geterror.yasl: can't find it" << std::endl;
        return response;
    }

    // Configure state to parse JSON
    YASL_load_json(S);
    YASL_declglobal(S, "$");
    patch_pushcstring(S, response.toUtf8());
    YASL_setglobal(S, "$");

    // Execute script
    if(int status = YASL_execute(S) != YASL_MODULE_SUCCESS)
    {
        std::cerr << "scripts/geterror.yasl: module failed" << std::endl;
        return response;
    }

    // Get error message from script
    YASL_Object* errorObj = YASL_popobject(S);
    QString error = QString::fromUtf8(YASL_getstring(errorObj), YASL_getstringlen(errorObj));
    YASL_delstate(S);

    return error;
}

QString RestClient::getMessage(QString response)
{
    // Retrieve Distort message from JSON using new YASL state
    YASL_State *S = YASL_newstate("scripts/getmessage.yasl");
    if (!S) {
        std::cerr << "scripts/getmessage.yasl: can't find it" << std::endl;
        return response;
    }

    // Configure state to parse JSON
    YASL_load_json(S);
    YASL_declglobal(S, "$");
    patch_pushcstring(S, response.toUtf8());
    YASL_setglobal(S, "$");

    // Execute script
    if(int status = YASL_execute(S) != YASL_MODULE_SUCCESS)
    {
        std::cerr << "scripts/getmessage.yasl: module failed" << std::endl;
        return response;
    }

    // Get error message from script
    YASL_Object* msgObj = YASL_popobject(S);
    QString message = QString::fromUtf8(YASL_getstring(msgObj), YASL_getstringlen(msgObj));
    YASL_delstate(S);

    return message;
}
