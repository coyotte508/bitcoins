#ifndef MtGoxAPI_H
#define MtGoxAPI_H

#include "genericapi.h"

class MtGoxApi : public GenericApi
{
    Q_OBJECT
public slots:
    void refresh();
signals:
    void currentPrices(double ask, double bid);
protected:
    void analyzeTicker(const QScriptValue &);
};

#endif // MtGoxAPI_H
