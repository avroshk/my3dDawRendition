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
//    setupPastFrames(numPastFrames);
    drawSpectrogram(0, tick); //draw 0th instance
}

//--------------------------------------------------------------
void TrackCylinder::initRandomSpectrogram(){
    for (int j=0; j<numPastFrames; j++) {
        for (int i=0; i<fftSize/2; ++i) {
            if (j==0) {
                spectrogramBlock[i][j] = ofRandom(0.0,10.0);
            } else {
                spectrogramBlock[i][j] = 0.0;
            }
        }
    }
    
    initialI = ofRandom(0.0,1.0)*180;
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
        
        if (spectrogramBlock[i][prevTick] > spectrogramBlock[i][tick]) {
            //Release state
            spectrogramBlock[i][tick] = (1 - releaseAlpha) * spectrogramBlock[i][tick];
        } else {
            //Attack state
            spectrogramBlock[i][tick] = releaseAlpha * spectrogram[i]*8/max + (1 - releaseAlpha) * spectrogramBlock[i][prevTick];
        }
        
        spectrogramBlock[i][prevTick] = spectrogramBlock[i][tick];
    }
    
    incrementTick();

//    for (int i=0; i<fftSize/2; ++i) {
//        spectrogramBlock[i] = 10*(spectrogram[i]);
//    }
//
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
    trackCylinder.draw();
    
//    ofDrawBitmapString(trackName, trackPos.x,20+(height/2),trackPos.y);

    
    if (drawPanControl) {
        ofSetColor(200,0,200,180);
        controlSphere.setPosition(trackPos.x,-height/2,trackPos.y);
        controlSphere.draw();
    }
    
    for (int j=0; j<numPastFrames-1; j++) {
        fftBoundaryDraw[j].draw();
    }
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
    
    fftBoundaryR[pastFrameNum].clear();
    fftBoundaryL[pastFrameNum].clear();
    fftBoundaryDraw[pastFrameNum].clear();
    
    int k = 0;
    float angle = 0;
    
    for (float i=0; i<height; i=i+height/128) {
        angle = spectrogramBlock[k++][currentTick]; //demoday

        specBoundR = utils.polarToCartesian(distRadius+pastFrameNum*100, panAngle+angle);
        
        specBoundL = utils.polarToCartesian(distRadius+pastFrameNum*100, panAngle-angle);
        
        fftBoundaryR[pastFrameNum].addVertex(specBoundR.x,(i-height/2)+0*100,specBoundR.y);
        fftBoundaryL[pastFrameNum].addVertex(specBoundL.x,(i-height/2)+0*100,specBoundL.y);
    }
    
    trackPos1 = utils.polarToCartesian(distRadius+pastFrameNum*100, panAngle);
    
    verticesR = fftBoundaryR[pastFrameNum].getSmoothed(10).getVertices();
    verticesL = fftBoundaryL[pastFrameNum].getSmoothed(10).getVertices();
    
    fftBoundaryDraw[pastFrameNum].moveTo(trackPos1.x,(-height/2)+0*100,trackPos1.y);
    
    for (int i=0; i<verticesR.size(); i++) {
        fftBoundaryDraw[pastFrameNum].lineTo(verticesR[i]);
    }
    
    fftBoundaryDraw[pastFrameNum].lineTo(trackPos1.x,(height/2)+0*100,trackPos1.y);
    
    for (int i=verticesL.size()-1; i>=0; --i) {
        fftBoundaryDraw[pastFrameNum].lineTo(verticesL[i]);
    }
    
    fftBoundaryDraw[pastFrameNum].lineTo(trackPos1.x,(-height/2)+0*100,trackPos1.y);
    fftBoundaryDraw[pastFrameNum].close();
    
    fftBoundaryDraw[pastFrameNum].setFillColor(ofFloatColor(R,G,B,0.6-(pastFrameNum*0.06)));
    
    fftBoundaryDraw[pastFrameNum].setFilled(true);
}

