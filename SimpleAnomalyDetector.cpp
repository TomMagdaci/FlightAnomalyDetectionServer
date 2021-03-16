//Tom Magdaci 316603604


#include "SimpleAnomalyDetector.h"
#include <iostream>

bool SimpleAnomalyDetector::checkIfExistPairOfCor(const char* newFeaOne, const char* newFeaTwo) {
    if (this->cf.empty()) {
        return false;
    }
    std::vector<correlatedFeatures>::iterator it;
    for(it = (*this).cf.begin(); it != (*this).cf.end(); it++) {
        if((strcmp(it->feature1.c_str(), newFeaOne) == 0) && strcmp(it->feature2.c_str(), newFeaTwo) ==0) {
            return true;
        }
        if((strcmp(it->feature1.c_str(), newFeaTwo) == 0) && strcmp(it->feature2.c_str(), newFeaOne) ==0) {
            return true;
        }
    }
    return false;
}



void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    float temp = 0, maxCorPeaFor_it = 0, normThresholdForCor = this->threshold;
    std::string mostCorFeaName;
    Feature* mostCorFea;
    Point** pArr;
    //int fNum = ts.getNumOfFeatures();
    std::vector<Feature*>::const_iterator it, it2;
    int i = 0, j = 0; //will represents hat a match has occured.
    for (it = ts.getRefToVecOfFeatures().begin(); it != ts.getRefToVecOfFeatures().end(); it++, i++) {
        for (it2 = std::next(it); it2 != ts.getRefToVecOfFeatures().end(); it2++, j++) {
            //intended to check all the couples in the upper traingle of the couples matrix
            //Reaching here all the cases when it and it2 are not pointing on the same Feature*
            //As a result, *(*it) and *(*it2) are not the same Features.
            temp = pearson((float*) &(((*it)->getRefToFeaVecOfFloats())[0]), (float*) &(((*it2)->getRefToFeaVecOfFloats())[0]),
                         (*it)->getRefToFeaVecOfFloats().size());
            if (fabs(temp) > fabs(maxCorPeaFor_it)) {
                maxCorPeaFor_it = temp;
                mostCorFeaName = (*(*it2)).getName();
                mostCorFea = (*it2);
            }
        } //The internal loop ends here. As a result we have the most correlated feature to the feature that "it" pointing at.
        //Also we have their pearson's returned value, and the most correlated feature name, and himself.

        //Create point array with these features as axises, the feature it is pointing at is the x's axis
        pArr = (*(*it)).getPointsInTwoFeaturesAxises(*mostCorFea);

        learnHelper(ts,maxCorPeaFor_it/*pearson*/,(*(*it)).getName(), mostCorFeaName.c_str(),pArr);

        j = 0;
        maxCorPeaFor_it = 0;
        temp = 0;
        //Releasing the Point** array
        releaseArr(pArr, (*it)->getRefToFeaVecOfFloats().size());
    }

}

void SimpleAnomalyDetector::learnHelper(const TimeSeries& ts,float p/*pearson*/,const char* f1, const char* f2,Point** ps) {
    int psSize = ts.getFeatureByName(f1).getVecFloatsSize();
    float devThreshold = 1.1;
    if (fabs(p) >= this->threshold)  {
        //Checking if we already created this couple as a correlated features. And if they are actually correlated.
        //Create array of Points while the axis are "it" (=Feature1) and mostCorFea is the most correlated feature for "it"
        Line lReg = linear_reg(ps, psSize);
        (*this).cf.push_back({f1, f2,
                              p,
                              lReg,
                              devThreshold * maxDev(ps,psSize,lReg),
                              nullptr});
    }
}


bool SimpleAnomalyDetector::isAnomalous(float x, float y,correlatedFeatures c) {
    return (abs(y - c.lin_reg.f(x))>c.threshold);
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    //auto aR = new std::vector<AnomalyReport>();
    //return (*aR);
    //Pseudu code:
    //In loop go over all the couples in cF
    //Then for each couple by fea1 fea2 find faeture1 and feature2 i TImeseries
    //Then with the linear reg and
    std::vector<AnomalyReport> aR;
    std::vector<correlatedFeatures>::const_iterator it;
    int sizeP;
    Point** pArr;
    int i = 0;
    for (it = this->cf.begin(); it != this->cf.end(); it++) {
        const Feature& f1 = ts.getFeatureByName((*it).feature1.c_str());
        const vector<float>& f1Vals = f1.getRefToFeaVecOfFloats();
        const Feature& f2 = ts.getFeatureByName((*it).feature2.c_str());
        const vector<float>& f2Vals = f2.getRefToFeaVecOfFloats();
        pArr = f1.getPointsInTwoFeaturesAxises(f2);
        sizeP = f1.getVecFloatsSize();
        for (i=0;i<sizeP; i++) {
            if (isAnomalous(f1Vals[i], f2Vals[i],(*it))){
                string f1NameStr = f1.getName();
                string f2NameSrt = f2.getName();
                aR.push_back(AnomalyReport(f1NameStr + "-" + f2NameSrt, i+1));
            }
        }
        releaseArr(pArr, sizeP);
    }
    return aR;
}


