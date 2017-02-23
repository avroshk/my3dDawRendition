//
//  myUtils.cpp
//  my3dDawRendition
//
//  Created by Avrosh Kumar on 12/10/16.
//
//

#include "myUtils.hpp"

ofVec2f myUtils::cartesianToPolar(int x,int y) {
    float r = sqrt(x^2 + y^2);
    float theta = atan2(y,x);
    ofVec2f polarPoint;
    polarPoint.set(r,theta);
    return polarPoint;
}

ofVec2f myUtils::polarToCartesian(float r,float theta) {
    int x = r * cos(theta*PI/180);
    int y = r * sin(theta*PI/180);
    ofVec2f cartesianPoint;
    cartesianPoint.set(x,y);
    return cartesianPoint;
}