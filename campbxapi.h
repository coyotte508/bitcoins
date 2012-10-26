#ifndef CAMPBXAPI_H
#define CAMPBXAPI_H

#include "genericapi.h"

class CampBxApi : public GenericApi
{
    Q_OBJECT
public slots:
    void refresh();
protected:
    void analyzeTicker(const QScriptValue &v);
};

#endif // CAMPBXAPI_H
