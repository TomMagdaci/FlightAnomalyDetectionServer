//Tom Magdaci 

#include "timeseries.h"
#include <fstream>
#include <sstream>

void TimeSeries::csvLoading() {
    std::fstream fin;
    fin.open(this->pathFile, std::ios::in);
    std::vector<std::string> row;
    std::string line, word, temp;
    int flag = 0; //For knowing when features initializations are over
    int counter =0 ; //For counting the features initializations
    int i = 0;
    int length = 0;

    while (std::getline(fin, line)) {
        std::stringstream s(line);
        while (std::getline(s, word, ',')) {
            if (flag == 0) {//Initialization of Features is needed
                (*(this->vecFeatures)).push_back(new Feature(word.c_str()));
                /*//.push_back(new Feature(word));
                // add all the column data
                // of a row to a vector
                //row.push_back(word);*/
                counter++;
            } else { //counter == number of Features in vecFeatures
                if (i >= counter) { //i = index that runs on ll the features in the vecfeatures
                                    //therefore when it's equal to counter it should turn to zero because it passed its limits.
                    i = 0;
                }
                (*(*(this->vecFeatures))[i++]->v).push_back(std::stof(word));
            }
        }
        flag = 1;
    }
}

char** TimeSeries::getFeaturesName() const{
    if (this->vecFeatures->empty()) {
        return nullptr;
    }

    char** feaNames = new char*[(this->vecFeatures)->size()];
    int i = 0;
    std::vector<Feature*>::iterator it;
    for (it = this->vecFeatures->begin(); it != this->vecFeatures->end(); it++) {
        strcpy(*(feaNames+(i++)),(*it)->name);
    }
    return feaNames;
}

int TimeSeries::getNumOfFeatures() const{
    return (this->vecFeatures)->size();
}

const std::vector<Feature*>& TimeSeries::getRefToVecOfFeatures() const{
    return *(this->vecFeatures);
}

const Feature& TimeSeries::getFeatureByName(const char* feaName) const{
    std::vector<Feature*>::iterator it;
    for (it = this->vecFeatures->begin(); it != this->vecFeatures->end(); it++) {
        if (strcmp((*(*it)).getName(),feaName) == 0) {
            break;
        }
    }
    return (*(*it));
}

