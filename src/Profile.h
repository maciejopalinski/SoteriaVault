#ifndef PROFILE_H
#define PROFILE_H

#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <openssl/aes.h>
#include <openssl/rand.h>
using namespace std;

#include "Version.h"
#include "PBKDF2.h"
#include "Utils.h"

class Profile
{
    private:
        string filename;

        bool loaded = false;
        bool authenticated = false;

        uint8_t salt[512/8];
        uint8_t iv[128/8];
        uint8_t encrypted_key1[256/8];
        uint8_t key1[256/8];
        uint8_t key2[256/8];
        uint8_t key3[256/8];
        
        uint8_t key2_pbkdf2[256/8];
        uint8_t key3_pbkdf2[256/8];

        size_t data_size; // in bytes
        uint8_t* data;
        uint8_t* encrypted_data;

        void pbkdf2(string password);
        void encryptFileKey();
        bool decryptFileKey(string password);

    public:
        string getFilename();
        void setFilename(string filename);
        bool loadFromFile();
        bool saveToFile();

        bool authenticate(string password);

        void setData(char const* data, size_t size);
        void setData(string data);
        uint8_t* getData();
        size_t getDataSize();

        void encryptData(); // after setData()
        void decryptData(); // after loadFromFile()

        void setup(string password);

        bool isAuthenticated();

        void debug();
};

#endif