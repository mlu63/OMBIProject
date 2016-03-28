#ifndef ULTRADATA_H
#define ULTRADATA_H

#include <vector>

#include <QObject>

#include "Data.h"

class UltraData : public QObject
{
    Q_OBJECT

public:
    explicit UltraData(std::vector<Data*>* dataVectorIn);

signals:
    void dataReady(std::vector<Data*>* data);

private:
    std::vector<Data*>* dataVector;

private slots:
    void newServiceDataRequest();

};


#endif // ULTRADATA_H