/*
void TrackCylinder::setupPastFrames(int numFrames) {
    
    for (int n=0; n<numFrames; n++) {
        fftBoundaryR[n].clear();
        fftBoundaryL[n].clear();
        fftBoundaryDraw[n].clear();
    }
    
//    float tempRandomValue = 0.0;
    
    int k = 0;
    float angle = 0;
    for (float i=0; i<height; i=i+height/128) {
//        tempRandomValue = ofRandom(0.0,10.0);
//        tempRandomValue = spectrogramBlock[k++];
//        angle = tempRandomValue*(sin((initialI+i)*pi/180)+sin(pi/2));
        
        angle = spectrogramBlock[k++]; //demoday
        
        for (int j=0; j<numFrames; j++) {
            
            if (j==0) {
                spectrogramBoundaryR[j] = utils.polarToCartesian(distRadius+j*100, panAngle+angle);
                
                spectrogramBoundaryL[j] = utils.polarToCartesian(distRadius+j*100, panAngle-angle);
                
                fftBoundaryR[j].addVertex(spectrogramBoundaryR[j].x,(i-height/2)+j*100,spectrogramBoundaryR[j].y);
                fftBoundaryL[j].addVertex(spectrogramBoundaryL[j].x,(i-height/2)+j*100,spectrogramBoundaryL[j].y);
            } else {
                spectrogramBoundaryR[j] = utils.polarToCartesian(distRadius+j*100+1000, panAngle+angle);
                
                spectrogramBoundaryL[j] = utils.polarToCartesian(distRadius+j*100+1000, panAngle-angle);
                
                fftBoundaryR[j].addVertex(spectrogramBoundaryR[j].x,(i-height/2)+j*100-200,spectrogramBoundaryR[j].y);
                fftBoundaryL[j].addVertex(spectrogramBoundaryL[j].x,(i-height/2)+j*100-200,spectrogramBoundaryL[j].y);
            }
            
        }
    }
    
    for (int j=0; j<numFrames; j++) {
        
        
        if (j==0) {
            trackPos = utils.polarToCartesian(distRadius+j*100, panAngle);
            
            verticesR = fftBoundaryR[j].getSmoothed(1).getVertices();
            verticesL = fftBoundaryL[j].getSmoothed(1).getVertices();
            
            fftBoundaryDraw[j].moveTo(trackPos.x,(-height/2)+j*100,trackPos.y);
            
            for (int i=0; i<verticesR.size(); i++) {
                fftBoundaryDraw[j].lineTo(verticesR[i]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(height/2)+j*100,trackPos.y);
            
            for (int i=verticesL.size()-1; i>=0; --i) {
                fftBoundaryDraw[j].lineTo(verticesL[i]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(-height/2)+j*100,trackPos.y);
            fftBoundaryDraw[j].close();
            
            fftBoundaryDraw[j].setFillColor(ofFloatColor(R,G,B,0.6-(j*0.06)));
        } else {
            trackPos = utils.polarToCartesian(distRadius+j*100+1000, panAngle);
            
            verticesR = fftBoundaryR[j].getSmoothed(1).getVertices();
            verticesL = fftBoundaryL[j].getSmoothed(1).getVertices();
            
            fftBoundaryDraw[j].moveTo(trackPos.x,(-height/2)+j*100-200,trackPos.y);
            
            for (int l=0; l<verticesR.size(); l++) {
                fftBoundaryDraw[j].lineTo(verticesR[l]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(height/2)+j*100-200,trackPos.y);
            
            for (int l=verticesL.size()-1; l>=0; --l) {
                fftBoundaryDraw[j].lineTo(verticesL[l]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(-height/2)+j*100-200,trackPos.y);
            fftBoundaryDraw[j].close();
            
            fftBoundaryDraw[j].setFillColor(ofFloatColor(1.0,1.0,1.0,0.6-(j*0.06)));
        }
        
        fftBoundaryDraw[j].setFilled(true);
    }
}
*/


