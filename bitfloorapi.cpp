#include <QtNetwork/QNetworkReply>
#include <QScriptValueIterator>
#include <QDateTime>

#include "hmac.h"
#include "secrets.h" /* Contains user's api keys & passphrase */
#include "bitfloorapi.h"

void BitFloorApi::refresh()
{
    QNetworkReply *rep = nam.get(QNetworkRequest(QUrl("https://api.bitfloor.com/book/L1/1")));
    rep->setProperty("type", "ticker");
}

void BitFloorApi::getBalances()
{
    static qint64 lastCall = 0;

    QNetworkRequest req(QUrl("https://api.bitfloor.com/accounts"));

    QByteArray secret = bfsecretkey;
    secret = QByteArray::fromBase64(secret);

    lastCall = std::max(lastCall+1, QDateTime::currentMSecsSinceEpoch());

    QByteArray body = "nonce=" + QByteArray::number(lastCall);
    QByteArray sign = hmac(secret, body).toBase64();

    req.setRawHeader("bitfloor-key", bfapikey);
    req.setRawHeader("bitfloor-passphrase", bfpassphrase);
    req.setRawHeader("bitfloor-version", "1");
    req.setRawHeader("bitfloor-sign", sign);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    nam.post(req, body)->setProperty("type", "balance");
}

void BitFloorApi::analyzeTicker(const QScriptValue &v)
{
    double bid = v.property("bid").property(0).toNumber();
    double ask = v.property("ask").property(0).toNumber();

    emit currentPrices(ask, bid);
}

void BitFloorApi::analyzeBalance(const QScriptValue &v)
{
    double btc(0), usd(0);

    QScriptValueIterator it(v);

    while (it.hasNext()) {
        it.next();

        QString type = it.value().property("currency").toString();

        if (type == "BTC") {
            btc = it.value().property("amount").toNumber();
        } else if (type == "USD") {
            usd = it.value().property("amount").toNumber();
        }
    }

    emit currentBalances(btc, usd);
}
