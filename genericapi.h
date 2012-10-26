#ifndef GENERICAPI_H
#define GENERICAPI_H


#include <QBasicTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtScript/QScriptEngine>

class GenericApi : public QObject
{
    Q_OBJECT
public:
    GenericApi();

    void start();
public slots:
    void replyReceived(QNetworkReply*);

    virtual void refresh() = 0;
    virtual void getBalances(){}
protected:
    void timerEvent(QTimerEvent *);
signals:
    void currentPrices(double ask, double bid);
    void currentBalances(double btc, double usd);
protected:
    virtual void analyzeTicker(const QScriptValue &){}
    virtual void analyzeBalance(const QScriptValue &){}

    QBasicTimer timer;
    QNetworkAccessManager nam;
    QScriptEngine engine;
};

#endif // GENERICAPI_H
