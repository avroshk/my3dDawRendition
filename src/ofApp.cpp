#include "ofApp.h"
#define kRotInc 5
#define kMoveInc 100
#define RECEIVE_PORT 9001
#define SEND_PORT 9000

//--------------------------------------------------------------
void ofApp::setup(){
    setBrightTheme(false);
    
    infiniteLength = ofGetWidth()*10;

    //(vertical Flip, field of view, near , far )
    
    cam.setupPerspective(false,fovInDegrees,nearDist,farDist);
    cam.setPosition(0,0,cameraZOffset);
    
    //Shed some light
    ofSetSmoothLighting(true);
    light.setPosition(0, ofGetHeight()/2,1000);
    light.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    light.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));
    //Atmospheric light
    light.setAmbientColor(ofFloatColor(1.f,0.f,0.f));
    
    //Set material for outer sphere
    // shininess is a value between 0 - 128, 128 being the most shiny //
    material.setShininess( 12 );
    // the light highlight of the material //
    material.setSpecularColor(ofFloatColor(0.f,1.f,0.f));
    
    //Setup plane
    planeL.set(600, 600);
    planeR.set(600, 600);
    planeL.rotate(90, 0.0, 1.0, 0.0);
    planeR.rotate(-90, 0.0, 1.0, 0.0);
    planeL.setPosition(utils.polarToCartesian(800, -190).x, 0, utils.polarToCartesian(800, -190).y);
    planeR.setPosition(utils.polarToCartesian(800, 10).x, 0, utils.polarToCartesian(800, 10).y);

    //Set track cylinders
    oscPan[0] = -50;
    oscPan[1] = -20;
    oscPan[2] = 0;
    oscPan[3] = 2;
    oscPan[4] = 45;
    oscPan[5] = 50;
   
    
    //OSC setup
    sender.setup("localhost", SEND_PORT);
    receive.setup(RECEIVE_PORT);
    
    tracks[0].setup(800,oscPan[0],"GuitaL");
    tracks[1].setup(800,oscPan[1],"Vox");
    tracks[2].setup(800,oscPan[2],"Bass");
    tracks[3].setup(800,oscPan[3],"Kick");
    tracks[4].setup(800,oscPan[4],"Synth");
    tracks[5].setup(800,oscPan[5],"GuitarR");


    //Set rendering resolutions
    ofSetCircleResolution(100);
    
    //Audio setup
    
    // 0 output channels,
    // 2 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    int bufferSize = 512;
    int numChannels = 4;
    int sampleRate = 44100;
    
    left1.assign(bufferSize, 0.0);
    left2.assign(bufferSize, 0.0);
    right1.assign(bufferSize, 0.0);
    right2.assign(bufferSize, 0.0);
//    left3.assign(bufferSize, 0.0);
//    left4.assign(bufferSize, 0.0);
//    left5.assign(bufferSize, 0.0);
//    left6.assign(bufferSize, 0.0);

    soundStream.printDeviceList();
    soundStream.setDeviceID(3);
    
    //Initialize FFT
    fft = ofxFft::create(bufferSize, OF_FFT_WINDOW_HANN, OF_FFT_BASIC);
    fftBinSize = fft->getBinSize();
    
    fftL1.assign(fftBinSize, 0.0);
    fftL2.assign(fftBinSize, 0.0);
    fftR1.assign(fftBinSize, 0.0);
    fftR2.assign(fftBinSize, 0.0);
//    fftL3.assign(fftBinSize, 0.0);
//    fftL4.assign(fftBinSize, 0.0);
//    fftL5.assign(fftBinSize, 0.0);
//    fftL6.assign(fftBinSize, 0.0);
    
    soundStream.setup(this, 0, numChannels, sampleRate, bufferSize, 4);
    
    
    //Timeline setup
    time.setup(sampleRate,bufferSize);
    
    //GUI Setup
    playButton.addListener(this, &ofApp::playButtonPressed);
    stopButton.addListener(this, &ofApp::stopButtonPressed);
    
    gui.setup(); // most of the time you don't need a name
    gui.add(lockAbleton.setup("Lock", false));
    gui.add(playButton.setup("Play"));
    gui.add(stopButton.setup("Stop"));
    
}


