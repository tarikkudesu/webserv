#pragma once

#include "../Request/Request.hpp"

class Token
{
private:
    static unsigned long long sed;
    Request &request;
    String token;
    String tokenId;
    void decryptData();
    void generateTokenId();

public:
    void encryptData();
    bool authentified(String id);
    String getToken() const;
    String getTokenId() const;
    Token(Request &request);
};
