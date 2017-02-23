//
//  TrackCylinder.hpp
//  my3dDawRendition
//
//  Created by Avrosh Kumar on 12/10/16.
//
//

#ifndef TrackCylinder_hpp
#define TrackCylinder_hpp

#include "ofMain.h"
#include "myUtils.hpp"
#include <random>

//using namespace std;


class TrackCylinder {
    

private:
//    TrackCylinder();
//    ~TrackCylinder();
    
    int radius = 10;
    int height = 700;
    int numPastFrames = 10;
    float releaseAlpha = 0.6;
    string trackName;
    int tick = 0;
    
  
    ofCylinderPrimitive trackCylinder;
    ofSpherePrimitive controlSphere;
//    ofPolyline trackAxis;
    ofPolyline fftBoundaryL[10],fftBoundaryR[10]; //numPastFrames = 10
//    ofPath fftBoundaryDraw;
    ofPath fftBoundaryDraw[10];
    vector<ofPoint> verticesL,verticesR, axisVertices;
    ofVec2f trackPos,specBoundR, specBoundL, trackPos1;
//    spectrogramBoundaryL[10],spectrogramBoundaryR[10];
    
    float initialI = 0.0;
    bool drawPanControl = false;
    
    int fftSize = 512;
    
    float spectrogramBlock[256][10];
//    , prevSpectrogramBlock[256];
//    maxSpectrogramBlock[256];
    
    void initRandomSpectrogram();
    
    float R,G,B;
    
    int getPrevTick(int index);
    void incrementTick();
   
    
public:
    void setup(float, float, string);
//    void setupPastFrames(int numFrames);
    void drawSpectrogram(int pastFrameNum, int currentTick);
    void draw();
    void update(ofVec2f pointer, ofCamera *cam);
    bool isFocussed(ofVec2f pointer, ofCamera *cam);
    void updateSpectrogram(vector <float> spectrogram, bool drawPastFrames);
    myUtils utils;
    
    float distRadius = 0;
    float panAngle = 0;
    
};


#endif /* TrackCylinder_hpp */
