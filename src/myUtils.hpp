//
//  myUtils.hpp
//  my3dDawRendition
//
//  Created by Avrosh Kumar on 12/10/16.
//
//

#ifndef myUtils_hpp
#define myUtils_hpp

#include "ofMain.h"

class myUtils {

public:
    ofVec2f cartesianToPolar(int,int);
    ofVec2f polarToCartesian(float,float);
};

#endif /* myUtils_hpp */
