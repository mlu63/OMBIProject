 * Date: Oct 24, 2013
 * mainwindow.cpp
 * Description: The maindow class is the "controller" in the MVC design pattern. Used to communicate between the data and the UI
 */

#include <QCheckBox>
#include <QVector>

#include <set>
#include <map>
#include <utility>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"


/* Constructor
 * Description: Creates a new mainwindow ui object and loads in all the data from a data stub
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");


    std::vector<QCheckBox> yearCBVector;    //Vector containing the QCheckBox objects representing years selectable in the filter
    std::vector<QCheckBox> cityCBvector;    //Vector containing the QCheckBox objects representing cities selectable in the filter
    //Create a QSignalMapper to all of the city QCheckBox objects
    citySignal=new QSignalMapper(this);
    connect(citySignal, SIGNAL(mapped(int)),this,SIGNAL(cityClicked(int)));
    //Create a QSignalMapper to all of the year QCheckBox objects
    yearSignal=new QSignalMapper(this);
    connect(yearSignal,SIGNAL(mapped(int)),this,SIGNAL(yearClicked(int)));
    connect(ui->nextButton, SIGNAL(clicked()),this, SLOT(nextButtonClicked()));

    //          >>>>>>>  Data Stub  <<<<<<<<
    std::map<int, float> londonData;
    londonData.insert(std::pair<int, float>(2009, 1.033));
    londonData.insert(std::pair<int, float>(2010, 1.073));
    londonData.insert(std::pair<int, float>(2011, 1.033));


    std::map<int, float> barrieData;
    barrieData.insert(std::pair<int, float>(2009, 0.843));
    barrieData.insert(std::pair<int, float>(2010, 0.780));
    barrieData.insert(std::pair<int, float>(2011, 0.843));

    std::map<int, float> calgaryData;
    calgaryData.insert(std::pair<int, float>(2009, 0.630));
    calgaryData.insert(std::pair<int, float>(2010, 0.594));
    calgaryData.insert(std::pair<int, float>(2011, 0.630));

    std::map<int, float> hamiltonData;
    hamiltonData.insert(std::pair<int, float>(2009, 0.661));
    hamiltonData.insert(std::pair<int, float>(2010, 0.984));
    hamiltonData.insert(std::pair<int, float>(2011, 0.661));

    std::map<int, float> ottawaData;
    ottawaData.insert(std::pair<int, float>(2009, 0.841));
    ottawaData.insert(std::pair<int, float>(2010, 0.932));
    ottawaData.insert(std::pair<int, float>(2011, 0.841));

    std::map<int, float> sudburyData;
    sudburyData.insert(std::pair<int, float>(2009, 1.312));
    sudburyData.insert(std::pair<int, float>(2010, 1.378));
    sudburyData.insert(std::pair<int, float>(2011, 1.312));

    std::map<int, float> thunderBayData;
    thunderBayData.insert(std::pair<int, float>(2009, 1.797));
    thunderBayData.insert(std::pair<int, float>(2010, 1.475));
    thunderBayData.insert(std::pair<int, float>(2011, 1.797));

    std::map<int, float> torontoData;
    torontoData.insert(std::pair<int, float>(2009, 0.901));
    torontoData.insert(std::pair<int, float>(2010, 0.953));
    torontoData.insert(std::pair<int, float>(2011, 0.901));

    std::map<int, float> windsorData;
    windsorData.insert(std::pair<int, float>(2009, 1.381));
    windsorData.insert(std::pair<int, float>(2010, 1.640));
    windsorData.insert(std::pair<int, float>(2011, 1.381));

    std::map<int, float> winnipegData;
    winnipegData.insert(std::pair<int, float>(2009, 1.666));
    winnipegData.insert(std::pair<int, float>(2010, 0.0));
    winnipegData.insert(std::pair<int, float>(2011, 1.666));

    QString serviceName="Number of Residential Structural Fires with Losses per 1,000 Households (Urban)";
    //Add data to the vector that UltraData will contain
    incomingData.push_back(new Data("London", serviceName, londonData));
    incomingData.push_back(new Data("Barrie", serviceName, barrieData));
    incomingData.push_back(new Data("Calgary", serviceName, calgaryData));
    incomingData.push_back(new Data("Hamilton", serviceName, hamiltonData));
    incomingData.push_back(new Data("Ottawa", serviceName, ottawaData));
    incomingData.push_back(new Data("Sudbury", serviceName, sudburyData));
    incomingData.push_back(new Data("Thunder Bay", serviceName, thunderBayData));
    incomingData.push_back(new Data("Toronto", serviceName, torontoData));
    incomingData.push_back(new Data("Windsor", serviceName, windsorData));
    incomingData.push_back(new Data("Winnipeg", serviceName, winnipegData));


    theData=new UltraData(&incomingData);   //create new UltraData object containing the data stub (Each city is its own Data object)

    //Used to signal controller that the data is ready to be displayed in the UI
    connect(theData, SIGNAL(dataReady(std::vector<Data*>*)), this, SLOT(displayNewServiceData(std::vector<Data*>*)));

    //Used to signal the model (UltraData) that new data is to be retreived
    connect(this, SIGNAL(getNewServiceData()), theData, SLOT(newServiceDataRequest()));

    //Used to signal the graph to apply filters to years/municipalities
    connect(this, SIGNAL(yearClicked(int)), this, SLOT(updateGraphYear(int)));
    connect(this, SIGNAL(cityClicked(int)), this, SLOT(updateGraphCity(int)));

    newServiceData();  //Get the data

    showSplash();



}

//Desconstructor
MainWindow::~MainWindow()
{
    delete ui;
}

/* Method Name: populateGraph
 * Description: This method populates the QCustomPlot with bar graphs containing all the service data
 *
 * Parameters:
 *  vector<Data*>* serviceData      vector containing all of the Data objects associated with the service area selected by the user
 */
