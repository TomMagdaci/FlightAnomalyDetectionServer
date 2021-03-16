// Tom Magdaci 316603604

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};

Circle findMinCircle(Point** points,size_t size);
float distanceBetweenTwoPoints(Point& p1, Point& p2);







#endif /* MINCIRCLE_H_ */
