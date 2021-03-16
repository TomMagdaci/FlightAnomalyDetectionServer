//Tom Magdaci 


#ifndef SIMPLEANOMALYDETECTOR_ANOMALYDETECTOR_H
#define SIMPLEANOMALYDETECTOR_ANOMALYDETECTOR_H

#include "timeseries.h"
#include "math.h"
using namespace std;

class AnomalyReport{
public:
    const std::string description;
    const long timeStep;
    AnomalyReport(std::string description, long timeStep) :
            description(description),timeStep(timeStep){}
};
class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //SIMPLEANOMALYDETECTOR_ANOMALYDETECTOR_H
