#include <QtNetwork/QNetworkReply>
#include <QDateTime>
#include <QScriptValueIterator>
#include <QDebug>
#include <QHttpMultiPart>

#include "hmac.h"
#include "secrets.h" /* Contains user's api keys & passphrase */
#include "bitfloorapi.h"

BitFloorApi::BitFloorApi()
{
    connect(&nam, SIGNAL(finished(QNetworkReply*)), SLOT(replyReceived(QNetworkReply*)));
}

void BitFloorApi::start()
{
    timer.start(30000, this);
    getBalances();
    refresh();
}

void BitFloorApi::timerEvent(QTimerEvent *)
{
    refresh();
}

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

void BitFloorApi::replyReceived(QNetworkReply *r)
{
    if (r->error() == QNetworkReply::NoError) {
        QByteArray data = r->readAll();

        QScriptValue v = engine.evaluate("(" + QString(data) + ")");

        if (r->property("type") == "ticker") {
            double bid = v.property("bid").property(0).toNumber();
            double ask = v.property("ask").property(0).toNumber();

            emit currentPrices(ask, bid);
        } else if (r->property("type") == "balance") {
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
    } else {
        qDebug() << r->errorString();
        qDebug() << r->readAll();
    }
}
