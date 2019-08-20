#ifndef AUTHPARAMS_H
#define AUTHPARAMS_H

#include <string>

class AuthParams
{
public:
    AuthParams(std::string homeserver, std::string peerId, std::string account, std::string authToken);

    std::string getHomeserver() const;
    std::string getPeerId() const;
    std::string getAccount() const;
    std::string getAuthToken() const;

    void setHomeserver(std::string homeserver);
    void setPeerId(std::string peerId);
    void setAccount(std::string account);
    void setAuthToken(std::string authToken);

private:
    std::string homeserver;
    std::string peerId;
    std::string account;
    std::string authToken;
};

#endif // AUTHPARAMS_H
