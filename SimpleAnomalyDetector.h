//Tom Magdaci 316603604


#ifndef SIMPLEANOMALYDETECTOR_SIMPLEANOMALYDETECTOR_H
#define SIMPLEANOMALYDETECTOR_SIMPLEANOMALYDETECTOR_H

#include "AnomalyDetector.h"
#include <algorithm>

//an global function to delete an array of objects
template<class T>
void releaseArr(T** arr, int size) {
    int i = 0;
    for(;i<size;i++) {
        delete arr[i];
    }
    delete[] arr;
}

struct correlatedFeatures{
    std::string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    Point* center;
};
class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector {
protected:
    std::vector<correlatedFeatures> cf;
    float threshold;
public:
    SimpleAnomalyDetector(): TimeSeriesAnomalyDetector() {
        threshold = 0.9;
    };

    //copy constructor
    SimpleAnomalyDetector(SimpleAnomalyDetector& s){
        *this = s;
    }
    //move constructor
    SimpleAnomalyDetector(SimpleAnomalyDetector&& s){
        *this = move(s);
    }

    //copy assignment operator
    SimpleAnomalyDetector& operator=(SimpleAnomalyDetector& s) {
        if (this != &s) {
            this->cf = s.cf;
        }
        return *this;
    }
    //move assignment operator
    SimpleAnomalyDetector& operator=(SimpleAnomalyDetector&& s){
        if (this != &s) {
            this->cf = s.cf;
        }
        return *this;
    }
    virtual ~SimpleAnomalyDetector(){};

    virtual void learnNormal(const TimeSeries &ts);

    bool checkIfExistPairOfCor(const char* newFeaOne, const char* newFeaTwo) ;

    virtual std::vector<AnomalyReport> detect(const TimeSeries &ts);

    std::vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    void setCorrelationThreshold(float threshold){
        this->threshold=threshold;
    }
    float getThreshold() {
        return this->threshold;
    }
protected:
    virtual void learnHelper(const TimeSeries& ts,float p/*pearson*/,const char* f1, const char* f2,Point** ps);
    virtual bool isAnomalous(float x, float y,correlatedFeatures c);
};
#endif //SIMPLEANOMALYDETECTOR_SIMPLEANOMALYDETECTOR_H