/*
void TrackCylinder::setupPastFrames(int numFrames) {
    
    for (int n=0; n<numFrames; n++) {
        fftBoundaryR[n].clear();
        fftBoundaryL[n].clear();
        fftBoundaryDraw[n].clear();
    }
    
    float tempRandomValue = 0.0;
    
    int k = 0;
    float angle = 0;
    for (float i=0; i<height; i=i+height/128) {
        tempRandomValue = ofRandom(0.0,10.0);
        angle = tempRandomValue*(sin((initialI+i)*pi/180)+sin(pi/2));
//        angle = spectrogramBlock[k++]; //demoday
        
        for (int j=0; j<numFrames; j++) {
            
            if (j==0) {
                spectrogramBoundaryR[j] = utils.polarToCartesian(distRadius+j*100, panAngle+angle);
                
                spectrogramBoundaryL[j] = utils.polarToCartesian(distRadius+j*100, panAngle-angle);
                
                fftBoundaryR[j].addVertex(spectrogramBoundaryR[j].x,(i-height/2)+j*100,spectrogramBoundaryR[j].y);
                fftBoundaryL[j].addVertex(spectrogramBoundaryL[j].x,(i-height/2)+j*100,spectrogramBoundaryL[j].y);
            } else {
                spectrogramBoundaryR[j] = utils.polarToCartesian(distRadius+j*100+1000, angle);
                
                spectrogramBoundaryL[j] = utils.polarToCartesian(distRadius+j*100+1000, -angle);
                
                fftBoundaryR[j].addVertex(spectrogramBoundaryR[j].x,(i-height/2)+j*100+1000,spectrogramBoundaryR[j].y);
                fftBoundaryL[j].addVertex(spectrogramBoundaryL[j].x,(i-height/2)+j*100+1000,spectrogramBoundaryL[j].y);
            }
            
        }
    }
    
    for (int j=0; j<numFrames; j++) {
        
        
        if (j==0) {
            trackPos = utils.polarToCartesian(distRadius+j*100, panAngle);
            
            verticesR = fftBoundaryR[j].getSmoothed(5).getVertices();
            verticesL = fftBoundaryL[j].getSmoothed(5).getVertices();
            
            fftBoundaryDraw[j].moveTo(trackPos.x,(-height/2)+j*100,trackPos.y);
            
            for (int i=0; i<verticesR.size(); i++) {
                fftBoundaryDraw[j].lineTo(verticesR[i]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(height/2)+j*100,trackPos.y);
            
            for (int i=verticesL.size()-1; i>=0; --i) {
                fftBoundaryDraw[j].lineTo(verticesL[i]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(-height/2)+j*100,trackPos.y);
            fftBoundaryDraw[j].close();
            
            fftBoundaryDraw[j].setFillColor(ofFloatColor(R,G,B,0.6-(j*0.06)));
        } else {
            trackPos = utils.polarToCartesian(distRadius+j*100+1000, panAngle);
            
            verticesR = fftBoundaryR[j].getSmoothed(5).getVertices();
            verticesL = fftBoundaryL[j].getSmoothed(5).getVertices();
            
            fftBoundaryDraw[j].moveTo(trackPos.x,(-height/2)+j*100+1000,trackPos.y);
            
            for (int l=0; l<verticesR.size(); l++) {
                fftBoundaryDraw[j].lineTo(verticesR[l]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(height/2)+j*100+1000,trackPos.y);
            
            for (int l=verticesL.size()-1; l>=0; --l) {
                fftBoundaryDraw[j].lineTo(verticesL[l]);
            }
            
            fftBoundaryDraw[j].lineTo(trackPos.x,(-height/2)+j*100+1000,trackPos.y);
            fftBoundaryDraw[j].close();
            
            fftBoundaryDraw[j].setFillColor(ofFloatColor(1.0,1.0,1.0,0.6-(j*0.06)));
        }
        
        fftBoundaryDraw[j].setFilled(true);
    }
}
 */
