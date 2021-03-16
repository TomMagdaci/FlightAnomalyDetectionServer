//Tom Magdaci 

#ifndef SIMPLEANOMALYDETECTOR_TIMESERIES_H
#define SIMPLEANOMALYDETECTOR_TIMESERIES_H
#include "anomaly_detection_util.h"
#include <vector>
#include <string>
#include <cstring>

class Feature { 
    char* name;
    int sizeName;
    std::vector<float> *v;
public:
    Feature(): v(new std::vector<float>()), name() {};
    Feature(const char* s): v(new std::vector<float>()){
        this->sizeName = strlen(s);
        this->name = new char[this->sizeName + 1];
        strcpy(this->name, s);
    };
    //copy constructor
    Feature(const Feature& f){
        //std::cout << "Feature's copy constructor has been called" << std::endl; //DELETE ME
        //Here the fields of this has been created already, and we want to use the 'copy assignment operator' for the copy,
        //but this method first delete my fields' allocated memory,
        this->name = nullptr;
        this->v = nullptr;
        (*this) = f;
    }

    //move constructor
    Feature(Feature&& f) {
        (*this) = std::move(f);
    }

    //copy assignment operator (deep copy)
    Feature& operator=(const Feature& f) {
        //std::cout << "Feature's copy assignment has been called" << std::endl; //DELETE ME
        if(this != &f) { //checking equal by address because if we check with the actual object the equalization  will come into this function.
            delete[] this->name;
            delete this->v;
            this->name = new char[strlen(f.getName()) + 1];
            strcpy(this->name, f.getName());
            this->sizeName = f.getVecFloatsSize();
            this->v = new std::vector<float>;
            std::vector<float>::iterator it;
            for (it = f.v->begin(); it != f.v->end(); it++) {
                this->v->push_back(*it);
            }
        }
        return *this;
    }

    //move assignment operator (shallow copy)
    Feature& operator=(Feature&& f) {
        if (this != &f) {
            this->name = f.name;
            this->v = f.v;
            f.name = nullptr;
            f.sizeName = 0;
            f.v = nullptr;
        }
        return *this;
    }
    virtual ~Feature() {
        delete this->v;
        delete[] this->name;
    }

    void setName(const char* c) {
        char* temp = name;
        this->sizeName = strlen(c);
        this->name = new char[this->sizeName + 1];
        strcpy(this->name, c);
        delete temp;
    }
    const char* getName() const {
        return this->name;
    }
    const std::vector<float>& getRefToFeaVecOfFloats() const{
        return *(this->v);
    }
    bool operator!=(const Feature& f){
        if (strcmp(this->name, f.getName()) == 0) {
            return false;
        }
        return true;
    }
    Point** getPointsInTwoFeaturesAxises(const Feature& f2) const{
        Point** pArr = new Point*[(*this).v->size()]();
        std::vector<float>::iterator it1,it2;
        int i = 0;
        for (it1 = (*this).v->begin(), it2 = f2.v->begin(); it1 != (*this).v->end(), it2 != f2.v->end(); it1++, it2++) {
            pArr[i++] = new Point((*it1), (*it2));
        }
        //"*this" is the x values, and "*f2" is the y values as required.
        return pArr;
    }
    int getVecFloatsSize() const{return v->size();}
    friend class TimeSeries;
};



class TimeSeries{
    std::vector<Feature*> *vecFeatures;
    char* pathFile;
    int sizePathFileName;
public:
    TimeSeries(const char* s): vecFeatures(new std::vector<Feature*>()){
        this->sizePathFileName = strlen(s);
        this->pathFile = new char[this->sizePathFileName + 1];
        strcpy(this->pathFile, s);
        this->csvLoading();
    };
    //copy constructor
    TimeSeries(const TimeSeries& t): vecFeatures(nullptr), pathFile(nullptr), sizePathFileName(0){
        //std::cout << "TimeSeries's copy constructor has been called" << std::endl; //DELETE ME
        *this = t;
    }
    //move constructor
    TimeSeries(TimeSeries&& t): vecFeatures(nullptr), pathFile(nullptr), sizePathFileName(0){
        *this = std::move(t);
    }
    //copy assignment operator
    TimeSeries& operator=(const TimeSeries& t){
        //std::cout << "TimeSeries's copy assignment has been called" << std::endl; //DELETE ME
        if (this != &t){
            delete[] this->pathFile;
            deleteVecOfFea();
            this->sizePathFileName = t.sizePathFileName;
            this->pathFile = new char[strlen(t.pathFile) + 1];
            strcpy(this->pathFile, t.pathFile);
            std::vector<Feature*>::iterator it;
            for(it = t.vecFeatures->begin(); it != t.vecFeatures->end(); it++) {
                this->vecFeatures->push_back(new Feature(*(*it)));
            }
        }
        return *this;
    }
    //move assignment operator
    TimeSeries& operator=(TimeSeries&& t){
        if (this != &t) {
            //delete[] this->pathFile;
            //deleteVecOfFea();
            this->pathFile = t.pathFile;
            this->sizePathFileName = t.sizePathFileName;
            this->vecFeatures = t.vecFeatures;
            t.pathFile = nullptr;
            t.sizePathFileName = 0;
            t.vecFeatures = nullptr;
        }
        return *this;
    }
    virtual ~TimeSeries() {
        delete[] this->pathFile;
        deleteVecOfFea();

    }
    void deleteVecOfFea() {
        std::vector<Feature*>::iterator it;
        for (it = this->vecFeatures->begin(); it != this->vecFeatures->end(); it++) {
            delete (*it);
        }
        delete this->vecFeatures;
    }
    void csvLoading();
    char** getFeaturesName() const;
    int getNumOfFeatures() const;
    const std::vector<Feature*>& getRefToVecOfFeatures() const;
    const Feature& getFeatureByName(const char* feaName) const;
};


#endif //SIMPLEANOMALYDETECTOR_TIMESERIES_H
