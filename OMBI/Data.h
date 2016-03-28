#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QObject>

#include <map>


class Data : public QObject
{
    Q_OBJECT
public:
    explicit Data(QString cityNameIn, QString serviceAreaIn, std::map<int, float> yearlyDataIn);

    QString getCityName();
    std::map<int, float> getYearlyData();

private:
    QString cityName;
    QString serviceArea;

    std::map<int, float> yearlyData;


};


#endif // DATA_H
