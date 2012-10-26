#include "mtgoxapi.h"
#include <QtNetwork/QNetworkReply>

void MtGoxApi::refresh()
{
    nam.get(QNetworkRequest(QUrl("https://mtgox.com/api/1/BTCUSD/ticker")))->setProperty("type", "ticker");
}

void MtGoxApi::analyzeTicker(const QScriptValue &v)
{
    double bid = v.property("return").property("buy").property("value").toNumber();
    double ask = v.property("return").property("sell").property("value").toNumber();

    emit currentPrices(ask, bid);
}
