#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

#include "qcustomplot.h"

#include "UltraData.h"

#include "Data.h"

#include <QSignalMapper>

//newly added 17:18
#include <QCheckBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
signals:
    void getNewServiceData();
    void cityClicked(int);
    void yearClicked(int);
    void nextClicked();

private:
    void populateGraph(std::vector<Data*>* serviceData);
    void newServiceData();
    void showSplash();

    Ui::MainWindow *ui;
    UltraData *theData;

    std::vector<Data*> incomingData;

    //newly added 17:18

    std::vector<Data*>* theServiceData;
    std::vector<QCheckBox*> yearCBVector;
    std::vector<QCheckBox*> cityCBVector;
    QSignalMapper *citySignal;
    QSignalMapper *yearSignal;

private slots:
    void displayNewServiceData(std::vector<Data*>* serviceData);
    void updateGraphYear(int boxID);
    void updateGraphCity(int boxID);
    void nextButtonClicked();

};

#endif // MAINWINDOW_H
