#include <openssl/sha.h>
#include <openssl/hmac.h>

#include "hmac.h"

QByteArray hmac(QByteArray secretKey, QByteArray data)
{
    QByteArray ret;
    ret.reserve(512);

    unsigned int length = 512;

    HMAC(EVP_sha512(),
         secretKey.data(),
         secretKey.length(),
         (uchar*)data.data(),
         data.length(),
         (uchar*)ret.data(),
         &length);

    ret.resize(length);

    return ret;
}
