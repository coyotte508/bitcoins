#include "campbxapi.h"
#include <QtNetwork/QNetworkReply>

void CampBxApi::refresh()
{
    nam.get(QNetworkRequest(QUrl("http://campbx.com/api/xticker.php")))->setProperty("type", "ticker");
}

void CampBxApi::analyzeTicker(const QScriptValue &v)
{
    double bid = v.property("Best Bid").toNumber();
    double ask = v.property("Best Ask").toNumber();

    emit currentPrices(ask, bid);
}

