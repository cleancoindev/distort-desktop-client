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

YASL_Object* send(std::string method, std::string url, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams, const AuthParams* authParams = nullptr)
{
    curlpp::Easy req;
    req.setOpt<curlpp::options::Timeout>(3);
    req.setOpt(new curlpp::options::CustomRequest{method});

    // Set the URL, including any query parameters
    if(queryParams.size() > 0)
    {
        url += "?" + encodeParams(queryParams);
    }
    req.setOpt<curlpp::options::Url>(url);

    // Set the header with auth params
    std::list<std::string> header;
    header.push_back("user-agent: distort-desktop-v0.1");
    header.push_back("Content-Type: application/x-www-form-urlencoded");
    if(authParams != nullptr)
    {
        header.push_back("peerid: " + authParams->getPeerId());
        header.push_back("accountname: " + authParams->getAccount());
        header.push_back("authtoken: " + authParams->getAuthToken());
    }

    req.setOpt<curlpp::options::HttpHeader>(header);

    if(bodyParams.size() != 0)
    {
        req.setOpt<curlpp::options::PostFields>(encodeParams(bodyParams));
    }

    // Send request and get a result.
    std::stringstream result;
    req.setOpt(cURLpp::Options::WriteStream(&result));
    req.perform();

    long httpCode = cURLpp::Infos::ResponseCode::get(req);
    if(httpCode != 200)
    {
        std::cerr << "ERROR: " << httpCode << " " << result.str() << std::endl;

        // TODO: Extract error from JSON
        throw(DistortException(httpCode, result.str()));
    }

    char* s = (char *)malloc(result.str().length() + 1);
    result.str().copy(s, result.str().length());

    YASL_State *S = YASL_newstate((char *)"scripts/restclient.yasl");
    if (!S) {
        std::cerr << "can't find it" << std::endl;
    }

    YASL_load_json(S);
    YASL_declglobal(S, "$");
    YASL_pushszstring(S, s);
    YASL_setglobal(S, "$");
    if(int status = YASL_execute(S) != YASL_MODULE_SUCCESS)
    {
        std::cerr << "module failed" << std::endl;
    }
    YASL_Object* object = YASL_popobject(S);
    YASL_delstate(S);

    return object;
}

YASL_Object* RestClient::sendDistortAuthRequest(std::string method, std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams)
{
    std::string url = authParams.getHomeserver() + requestPath;
    return send(method, url, queryParams, bodyParams, &authParams);
}

YASL_Object* RestClient::sendDistortUnauthRequest(std::string method, std::string url, const std::map<std::string, std::string>& queryParams, const std::map<std::string, std::string>& bodyParams)
{
    return send(method, url, queryParams, bodyParams);
}

YASL_Object* RestClient::sendDistortAuthGet(std::string requestPath, const AuthParams& authParams, const std::map<std::string, std::string>& queryParams)
{
    return sendDistortAuthRequest("GET", requestPath, authParams, queryParams, emptyParams);
}

YASL_Object* RestClient::sendDistortUnauthGet(std::string url, const std::map<std::string, std::string>& queryParams)
{
    return sendDistortUnauthRequest("GET", url, queryParams, emptyParams);
}
