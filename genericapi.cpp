#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QScriptValue>

#include "genericapi.h"

GenericApi::GenericApi()
{
    connect(&nam, SIGNAL(finished(QNetworkReply*)), SLOT(replyReceived(QNetworkReply*)));
}

void GenericApi::start()
{
    timer.start(30000, this);
    getBalances();
    refresh();
}

void GenericApi::timerEvent(QTimerEvent *)
{
    refresh();
}

void GenericApi::replyReceived(QNetworkReply *r)
{
    if (r->error() == QNetworkReply::NoError) {
        QByteArray data = r->readAll();
        //qDebug() << data;

        QScriptValue v = engine.evaluate("(" + QString(data) + ")");

        if (r->property("type") == "ticker") {
            analyzeTicker(v);
        } else if (r->property("type") == "balance") {
            analyzeBalance(v);
        }
    } else {
        qDebug() << r->errorString();
        qDebug() << r->readAll();
    }
}

