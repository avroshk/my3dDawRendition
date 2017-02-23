//
//  TrackCylinder.cpp
//  my3dDawRendition
//
//  Created by Avrosh Kumar on 12/10/16.
//
//

#include "TrackCylinder.hpp"
#include "ofMain.h"
#include <math.h>

bool TrackCylinder::isPlaying = false;
int TrackCylinder::timeDistanceInPixels = 30;

void TrackCylinder::setTimeDistanceInPixels(int pix) {
    timeDistanceInPixels = pix;
}

void TrackCylinder::setIsPlaying(bool playing) {
    isPlaying = playing;
}
////--------------------------------------------------------------
//TrackCylinder::TrackCylinder() {
//    
//}
//
////--------------------------------------------------------------
//TrackCylinder::~TrackCylinder() {
//    
//}

//--------------------------------------------------------------
void TrackCylinder::setup(float r, float theta, string name){
    trackName = name;
    distRadius = r;
    panAngle = theta-90;
    trackCylinder.set(radius, height);
    trackPos = utils.polarToCartesian(distRadius, panAngle);
    
    R = ofRandom(0.0,1.0);
    G = ofRandom(0.0,1.0);
    B = ofRandom(0.0,1.0);
    
    controlSphere.set(30, 10);
//    trackAxis.addVertex(trackPos.x,-height/2,trackPos.y);
//    trackAxis.addVertex(trackPos.x,height/2,trackPos.y);
    
    initRandomSpectrogram();
    
    fftBoundaryL = new ofPolyline[numPastFrames];
    fftBoundaryR = new ofPolyline[numPastFrames];
    fftBoundaryDraw = new ofPath[numPastFrames];

    
//    setupPastFrames(numPastFrames);
    drawSpectrogram(0, tick); //draw 0th instance
}

//--------------------------------------------------------------
void TrackCylinder::initRandomSpectrogram(){
    
    spectrogramBlock = new float* [numPastFrames];
    for (int i=0; i< numPastFrames; i++) {
        spectrogramBlock[i] = new float [fftSize/2];
    }
    
    for (int i=0; i<numPastFrames; ++i) {
        for (int j=0; j<fftSize/2; ++j) {
            if (i==0) {
                spectrogramBlock[i][j] = ofRandom(0.0,10.0);
            } else {
                spectrogramBlock[i][j] = 0.0;
            }
        }
    }
    
    initialI = ofRandom(0.0,1.0)*180;
}

void TrackCylinder::clear() {
    cout<<"Clearing tracks"<<endl;
    
    
    for (int i=0; i< numPastFrames; i++) {
        delete [] spectrogramBlock[i];
    }
    delete [] spectrogramBlock;
    
    delete [] fftBoundaryL;
    delete [] fftBoundaryR;
    delete [] fftBoundaryDraw;
    
    appExited = true;
}

int TrackCylinder::getPrevTick(int index = -1) {
    if (index > -1) {
        if (index == 0) {
            return numPastFrames-1;
        } else {
            return index-1;
        }
    } else {
        if (tick == 0) {
            return numPastFrames-1;
        } else {
            return tick-1;
        }
    }
}

void TrackCylinder::incrementTick() {
    if ((tick+1) % numPastFrames == 0) {
        tick = 0;
    } else {
        tick++;
    }
}

//--------------------------------------------------------------
void TrackCylinder::updateSpectrogram(vector <float> spectrogram, bool drawPastFrames){
    
    if (!appExited) {
        float max = 0.0;
        for (int i=0; i<fftSize/2; ++i) {
            if (spectrogram[i] > max) {
                max = spectrogram[i];
            }
        }
        
        if (max < 0.000001) {
            max = 0.0001;
        }
        
        int prevTick = getPrevTick();
        
        //Normalize to 0-90
        for (int i=0; i<fftSize/2; ++i) {
            
            if (spectrogramBlock[prevTick][i] > spectrogramBlock[tick][i]) {
                //Release state
                spectrogramBlock[tick][i] = (1 - releaseAlpha) * spectrogramBlock[tick][i];
            } else {
                //Attack state
                spectrogramBlock[tick][i] = attackAlpha * spectrogram[i]*8/max + (1 - attackAlpha) * spectrogramBlock[prevTick][i];
            }
            
            spectrogramBlock[prevTick][i] = spectrogramBlock[tick][i];
        }
        
        incrementTick();
        
        //    setupPastFrames(numPastFrames);
        
        if (drawPastFrames) {
            for (int i=0; i< numPastFrames-1; i++) {
                drawSpectrogram(i, prevTick);
                prevTick = getPrevTick(prevTick);
            }
        } else {
            drawSpectrogram(0,tick);
        }
    }
}

