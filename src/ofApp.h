#pragma once

#include "ofMain.h"
#include "TrackCylinder.hpp"
#include "TimeLine.hpp"
#include "myUtils.hpp"
#include "ofxOSC.h"
#include "ofxFft.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void setBrightTheme(bool);
    int pixelColor = 0;
    int fovInDegrees = 90;
    int nearDist = 0;
    int farDist = 20000;
    int cameraZOffset = 0;
    
    int infiniteLength;
    
    ofCamera cam;
    ofPolyline line;
    ofMaterial material;
    ofLight light;
    ofPlanePrimitive planeL, planeR;

    ofCylinderPrimitive trackCylinder;

    TimeLine time;
    float current_time;
    string beats;

    int numTracks = 6;
    TrackCylinder tracks[6];
    bool trackHoverTracker[6];
    
    //OSC
    ofxOscReceiver receive;
    ofxOscSender sender;
    float oscPan[6];
    
    //AUDIO
    void audioIn(float * input, int bufferSize, int nChannels);
    ofSoundStream soundStream;
    
    vector <float> fftData;
    vector <float> left1,right1,fftL1,fftR1;
    vector <float> left2,right2,fftL2,fftR2;
//    vector <float> left3,fftL3;
//    vector <float> left4,fftL4;
//    vector <float> left5,fftL5;
//    vector <float> left6,fftL6;
    
    //Spectrogram
    int plotHeight, fftBinSize;
    ofxFft* fft;
    float* currFft;
    
    //GUI
    ofxButton playButton;
    ofxButton stopButton;
    ofxToggle lockAbleton;
    ofxPanel gui;
    
    void playButtonPressed();
    void stopButtonPressed();
    
private:
//    ofVec2f polarPoint;
    ofVec2f trackPos;
    ofVec2f cartesianToPolar(int,int);
    ofVec2f polarToCartesian(float,float);
    myUtils utils;
//    vector<float> tempFft;
    
                             
};
