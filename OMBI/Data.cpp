#include "Data.h"

Data::Data(QString cityNameIn, QString serviceAreaIn, std::map<int, float> yearlyDataIn)
{
    cityName=cityNameIn;
    serviceArea=serviceAreaIn;
    yearlyData=yearlyDataIn;
}


QString Data::getCityName()
{
    return cityName;
}

std::map<int, float> Data::getYearlyData()
{
    return yearlyData;
}
