#include "Profile.h"

string Profile::getFilename() { return filename; }

void Profile::setFilename(string filename) { this->filename = filename; }

bool Profile::loadFromFile()
{
    FILE* file;
    
    if((file = fopen(filename.c_str(), "rb")))
    {
        string header(16, '\0');
        fread(&header[0], 1, 16, file);
        if(header != "Soteria-Vault  ") return false;
        
        fseek(file, 16*3, SEEK_SET);

        fread(encrypted_key1, 1, sizeof(encrypted_key1), file);
        fread(iv, 1, sizeof(iv), file);
        fread(salt, 1, sizeof(salt), file);
        fread(key3, 1, sizeof(key3), file);

        fread(&data_size, 8, 1, file);
        fread(&data_size, 8, 1, file);

        free(data);
        free(encrypted_data);
        data = (uint8_t*) malloc(data_size + 1);
        encrypted_data = (uint8_t*) malloc(data_size + 1);
        
        fread(encrypted_data, 1, data_size, file);

        fclose(file);
        
        loaded = true;
    }

    return loaded;
}

bool Profile::saveToFile()
{
    FILE* file;

    if((file = fopen(filename.c_str(), "wb")))
    {
        fwrite("Soteria-Vault  ", 1, 16, file);
        fwrite("Version 1.0.0  ", 1, 16, file);
        fwrite("Dont edit this!", 1, 16, file);

        fwrite(encrypted_key1, 1, sizeof(encrypted_key1), file);
        fwrite(iv, 1, sizeof(iv), file);
        fwrite(salt, 1, sizeof(salt), file);
        fwrite(key3, 1, sizeof(key3), file);

        fwrite(&data_size, 8, 1, file);
        fwrite(&data_size, 8, 1, file);

        fwrite(encrypted_data, 1, data_size, file);

        fclose(file);

        return true;
    }
    else return false;
}

bool Profile::authenticate(string password)
{
    if(decryptFileKey(password))
        authenticated = true;

    return authenticated;
}

void Profile::setData(string data)
{
    size_t required_padding = (AES_BLOCK_SIZE - (data.size() % AES_BLOCK_SIZE));
    vector<uint8_t> padded_data(data.begin(), data.end());

    for (size_t i = 0; i < required_padding; i++) padded_data.push_back(0);

    data_size = padded_data.size();

    free(this->data);
    free(this->encrypted_data);
    this->data = (uint8_t*) malloc(data_size + 1);
    this->encrypted_data = (uint8_t*) malloc(data_size + 1);

    memcpy(this->data, &padded_data[0], data_size);
}

uint8_t* Profile::getData() { return data; }

size_t Profile::getDataSize() { return data_size; }

void Profile::encryptData()
{
    uint8_t ivD[128/8];
    memcpy(ivD, iv, sizeof(iv));

    AES_KEY* aes_key = new AES_KEY();
    AES_set_encrypt_key(key1, sizeof(key1)*8, aes_key);

    AES_cbc_encrypt(data, encrypted_data, data_size, aes_key, iv, AES_ENCRYPT);

    memcpy(iv, ivD, sizeof(ivD));
}

void Profile::decryptData()
{
    uint8_t ivD[128/8];
    memcpy(ivD, iv, sizeof(iv));
    
    AES_KEY* aes_key = new AES_KEY();
    AES_set_decrypt_key(key1, sizeof(key1)*8, aes_key);

    AES_cbc_encrypt(encrypted_data, data, data_size, aes_key, iv, AES_DECRYPT);

    memcpy(iv, ivD, sizeof(ivD));
}

void Profile::setup(string password)
{
    RAND_priv_bytes(key1, sizeof(key1));
    RAND_bytes(iv, sizeof(iv));
    RAND_bytes(salt, sizeof(salt));

    pbkdf2(password);

    memcpy(key2, key2_pbkdf2, sizeof(key2));
    memcpy(key3, key3_pbkdf2, sizeof(key3));

    encryptFileKey();
}

bool Profile::isAuthenticated() { return authenticated; }

void Profile::debug()
{
    printf("\n\n");
    printf("256bit ek1: %s\n", bin_hex(encrypted_key1, sizeof(encrypted_key1)).c_str());
    printf("128bit iv: %s\n", bin_hex(iv, sizeof(iv)).c_str());
    printf("512bit salt: %s\n", bin_hex(salt, sizeof(salt)).c_str());
    printf("256bit k1: %s\n", bin_hex(key1, sizeof(key1)).c_str());
    printf("256bit k2: %s\n", bin_hex(key2, sizeof(key2)).c_str());
    printf("256bit k3: %s\n", bin_hex(key3, sizeof(key3)).c_str());
    // printf("data: %s\n", bin_hex(data, data_size).c_str());
    // printf("enc_data: %s\n", bin_hex(encrypted_data, data_size).c_str());
    printf("data_size: %i\n", (int) data_size);
}

/**
 * PRIVATE
 */

void Profile::pbkdf2(string password)
{
    uint8_t pbkdf2[sizeof(key2_pbkdf2)*2];

    PBKDF2_HMAC_SHA_512(
        password.c_str(),
        password.size(),
        salt,
        sizeof(salt),
        100000,
        sizeof(pbkdf2),
        pbkdf2
    );

    memcpy(key2_pbkdf2, pbkdf2, sizeof(key2_pbkdf2));
    memcpy(key3_pbkdf2, pbkdf2 + sizeof(key2_pbkdf2), sizeof(key3_pbkdf2));
}

void Profile::encryptFileKey()
{
    uint8_t ivD[128/8];
    memcpy(ivD, iv, sizeof(iv));
    
    AES_KEY* aes_key2 = new AES_KEY();
    AES_set_encrypt_key(key2, sizeof(key2)*8, aes_key2);

    AES_cbc_encrypt(key1, encrypted_key1, sizeof(key1), aes_key2, iv, AES_ENCRYPT);

    memcpy(iv, ivD, sizeof(ivD));
}

bool Profile::decryptFileKey(string password)
{
    pbkdf2(password);

    if(memcmp(key3_pbkdf2, key3, sizeof(key3)))
        return false;

    memcpy(key2, key2_pbkdf2, sizeof(key2));

    uint8_t ivD[128/8];
    memcpy(ivD, iv, sizeof(iv));

    AES_KEY* aes_key2 = new AES_KEY();
    AES_set_decrypt_key(key2, sizeof(key2)*8, aes_key2);

    AES_cbc_encrypt(encrypted_key1, key1, sizeof(encrypted_key1), aes_key2, iv, AES_DECRYPT);

    memcpy(iv, ivD, sizeof(ivD));

    return true;
}