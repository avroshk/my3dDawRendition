//
//  TimeLine.hpp
//  my3dDawRendition
//
//  Created by Avrosh Kumar on 1/19/17.
//
//

#ifndef TimeLine_hpp
#define TimeLine_hpp

#include "ofMain.h"

class TimeLine {

private:
    int numSamplesReceived;
    float timeInSec;
    int sampleRate;
    int bufferSize;
    ofPolyline line;
    ofRectangle rect[20];
    int measure[4];
    
   
    
public:
    void setup(int sampleRate, int bufferSize);
    void updateTime();
    void updateCurrentMeasure(string beatString);
    float getTimeinMs();
    void draw();
    int getMeasureInPixels();
    void moveTimeline();

};

#endif /* TimeLine_hpp */
