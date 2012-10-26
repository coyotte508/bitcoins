#ifndef BITFLOORAPI_H
#define BITFLOORAPI_H

#include "genericapi.h"

class BitFloorApi : public GenericApi
{
    Q_OBJECT
public slots:
    void refresh();
    void getBalances();
protected:
    void analyzeTicker(const QScriptValue &data);
    void analyzeBalance(const QScriptValue &data);
};

#endif // BITFLOORAPI_H
