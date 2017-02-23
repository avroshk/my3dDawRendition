//
//  TimeLine.cpp
//  my3dDawRendition
//
//  Created by Avrosh Kumar on 1/19/17.
//
//

#include "TimeLine.hpp"
#include <math.h>

void TimeLine::setup(int sampleRate, int bufferSize) {
    numSamplesReceived = 0;
    timeInSec = 0;
    this->sampleRate = sampleRate;
    this->bufferSize = bufferSize;
    
    for (int i=0; i<3; i++) {
        measure[i] = 1;
    }
    measure[3] = 0;

//    rect.set(ofPoint(i*30,400,-800), 60, 60);

}

void TimeLine::updateCurrentMeasure(string beatString) {
    int measurePos = 0;
    int pos = 0;
    string token;
    char delimiter = '.';
    while ((pos = beatString.find(delimiter)) != std::string::npos) {
        token = beatString.substr(0, pos);
        measure[measurePos++] = stoi(token);
        beatString.erase(0, pos + 1);
    }
}

void TimeLine::updateTime() {
    numSamplesReceived+=bufferSize;
    
//    if (numSamplesReceived % sampleRate == 0) {
//        timeInSec++;
//    }
}

float TimeLine::getTimeinMs() {
    return floor((numSamplesReceived/sampleRate)*100);
}

int TimeLine::getMeasureInPixels() {
    for (int i=0; i<4; i++) {
        return ceil((measure[0]-1)*160 + (measure[1]-1)*40 + (measure[2]-1)*10 + measure[3]*0.1);
    }
}

void TimeLine::moveTimeline() {
    for (int i=0; i<20; i++) {
        rect[i].translate(getMeasureInPixels(), 0);
        ofDrawRectangle(rect[i]);
    }
}

void TimeLine::draw() {
    
    //Let's say 1 measure = 160px
    //1 beat = 40px
    //1 quater beat = 10px
    //1/8 beat = 0.1 px) rounded

//    int startPoint = 0;
//    ofTranslate(-pix, 0,0);
    
//    for (int i=0; i<20; i++) {
//        rect[i].set(ofPoint(i*30,460,-800), 60, 60);
//        ofDrawRectangle(rect[i]);
//    }

//        rect.translate(-pix, 0);
        
//        rect.set(0, 0, 50, 50);
        
//        ofRectangle(ofPoint(-30,370,-800-i*160),60, 60);
        
//        line.addVertex(ofPoint(-30,370,startPoint-i*160));
//        line.addVertex(ofPoint(30,370,startPoint-i*160));
//        line.addVertex(ofPoint(30,430,startPoint-i*160));
//        line.addVertex(ofPoint(-30,430,startPoint-i*160));
//        line.addVertex(ofPoint(-30,370,startPoint-i*160));
////        line.draw();
//
//       
//        line.addVertex(ofPoint(-15,385,startPoint-i*160-40));
//        line.addVertex(ofPoint(15,385,startPoint-i*160-40));
//        line.addVertex(ofPoint(15,415,startPoint-i*160-40));
//        line.addVertex(ofPoint(-15,415,startPoint-i*160-40));
//        line.addVertex(ofPoint(-15,385,startPoint-i*160-40));
////        line.draw();
//        
//        line.addVertex(ofPoint(-15,385,startPoint-i*160-80));
//        line.addVertex(ofPoint(15,385,startPoint-i*160-80));
//        line.addVertex(ofPoint(15,415,startPoint-i*160-80));
//        line.addVertex(ofPoint(-15,415,startPoint-i*160-80));
//        line.addVertex(ofPoint(-15,385,startPoint-i*160-80));
////        line.draw();
////
//        line.addVertex(ofPoint(-15,385,-800-i*160-120));
//        line.addVertex(ofPoint(15,385,-800-i*160-120));
//        line.addVertex(ofPoint(15,415,-800-i*160-120));
//        line.addVertex(ofPoint(-15,415,-800-i*160-120));
//        line.addVertex(ofPoint(-15,385,-800-i*160-120));
//        line.draw();

        
        
//        ofRectangle(ofPoint(-30,770,-i*160) , ofPoint(30,830,-i*160));
//        ofRectangle(ofPoint(-15,785,-i*160-40) , ofPoint(15,815,-i*160-40));
//        ofRectangle(ofPoint(-15,785,-i*160-80) , ofPoint(15,815,-i*160-80));
//        ofRectangle(ofPoint(-15,785,-i*160-120) , ofPoint(15,815,-i*160-120));
//    ofTranslate(0, 0,0);
//    if (getTimeinMs() % 1000.0 == 0) {
//        ofDrawLine(0,-800,-800-getTimeinMs(),0,800,-800-getTimeinMs());
//     }
}
