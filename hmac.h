#ifndef HMAC_H
#define HMAC_H

#include <QByteArray>

QByteArray hmac(QByteArray secretKey, QByteArray data);

#endif // HMAC_H
