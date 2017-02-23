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
    float releaseAlpha = 0.05;
    float attackAlpha = 0.9;
    string trackName;
    int tick = 0;
    bool appExited = false;
    int numPastFrames = 10;
    static int timeDistanceInPixels;
    

    
  
    ofCylinderPrimitive trackCylinder;
    ofSpherePrimitive controlSphere;
//    ofPolyline trackAxis;
    ofPolyline *fftBoundaryL,*fftBoundaryR;
//    ofPath fftBoundaryDraw;
    ofPath *fftBoundaryDraw;
    vector<ofPoint> verticesL,verticesR, axisVertices;
    ofVec2f trackPos,specBoundR, specBoundL, trackPos1;
//    spectrogramBoundaryL[10],spectrogramBoundaryR[10];
    
    float initialI = 0.0;
    bool drawPanControl = false;
    
    int fftSize = 512;
    
    float **spectrogramBlock;
    
    void initRandomSpectrogram();
    
    float R,G,B;
    
    int getPrevTick(int index);
    void incrementTick();
    static bool isPlaying;

   
    
public:
    void setup(float, float, string);
//    void setupPastFrames(int numFrames);
    void drawSpectrogram(int pastFrameNum, int currentTick);
    void draw();
    void clear();
    
    void update(ofVec2f pointer, ofCamera *cam);
    bool isFocussed(ofVec2f pointer, ofCamera *cam);
    void updateSpectrogram(vector <float> spectrogram, bool drawPastFrames);
    myUtils utils;
    
    float distRadius = 0;
    float panAngle = 0;
    static void setIsPlaying(bool);
    static void setTimeDistanceInPixels(int);
    
};





#endif /* TrackCylinder_hpp */