//--------------------------------------------------------------
void TrackCylinder::update(ofVec2f mousePointer, ofCamera *cam){
    ofVec3f cur = cam->worldToScreen(ofPoint(trackPos.x,0,trackPos.y));
    
    
    while (abs(mousePointer.x - cur.x) > 11) {
        if (mousePointer.x < cur.x) {
            if (panAngle > -60-90 && panAngle < 60-90) {
                panAngle = panAngle - 1;
            } else {
                break;
            }
        } else {
            if (panAngle > -60-90 && panAngle < 60-90) {
                panAngle = panAngle  + 1;
            } else {
                break;
            }
        }
        trackPos = utils.polarToCartesian(distRadius, panAngle);
        cur = cam->worldToScreen(ofPoint(trackPos.x,0,trackPos.y));
    }
    
//    setupPastFrames(numPastFrames); //demoday
//    drawSpectrogram(0);

}


//--------------------------------------------------------------
void TrackCylinder::draw(){
    trackCylinder.setPosition(trackPos.x,0,trackPos.y);
    
//    ofDrawBitmapString(trackName, trackPos.x,20+(height/2),trackPos.y);

    
    if (drawPanControl) {
        ofSetColor(200,0,200,180);
        controlSphere.setPosition(trackPos.x,-height/2,trackPos.y);
        controlSphere.draw();
    }
    
    for (int j=0; j<numPastFrames; j++) {
        fftBoundaryDraw[j].draw();
    }
    if (!isPlaying) {
        trackCylinder.set(radius, height);
    } else {
        trackCylinder.set(radius/2, height);
    }
    trackCylinder.draw();
}

bool TrackCylinder::isFocussed(ofVec2f mousePointer, ofCamera *cam) {

    ofVec3f cur = cam->worldToScreen(ofPoint(trackPos.x,0,trackPos.y));

    if(abs(mousePointer.x-cur.x) < 11) {
        drawPanControl = true;
    } else {
        drawPanControl = false;
    }

    return drawPanControl;
}


void TrackCylinder::drawSpectrogram(int pastFrameNum, int currentTick) {
    if (!appExited) {
        fftBoundaryR[pastFrameNum].clear();
        fftBoundaryL[pastFrameNum].clear();
        fftBoundaryDraw[pastFrameNum].clear();
        
        int k = 0;
        float angle = 0;
        
        for (float i=0; i<height; i=i+height/128) {
            angle = spectrogramBlock[currentTick][k++]; //demoday
            
            specBoundR = utils.polarToCartesian(distRadius+pastFrameNum*timeDistanceInPixels, panAngle+angle);
            
            specBoundL = utils.polarToCartesian(distRadius+pastFrameNum*timeDistanceInPixels, panAngle-angle);
            
            fftBoundaryR[pastFrameNum].addVertex(specBoundR.x,(i-height/2),specBoundR.y);
            fftBoundaryL[pastFrameNum].addVertex(specBoundL.x,(i-height/2),specBoundL.y);
        }
        
        trackPos1 = utils.polarToCartesian(distRadius+pastFrameNum*timeDistanceInPixels, panAngle);
        
        verticesR = fftBoundaryR[pastFrameNum].getSmoothed(10).getVertices();
        verticesL = fftBoundaryL[pastFrameNum].getSmoothed(10).getVertices();
        
        fftBoundaryDraw[pastFrameNum].moveTo(trackPos1.x,(-height/2),trackPos1.y);
        
        for (int i=0; i<verticesR.size(); i++) {
            fftBoundaryDraw[pastFrameNum].lineTo(verticesR[i]);
        }
        
        fftBoundaryDraw[pastFrameNum].lineTo(trackPos1.x,(height/2),trackPos1.y);
        
        for (int i=verticesL.size()-1; i>=0; --i) {
            fftBoundaryDraw[pastFrameNum].lineTo(verticesL[i]);
        }
        
        fftBoundaryDraw[pastFrameNum].lineTo(trackPos1.x,(-height/2),trackPos1.y);
        fftBoundaryDraw[pastFrameNum].close();
        
        fftBoundaryDraw[pastFrameNum].setFillColor(ofFloatColor(R,G,B,0.6-(pastFrameNum*0.6/numPastFrames)));
        
        fftBoundaryDraw[pastFrameNum].setFilled(true);
    }
}



