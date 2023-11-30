#include "ofApp.h"
#include "ofxPS3EyeGrabber.h"


void ofApp::setup(){
    
    // Set the custom ofxPS3EyeGrabber.
    grabber.setGrabber(std::make_shared<ofxPS3EyeGrabber>());
    grabber.setDesiredFrameRate(75);
    grabber.setup(640, 480);
    
    //eyetracking test
    finder.setup("haarcascade_eye.xml");
    
    
    grabber.getGrabber<ofxPS3EyeGrabber>()->setAutogain(false);
    grabber.getGrabber<ofxPS3EyeGrabber>()->setAutoWhiteBalance(false);
    
    //psEye resolution
    camWidth = 640;
    camHeight = 480;
    
    videoInverted.allocate(grabber.getWidth(), grabber.getHeight(), OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    
    ofSetVerticalSync(true);
    
    
    //load graphic assets
    mask.load("mask.png");
    scan.load("scan.mov");
    newsVid.load("newschannel.mov");
    
    instructionsVid.setLoopState(OF_LOOP_NONE);
    instructionsVid.load("instructions.mov");
    instructionsVid.setLoopState(OF_LOOP_NONE);
    
    yesno.load("yesno.png");
    results.load("results.png");
    compiling.load("compiling.png");
    
    ofSetCircleResolution(100);
    //ofSetDepthTest(true); // disable this for smooth colour // enable for shards of colour
    
    
    // create vector of symbols
    int numOfImages = 10;
    for(int i = 0; i < numOfImages; i++) {
        ofImage newImage;
        string imageName = "label" + ofToString(i) + ".png";
        newImage.load(imageName);
        labels.push_back(newImage);
    }
    
    int numOfImages2 = 20;
    for(int i = 0; i < numOfImages2; i++) {
        ofImage newImage;
        string imageName = "question" + ofToString(i) + ".png";
        newImage.load(imageName);
        questions.push_back(newImage);
    }
    
    
    int numOfImages3 = 10;
    for(int i = 0; i < numOfImages3; i++) {
        ofImage newImage;
        string imageName = "eyetext" + ofToString(i) + ".png";
        newImage.load(imageName);
        eyeData.push_back(newImage);
    }
    
    
    //setup serial communication with arduino
    mySerial.listDevices();
    
    mySerial.setup(1, 9600);
    myByte = 0;
    scanStart == false;
    
}

//--------------------------------------------------------------
void ofApp::setupTop(){
    
    //load graphic assets for top projection
    topProj.load("topProj.mov");
    topWait.load("topWait.mov");
}

//--------------------------------------------------------------

void ofApp::update(){
    grabber.update();
    
    //finder.findHaarObjects(grabber.getPixels());
    
    
    //create negative of psEye
    if(grabber.isFrameNew()){
        ofPixels & pixels = grabber.getPixels();
        for(size_t i = 0; i < pixels.size(); i++){
            //invert the color of the pixel
            videoInverted[i] = 255 - pixels[i];
        }
        //load the inverted pixels
        videoTexture.loadData(videoInverted);
    }
    
    
    //update video assets
    scan.update();
    newsVid.update();
    
    instructionsVid.setLoopState(OF_LOOP_NONE);
    instructionsVid.update();
    
    topProj.update();
    topWait.update();
    
    
    
    
    //read values from arduino sensor
    if(mySerial.available()){
        myByte = mySerial.readByte();
        
        cout<< myByte << endl;
    }
    
    
    
}

//--------------------------------------------------------------

void ofApp::draw(){
    ofSetHexColor(0xffffff);
    ofBackground(0);
    
    
    
    //RESET TIME ELLAPSED OFTEN
    if (ofGetElapsedTimeMillis() > 1500 & myByte > ofRandom(170,250) & scanStart == false){
        scanStart = !scanStart;
        startTime = ofGetElapsedTimeMillis();
    }
    
    
    //trigger scan instructions
    if (scanStart){
        
        newsVid.stop();
        instructionsVid.play();
        instructionsVid.draw(0,0);
        
        //trigger scan interface
        if (ofGetElapsedTimeMillis() > (startTime + 33600)) {
            
          
            scan.play();
            
            ofPushMatrix();
            ofScale(1.4);
            
            //psEye video
            ofRotateZ(-24);
            videoTexture.draw(-ofGetWidth()*0.32,-ofGetHeight()*0.17);
            ofPopMatrix();
            
            //mask for interface layout
            mask.draw(0, 0);
            
            //brain morphing animation
            ofPushMatrix();
            ofScale(0.75);
            scan.draw(0+(ofGetWidth()*0.75),0);
            ofPopMatrix();
            
            ofPushMatrix();
            ofPushStyle();
            float time = ofGetElapsedTimef()*3.;
            
            
            //generate waves of brain data
            ofRotateZ(90);
            ofTranslate(ofGetHeight()/1.5, -ofGetWidth()*1.2, 0);
            
            ofScale(0.15,3);
            
            for (int x = 1; x < 200; x ++){
                
                
                for (int i=0; i < 900; i+=4){
                    
                    
                    ofSetColor(ofRandom(255) + ofRandom(255) * sin(i * 0.01 + time + x ) * sin(i * 0.01 + time*0.5 + x)*2.,ofRandom(255) + ofRandom(255) * sin(i * 0.011 + time ),ofRandom(255) + ofRandom(255) * sin(i * 0.012 + time ));
                    
                    
                    
                    ofDrawCircle( 500 * x + 200 * sin( i * 0.08 + time + x) * sin(i * 0.01 + time*0.5 + x) * ofNoise(i * 0.01 + time*2.5 + x)*5., 50 + i , 5 +  10 * sin( i * 0.0005 + time + x) ) ;
                }
            }
            ofPopStyle();
            ofPopMatrix();
            
            
            //draw alien symbols
            int randomNr = ofRandom(10);
            labels[randomNr].draw(0, 0);
            
            int randomNr2 = ofRandom(20);
            questions[randomNr2].draw(0, 0);
          
            
            int randomNr3 = ofRandom(10);
            eyeData[randomNr3].draw(0, 0);
       
            
            yesno.draw(0,0);
            
            
            
        }
        
        //clear interface at end of scan
        if (ofGetElapsedTimeMillis() > (startTime + 205500)) {
            
            scan.stop();
            
            videoTexture.unbind();
            videoTexture.clear();
            ofClear(0,255);
            
            instructionsVid.draw(0,0);
           
        }
        
        
        //show results
        if (ofGetElapsedTimeMillis() > (startTime + 209500)) {
            
            compiling.draw(0,0);
            
        }

        if (ofGetElapsedTimeMillis() > (startTime + 217000)) {
            
            results.draw(0,0);
            
        }
        
        if (ofGetElapsedTimeMillis() > (startTime + 233000)) {
            scanStart = !scanStart;
        }
    }
    
    //if not scanning, play broadcast
    else{
        
        scan.stop();
        instructionsVid.stop();
        newsVid.play();
        newsVid.draw(0,0);
        
    }
    
    //reset time to keep variable low
    if (ofGetElapsedTimeMillis() >  16000 & scanStart == false) {
        
        ofResetElapsedTimeCounter();
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::drawTop(ofEventArgs & args){
    
    //draw animation for top projection
    ofBackground(0);
    
    ofScale(1.5);
    
    if (scanStart){
        topWait.stop();
        topProj.play();
        topProj.draw(280,0);
        
    }else{
        topProj.stop();
        topWait.play();
        topWait.draw(280,0);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    newschannel = !newschannel;
    
    //trigger scan
    if (key == 's')
        scanStart = !scanStart;
    
    startTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