void MainWindow::populateGraph(std::vector<Data*>* serviceData)
{
    QCustomPlot* plot=ui->plot;
    theServiceData=serviceData;

    QVector<QColor> colorPalette;


    // Set up a palette of pen/brush colors for the different cities
    colorPalette.push_back(QColor(Qt::red));
    colorPalette.push_back(QColor(Qt::green));
    colorPalette.push_back(QColor(Qt::blue));
    colorPalette.push_back(QColor(Qt::cyan));
    colorPalette.push_back(QColor(Qt::magenta));
    colorPalette.push_back(QColor(Qt::yellow));
    colorPalette.push_back(QColor(Qt::gray));
    colorPalette.push_back(QColor(Qt::darkRed));
    colorPalette.push_back(QColor(Qt::darkGreen));
    colorPalette.push_back(QColor(Qt::darkCyan));

    // Reduce the alpha channel somewhat so the cities overlay in a usable way
    QVector<QColor>::Iterator colorIterator=colorPalette.begin();
    while(colorIterator!=colorPalette.end())
    {
        colorIterator->setAlpha(100);
        ++colorIterator;
    }

    colorIterator=colorPalette.begin();

    // Set the major elements of the QCustomPlot
    plot->xAxis->setLabel("Year");
    plot->yAxis->setLabel("Measurement");
    plot->xAxis->setTicks(false);
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(1.0);
    plot->xAxis->setRange(2009, 2011.02);
    plot->yAxis->setRange(0, 3.0);



    std::vector<Data*>::iterator serviceIter=serviceData->begin();
    std::map<int, float> yearlyData;
    std::map<int, float>::iterator yearlyIter;


    // Iterate through all the retrieved data and add it to new bar graphs
    int i=0;
    while(serviceIter!=serviceData->end())
    {
        yearlyData=(*serviceIter)->getYearlyData();
        yearlyIter=yearlyData.begin();

        QVector<double> x(yearlyData.size());
        QVector<double> y(yearlyData.size());

        for(int j=0; j<(int)yearlyData.size(); j++)
        {

            x[j]=(*yearlyIter).first;
            y[j]=(*yearlyIter).second;

            ++yearlyIter;
        }

        QCPBars *nextBar=new QCPBars(plot->xAxis, plot->yAxis);
        plot->addPlottable(nextBar);    // Add this bar graph to the main plot
        nextBar->setPen(QPen(*colorIterator));
        nextBar->setBrush(*colorIterator);
        colorIterator++;
        nextBar->setName((*serviceIter)->getCityName());
        nextBar->setData(x, y);
        nextBar->setVisible(false); // To coincide with city names defaulting unchecked


        plot->replot();
        i++;
        serviceIter++;
    }


}

/* Method Name: displayNewServiceData
 * Description: This method is used to display the data on the graph
 *
 * Parameters:
 *  vector<Data*>* serviceData      vector containing all of the Data objects associated with the service area selected by the user
 */
