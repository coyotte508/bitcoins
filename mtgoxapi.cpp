#include <QtNetwork/QNetworkReply>
#include <QDateTime>
#include <QDebug>

#include "mtgoxapi.h"
#include "secrets.h"
#include "hmac.h"

void MtGoxApi::refresh()
{
    nam.get(QNetworkRequest(QUrl("https://mtgox.com/api/1/BTCUSD/ticker")))->setProperty("type", "ticker");
}

void MtGoxApi::getBalances()
{
    static qint64 lastCall = 0;

    QNetworkRequest req(QUrl("https://mtgox.com/api/1/generic/private/info"));

    QByteArray secret = Secret::mgsecretkey;
    secret = QByteArray::fromBase64(secret);

    lastCall = std::max(lastCall+1, QDateTime::currentMSecsSinceEpoch());

    QByteArray body = "nonce=" + QByteArray::number(lastCall);
    QByteArray sign = hmac(secret, body).toBase64();

    req.setRawHeader("Rest-Key", Secret::mgapikey);
    req.setRawHeader("Rest-Sign", sign);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    nam.post(req, body)->setProperty("type", "balance");
}

void MtGoxApi::analyzeTicker(const QScriptValue &v)
{
    double bid = v.property("return").property("buy").property("value").toNumber();
    double ask = v.property("return").property("sell").property("value").toNumber();

    emit currentPrices(ask, bid);
}

void MtGoxApi::analyzeBalance(const QScriptValue &v)
{
    QScriptValue wallet = v.property("return").property("Wallets");
    double btc = wallet.property("BTC").property("Balance").property("value").toNumber();
    double usd = wallet.property("USD").property("Balance").property("value").toNumber();

    emit currentBalances(btc, usd);
}
