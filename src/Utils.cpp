#include "Utils.h"

string bin_hex(uint8_t *binInput, size_t size)
{
    char hexResult[2*size+1];

    for (size_t i = 0; i < size; i++)
    {
        sprintf(hexResult + (i * 2), "%02x", 255 & binInput[i]);
    }

    string out(hexResult, sizeof(hexResult));
    return out;
}