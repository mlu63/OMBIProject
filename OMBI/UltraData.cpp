
#include "UltraData.h"


UltraData::UltraData(std::vector<Data*>* dataVectorIn)
{
    dataVector=dataVectorIn;
}


void UltraData::newServiceDataRequest()
{
    emit UltraData::dataReady(dataVector);
}
