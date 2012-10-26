#ifndef BITFLOORAPI_H
#define BITFLOORAPI_H

#include <QBasicTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtScript/QScriptEngine>

class BitFloorApi : public QObject
{
    Q_OBJECT
public:
    BitFloorApi();

    void start();
public slots:
    void replyReceived(QNetworkReply*);

    void refresh();
    void getBalances();
protected:
    void timerEvent(QTimerEvent *);
signals:
    void currentPrices(double ask, double bid);
    void currentBalances(double btc, double usd);
private:
    QBasicTimer timer;
    QNetworkAccessManager nam;
    QScriptEngine engine;
};

#endif // BITFLOORAPI_H
