#ifndef PBKDF2_H
#define PBKDF2_H

#include <openssl/evp.h>
using namespace std;

void PBKDF2_HMAC_SHA_512(
    const char* pass,
    int passlen,
    const unsigned char* salt,
    int saltlen,
    int32_t iterations,
    uint32_t outputBytes,
    uint8_t* binResult
);

#endif