void MainWindow::displayNewServiceData(std::vector<Data*>* serviceData)
{
    std::vector<Data*>::iterator dataIter=serviceData->begin();     //Iterator for data
    std::map<int, float>::iterator mapIter;                         //Iterator for map of yearly data

    std::set<int> years;                                            //Set of years to be displayed on the graph

    int j=0;  // j and k are used for drawing the correct amount of QCheckBoxes per column
    int k=0;

    //Draws QCheckBox filters for cities
    while(dataIter!=serviceData->end())
    {


        for(int i=0; i<3; i++)          //For each for statement a column of QCheckBoxes are drawn
        {

            if(dataIter!=serviceData->end())
            {
                QCheckBox *checkBoxAdd=new QCheckBox((*dataIter)->getCityName());

                connect(checkBoxAdd, SIGNAL(stateChanged(int)), citySignal, SLOT(map()));
                citySignal->setMapping(checkBoxAdd,k);

                cityCBVector.push_back(checkBoxAdd);
                this->ui->cityLayout->addWidget(checkBoxAdd, i, j);
                k++;
                ++dataIter;
            }
        }

        ++j;
    }


    //Gets the years from the yearly Data
    dataIter=serviceData->begin();
    while(dataIter!=serviceData->end())
    {
        std::map<int, float> theYears=(*dataIter)->getYearlyData();
        mapIter=theYears.begin();
        while(mapIter!=theYears.end())
        {
            years.insert((*mapIter).first);
            ++mapIter;
        }
        ++dataIter;
    }

    //Draws the QCheckBox filters for years
    std::set<int>::iterator yearIter=years.begin();
    int h=0;
    while(yearIter!=years.end())
    {
        QCheckBox *checkBoxAdd=new QCheckBox((QString::number(*yearIter)));\
        checkBoxAdd->setChecked(true);
        yearSignal->setMapping(checkBoxAdd,h);
        connect(checkBoxAdd,SIGNAL(stateChanged(int)),yearSignal,SLOT(map()));
        ++h;
        yearCBVector.push_back(checkBoxAdd);
        this->ui->yearLayout->addWidget(checkBoxAdd);
        ++yearIter;
    }


    populateGraph(serviceData);


  }

/* Method Name: updateGraphYear
 * Description: This method is used to apply a filter for years on the graph
 *
 * Parameters:
 *  int boxID      contains integer value for the year to be updated in the graph
 */
void MainWindow::updateGraphYear(int boxID){

    std::vector<Data*>::iterator dataIter=theServiceData->begin();


    if(yearCBVector[boxID]->isChecked())
    {
        // Add data
        for(int i=0; i<this->ui->plot->plottableCount(); i++)
        {
            std::map<int, float> theYears=(*dataIter)->getYearlyData();

            float currentYearData=(*(theYears.find(yearCBVector[boxID]->text().toInt()))).second;
            ((QCPBars*)this->ui->plot->plottable(i))->addData(yearCBVector[boxID]->text().toDouble(), currentYearData);
            ++dataIter;
        }

    }
    else
    {
        for(int i=0; i<this->ui->plot->plottableCount(); i++)
            ((QCPBars*)this->ui->plot->plottable(i))->removeData(yearCBVector[boxID]->text().toDouble());

    }


    this->ui->plot->replot();
}

/* Method Name: updateGraphCity
 * Description: This method is used to apply a filter for years on the graph
 *
 * Parameters:
 *  int boxID      contains integer value for the city to be updated in the graph
 */

void MainWindow::updateGraphCity(int boxID){



    if(!(this->ui->plot->plottable(boxID)->visible()))
    {
        // Turn graph on
        this->ui->plot->plottable(boxID)->setVisible(true);
        this->ui->plot->replot();
    }
    else
    {
        // Turn graph off
        this->ui->plot->plottable(boxID)->setVisible(false);
        this->ui->plot->replot();
    }

}

/* Method Name: newServiceData
 * Description: This method is used to emit a signal for the model to retreive new data
 */
void MainWindow::newServiceData()
{
    emit getNewServiceData();
}


void MainWindow::nextButtonClicked(){
    ui->plot->setVisible(true);
    ui->cityFrame->setVisible(true);
    ui->yearFrame->setVisible(true);
    ui->nextButton->setVisible(false);
    ui->ombiLogo->setVisible(false);
    ui->ombiGoal->setVisible(false);
}


void MainWindow::showSplash(){
    ui->plot->setVisible(false);
    ui->cityFrame->setVisible(false);
    ui->yearFrame->setVisible(false);
}
