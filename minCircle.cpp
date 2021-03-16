//Tom Magdaci 316603604

#include "minCircle.h"

float distanceBetweenTwoPoints(Point& p1, Point& p2){
    return (sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)));
}

Point midPoint(Point& p, Point& q) {
    return Point((p.x + q.x) / 2, (p.y + q.y) / 2);
}

float calcSlope(Point& p, Point& q) {
    return ((q.y - p.y) / (q.x - p.x));
}

Circle getCircleFrom3Points(Point l, Point m, Point r) {
    float epsilon = pow(10,-14);
    // A is line from l to m
    // B is line from r to m
    if ((l.x == m.x) && (m.x == r.x))
        return Circle(Point(0,0), -1);
    if (l.x == m.x)
        return getCircleFrom3Points(l, r, m);
    if (m.x == r.x)
        return getCircleFrom3Points(m, l, r);
    //mA is the slope of A
    float mA = calcSlope(l, m);
    //mB is the slope of B
    float mB = calcSlope(m, r);
    float CenterX = (mA * mB * (l.y - r.y) + mB * (l.x + m.x) - mA * (m.x + r.x)) / (2 * (mB - mA));
    //calc the Y value of A in CenterX
    float CenterY;
    if(mA > epsilon)
        CenterY = ((-1 / mA) * (CenterX - ((l.x+m.x)/2))) + ((l.y + m.y)/2);
    else
        CenterY = ((-1 / mB) * (CenterX - ((m.x+r.x)/2))) + ((m.y + r.y)/2);
    //float CenterY = (mA * (CenterX - l.x)) + l.y;
    Point center(CenterX, CenterY);
    return Circle(center, distanceBetweenTwoPoints(center, l));
}

Circle trivialCircle(vector<Point*> r, size_t sizeR) {
    switch (sizeR) {
        case 0:
            //The empty circle, there is no point in it. the -1 radius symbolize that.
            return Circle(Point(0,0), -1);
        case 1:
            return Circle(*(r[0]), 0);
        case 2:
            return Circle(midPoint(*(r[0]),*(r[1])),
                          distanceBetweenTwoPoints(*(r[0]), *(r[1]))/ 2);
        case 3:
            //In case of three points a circumcircle will be found.
            return getCircleFrom3Points(*(r[0]), *(r[1]), *(r[2]));
        default:
            break;
    }
}
bool checkPointInCircle(Circle& c, Point& p) {
    if (c.radius == -1) {
        return false;
    }
    if ((c.center.x == p.x) && (c.center.y == p.y)) {
        return true;
    }
    if (distanceBetweenTwoPoints(c.center,p) <= c.radius){
        return true;
    }
    return false;
}

Circle welzel(std::vector<Point*>& p, size_t sizeP, vector<Point*> r, size_t sizeR) {
    //cout << sizeP << ", " <<sizeR <<endl;
    if (sizeP == 0 || sizeR == 3) {
        return trivialCircle(r, sizeR);
    }
    Circle c = welzel(p, sizeP-1, r, sizeR);
    //cout <<"*"<< sizeP - 1 << ", " <<sizeR <<"*"<<"   ";
    if (checkPointInCircle(c,*(p[sizeP-1]))) {
        return c;
    }
    r.push_back(p[sizeP-1]);
    Circle ctrue = welzel(p, sizeP-1, r, sizeR+1);
    //cout <<"||"<< sizeP - 1 << ", " <<sizeR+1 <<endl;
    return ctrue;

}

Circle findMinCircle(Point** points,size_t size){
    vector<Point*> r;
    std::vector<Point*> p(points,points+size);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle (p.begin(), p.end(), std::default_random_engine(seed));
    return welzel(p, size, r, 0);
}

