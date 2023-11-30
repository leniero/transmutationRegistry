/*
 
Project Title: transmutationRegistry
Description: Software companion to “Neogenesis Scanner”. NHH, “New Human Health” 2022.
Instructions: When not physically connected to Neogenesis Scanner, use keypress "S" to switch betwen modes.

©Leandro Niero 2022
leandroniero@mail.com
https://leandroniero.myportfolio.com/transmutationregistry
 
*/

#pragma once


#include "ofMain.h"
#include "ofxCvHaarFinder.h"
//#include "ofxThreadedImageLoader.h"


// listen on port
#define PORT 9600
#define NUM_MSG_STRINGS 20


class ofApp: public ofBaseApp
{
public:
    void setup();
    void setupTop();
    void update();
    void draw();
    void drawTop(ofEventArgs & args);
    void keyPressed(int key);

    ofxCvHaarFinder finder;
    
    ofVideoGrabber grabber;

    
    ofPixels videoInverted;
    ofTexture videoTexture;

    ofPixels videoEye;
    ofTexture videoEyeT;
    
    int camWidth;
    int camHeight;
    
    ofImage mask;
    ofVideoPlayer scan;
    
    bool newschannel;
    ofVideoPlayer newsVid;
    
    ofVideoPlayer instructionsVid;
    
    
    
    bool scanStart;
    int startTime;
    
    ofVideoPlayer topProj;
    ofVideoPlayer topWait;
    
    std::vector<ofImage> labels;
    std::vector<ofImage> questions;
    std::vector<ofImage> eyeData;
    
    ofImage yesno;
    ofImage results;
    ofImage compiling;
    
    

    ofSerial mySerial;
    int myByte;
};
