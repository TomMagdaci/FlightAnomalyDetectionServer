//Tom Magdaci 

#include "anomaly_detection_util.h"
#include <math.h>
#include <memory>

// returns the variance of X and Y
float var(float* x, int size){
    float mISize = (1/((float) size));
    int i = 0;
    float sumForFirSec = 0, sumForMiu = 0;
    for (i = 0; i < size; i++) {
        sumForFirSec += ((*(x+i) * (*(x+i))));
        sumForMiu += (*(x+i));
    }
    return ((mISize * sumForFirSec) - ((mISize * mISize) * (sumForMiu * sumForMiu))); 
}

/**
 * @brief Computing an average for list of numbers
 * 
 * @param z 
 * @param size 
 * @return float 
 */
float avg(float* z, int size){
    float ave = 0, sum = 0;
    int i = 0;
    for (i = 0; i < size; i++){
        sum += (*(z+i));
    }
    ave = (sum/size);
    return ave;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float xMean, yMean, sum = 0, cov = 0;
    int i;
    xMean = avg(x, size);
    yMean = avg(y, size);
    for (i = 0; i < size; i++){
        sum += (((*(x+i)) - xMean) * ((*(y+i)) - yMean));
    }
    cov = sum / size;
    return cov;
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    return (cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size))));
}

// performs a linear regression and return s the line equation
Line linear_reg(Point** points, int size){
    float a = 0, b = 0;
    float x[size], y[size];
    int i;
    for (i=0;i<size;i++){
        x[i] = (*(points+i))->x;
        y[i] = (*(points+i))->y;
    }
    a = (cov(x,y,size) / var(x,size));
    b = avg(y, size) - a * avg(x, size);
    Line* line = new Line(a, b);
    return (*line);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    float d;
    Line line = linear_reg(points, size);
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float d = fabs(l.f(p.x) - p.y);
    return d;
}

float maxDev(Point** pArr,int size, Line& l) {
    int i = 0;
    float maxDev=0, temp=0;
    for (;i <size;i++) {
        temp = dev(*(pArr[i]), l);
        if (maxDev < temp) {
            maxDev = temp;
        }
    }
    return maxDev;
}