//--------------------------------------------------------------
void ofApp::update(){
    while (receive.hasWaitingMessages()) {
        ofxOscMessage m;
        receive.getNextMessage(&m);
        
        /// TODO: Split these into functions
        for (int i=1; i<=numTracks; i++) {
            if (m.getAddress() == "/pan/track"+std::to_string(i)) {
                oscPan[i-1] = m.getArgAsFloat(0);
            } else if(m.getAddress() == "/transport/beats") {
                beats = m.getArgAsString(0);
                time.updateCurrentMeasure(beats);
            } else if(m.getAddress() == "/live/time") {
                current_time = m.getArgAsFloat(0);
            }
            
        }
    }
    time.moveTimeline();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofTranslate(0,0,0);
    ofEnableAlphaBlending();
    ofSetColor(pixelColor,pixelColor,pixelColor,pixelColor);
   
    
    ofEnableLighting();
    light.enable();

    cam.begin();
    light.draw();
    
    ofDrawAxis(100);
    
    // draw the outer sphere
    material.begin();
    ofNoFill();
//    ofFill();
    ofDrawSphere(0, 0, infiniteLength);
    material.end();
    
    //Draw plane
    ofNoFill();
//    ofTranslate(0,0,-time.getMeasureInPixels());
//    time.draw();
    ofTranslate(0,0,0);
    

    planeL.draw();
    planeR.draw();
    
    tracks[0].draw();
    tracks[1].draw();
    tracks[2].draw();
    tracks[3].draw();
//    tracks[4].draw();
//    tracks[5].draw();

    
    
    ofDrawBitmapString("Lows", utils.polarToCartesian(800,-135).x, -330, utils.polarToCartesian(800,-135).y);
    ofDrawBitmapString("Mids", utils.polarToCartesian(800,-135).x, -100, utils.polarToCartesian(800,-135).y);
    ofDrawBitmapString("Highs", utils.polarToCartesian(800,-135).x, 135, utils.polarToCartesian(800,-135).y);
    
    ofDrawLine(0,-800,-800,0,800,-800);
    ofDrawBitmapString("Pan Center", -60,-500,-800);
    
    ofDrawBitmapString("Beats: " + beats, 0, 10);
    ofDrawBitmapString("Time: " + std::to_string(current_time), 0, 30);
    
     ofDrawBitmapString("Lows", utils.polarToCartesian(800,-135).x, -330, utils.polarToCartesian(800,-135).y);
    
    ofPushMatrix();
    ofRotateX(90);  // <- rotate the circle around the z axis by some amount.
    //Top and bottom of spectrum boundaries
    ofDrawCircle(0,0,-350,800);
    ofDrawCircle(0,0,350,800);
    
//    ofSetColor(0,255,0,180);
    
    ofDrawCircle(0,0,-115,800);
    ofDrawCircle(0,0,115,800);
    ofPopMatrix();
    cam.end();
    


    
    ofDrawBitmapString("3d Daw Rendition", 20, 20);
//    
//    ofDrawBitmapString(
//                       "Debug Space ("+std::to_string(mouseX)+","+std::to_string(mouseY)+")"
//                       , 300, 20);
    
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
            
        case OF_KEY_LEFT: cam.pan(kRotInc); break;
        case OF_KEY_RIGHT: cam.pan(-kRotInc); break;
        case OF_KEY_UP: cam.tilt(-kRotInc); break;
        case OF_KEY_DOWN: cam.tilt(kRotInc); break;
        case 'q': {
                ofxOscMessage m;
                m.setAddress("/ableton/free");
                if (lockAbleton) {
                    m.addInt32Arg(0);
                } else {
                    m.addInt32Arg(1);
                }
                sender.sendMessage(m);
                break;
            }
        case 'p': break;
        case 'l': break;
        case ',': cam.roll(kRotInc); break;
        case '.': cam.roll(-kRotInc); break;
        case 'a': cam.truck(-kMoveInc); break;
        case 'd': cam.truck(kMoveInc); break;
            
        case 'w': cam.dolly(-kMoveInc); break;
        case 's': cam.dolly(kMoveInc); break;
            
        case 'r': cam.boom(kMoveInc); break;
        case 'f': cam.boom(-kMoveInc); break;
            
        case 'z':
            cam.resetTransform();
            cam.setPosition(0,0,cameraZOffset);
            break;
            
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for (int i=0 ; i<numTracks; i++) {
        trackHoverTracker[i] = tracks[i].isFocussed(ofVec2f(x,y), &cam);
    }
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    ofxOscMessage m;
   
    for (int i=0 ; i<numTracks; i++) {
        if (trackHoverTracker[i]) {
            tracks[i].update(ofVec2f(x,y), &cam);
            m.setAddress("/pan/track"+std::to_string(i));
            m.addInt32Arg(tracks[i].panAngle+90);
            sender.sendMessage(m);
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::exit(){
    playButton.removeListener(this, &ofApp::playButtonPressed);
    stopButton.removeListener(this, &ofApp::stopButtonPressed);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::setBrightTheme(bool setBright){
    if (setBright) {
        ofBackground(255,255,255);
        pixelColor = 30;
    } else {
        ofBackground(0,0,0);
        pixelColor = 200;
    }
}

//--------------------------------------------------------------
void ofApp::playButtonPressed(){
    ofxOscMessage m;
    
//    m.setAddress("/transport/play");
    m.setAddress("/live/play");
    m.addInt32Arg(1);
    sender.sendMessage(m);
}

//--------------------------------------------------------------
void ofApp::stopButtonPressed(){
    ofxOscMessage m;

    m.setAddress("/transport/stop");
    m.addInt32Arg(1);
    sender.sendMessage(m);
}


//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
//    time.updateTime();
    
    //lets go through each sample
    for (int i = 0; i < bufferSize; i++){
        left1[i]	= input[i*nChannels+0]*0.9;
        right1[i]	= input[i*nChannels+1]*0.9;
        left2[i]	= input[i*nChannels+2]*0.9;
        right2[i]	= input[i*nChannels+3]*0.9;
//        left3[i]	= input[i*nChannels+4]*0.5;
//        left4[i]	= input[i*nChannels+5]*0.5;
//        left5[i]	= input[i*nChannels+6]*0.5;
//        left6[i]	= input[i*nChannels+7]*0.5;
    }
    
    fft->setSignal(left1);
    currFft = fft->getAmplitude();
    copy(currFft, currFft + fft->getBinSize(), fftL1.begin());
    
    fft->setSignal(right1);
    currFft = fft->getAmplitude();
    copy(currFft, currFft + fft->getBinSize(), fftR1.begin());
    
    fft->setSignal(left2);
    currFft = fft->getAmplitude();
    copy(currFft, currFft + fft->getBinSize(), fftL2.begin());
    
    fft->setSignal(right2);
    currFft = fft->getAmplitude();
    copy(currFft, currFft + fft->getBinSize(), fftR2.begin());
//
//    
//    fft->setSignal(left3);
//    currFft = fft->getAmplitude();
//    copy(currFft, currFft + fft->getBinSize(), fftL3.begin());
//
//    
//    fft->setSignal(left4);
//    currFft = fft->getAmplitude();
//    copy(currFft, currFft + fft->getBinSize(), fftL4.begin());
//    
//    
//    fft->setSignal(left5);
//    currFft = fft->getAmplitude();
//    copy(currFft, currFft + fft->getBinSize(), fftL5.begin());
//    
//    
//    fft->setSignal(left6);
//    currFft = fft->getAmplitude();
//    copy(currFft, currFft + fft->getBinSize(), fftL6.begin());
    
    //demoday
    tracks[0].updateSpectrogram(fftL1, true);
    tracks[1].updateSpectrogram(fftR1, true);
    tracks[2].updateSpectrogram(fftL2, true);
    tracks[3].updateSpectrogram(fftR2, true);
//    tracks[4].updateSpectrogram(fftL5);
//    tracks[5].updateSpectrogram(fftL6);

}
