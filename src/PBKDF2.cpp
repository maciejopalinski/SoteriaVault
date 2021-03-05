#include "PBKDF2.h"

void PBKDF2_HMAC_SHA_512(
    const char* pass,
    int passlen,
    const unsigned char* salt,
    int saltlen,
    int32_t iterations,
    uint32_t outputBytes,
    uint8_t* binResult
)
{
    PKCS5_PBKDF2_HMAC(pass, passlen, salt, saltlen, iterations, EVP_sha512(), outputBytes, binResult);
}