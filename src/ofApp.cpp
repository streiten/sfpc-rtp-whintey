#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofSetFrameRate(30);
    ofBackground(0);
    ofEnableSmoothing();
    
    bckgImg.loadImage("back_rotate.png");
    triImg.loadImage("tri.png");
    eyesImg.loadImage("1f440.png");

    ofApp::initControls();
    
    anim[0].reset( 0.0f );
    anim[0].setRepeatType(LOOP);
    anim[0].setCurve(LINEAR);
    anim[0].animateTo( TWO_PI );
    
    anim[1].reset( 0.0f );
    anim[1].setRepeatType(LOOP);
    anim[1].setCurve(LINEAR);
    anim[1].animateTo( TWO_PI );
    
    rgbaFbo.allocate(1080, 1920, GL_RGBA);
    rgbaFbo.begin();
        ofClear(255,255,255, 0);
    rgbaFbo.end();
    
    center.set(rgbaFbo.getWidth()*0.5f, rgbaFbo.getHeight()*0.5f, 0);
    
    recLoopDone = false;
    lastPhase = 0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    this->imgScales[1] = pImgScale[1]* 0.1;
    this->imgScales[0] = pImgScale[0]* 0.1;
    
    anim[0].update( 1.0f/pAnimSpeed[0] );
    anim[1].update( 1.0f/pAnimSpeed[1] );

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float seq;
    
    rgbaFbo.begin();
    
    ofClear(0);
    bckgImg.draw(0, 0);
    
    ofPushMatrix();
    ofTranslate(center);
    ofSetColor(255);
    ofSetLineWidth(1);
    
    if(pShow[0]) {
        drawWave();
    }
    
    if(pShow[1]) {
        seq = floor(drawLissaous() * 1000);
    }
    ofPopMatrix();
    
    
    rgbaFbo.end();
    rgbaFbo.draw(0,0, ofGetWidth(), ofGetHeight());
    
    if(pRec && !recLoopDone) {
        ofPixels p;
        rgbaFbo.readToPixels(p);
        ofSaveImage(p, "./capture/" + ofToString(seq) + ofGetTimestampString() + ".png");
        if(seq < lastPhase) {
            recLoopDone = true;
        }
        lastPhase = seq;
    }
    
    gui.draw();
    
}

void ofApp::saveFrame(){
    ofPixels p;
    rgbaFbo.readToPixels(p);
    ofSaveImage(p, "./capture/" + ofGetTimestampString() + ".jpg");
}

void ofApp::initControls(){

    gui.setup("Parameters");
    
    paramsGroup1.setName("Group 1");
    paramsGroup1.add(pShow[0].set( "Show/Hide", true ));
    paramsGroup1.add(pAmp[0].set( "Amplitude", 150, 0, 300 ));
    paramsGroup1.add(pFreq[0].set( "Frequency", 1, 1, 10 ));
    // paramsGroup1.add(pPhase[0].set("Phase", 0,0,TWO_PI));
    paramsGroup1.add(pDensity[0].set("Density", 50, 0, 200));
    paramsGroup1.add(pAnimSpeed[0].set("Animation Speed", 960, 30, 960));
    paramsGroup1.add(pImgScale[0].set("Image scale", 1, 1, 50));
    // paramsGroup1.add(pJitterScale[0].set("Jitter scale", true));

    paramsGroup2.setName("Group 2");
    paramsGroup2.add(pShow[1].set( "Show/Hide", true ));
    paramsGroup2.add(pAmp[1].set( "Amplitude", 150, 0, 200 ));
    // paramsGroup2.add(pFreq[1].set( "Frequency ", 1, 1, 10 ));
    // paramsGroup2.add(pPhase[1].set("Phase", 0,0,TWO_PI));
    paramsGroup2.add(pDensity[1].set("Density", 50, 0, 200));
    paramsGroup2.add(pAnimSpeed[1].set("Animation Speed", 960, 30, 960));
    paramsGroup2.add(plissajouRatio[0].set("Lissajous Radio X", 1, 1, 8));
    paramsGroup2.add(plissajouRatio[1].set("Lissajous Radio Y", 3, 1, 8));
    paramsGroup2.add(pImgScale[1].set("Image scale", 1, 1, 50));
    // paramsGroup2.add(pJitterScale[1].set("Jitter scale", true));
    
    paramsGroup2.add(pRec.set( "Record", false ));

    gui.add(paramsGroup1);
    gui.add(paramsGroup2);
    
}


void ofApp::drawWave() {
    double y,x,y2;
    float t, phase,iconHeight,iconWidth;
    
    phase =  anim[0].val();

    t = TWO_PI * this->pFreq[0] / this->pDensity[0] ;
    for (int i = 0; i < this->pDensity[0] ;i++) {
        y = this->pAmp[0] * sin( t * i + phase );
        y2 = this->pAmp[0] * sin( t * i + phase + PI);

        x = i * rgbaFbo.getWidth() / this->pDensity[0];
        //ofCircle( x - rgbaFbo.getWidth()/2 ,y,1);
        //ofCircle( x - rgbaFbo.getWidth()/2 ,y2,1);
        
        iconWidth = triImg.width * this->imgScales[0];
        iconHeight = triImg.height * this->imgScales[0];
        triImg.draw(x-iconWidth/2-rgbaFbo.getWidth()/2, y-iconHeight/2, iconWidth, iconHeight);
        triImg.draw(x-iconWidth/2-rgbaFbo.getWidth()/2, y2-iconHeight/2, iconWidth, iconHeight);

    }
}

float ofApp::drawLissaous() {
    
    double y,x;
    float phase,v,iconHeight,iconWidth;
    int scale;
    
    scale = pAmp[1] * 2;
    phase = anim[1].val();

    v = TWO_PI / this->pDensity[1];
    
    for (int i = 0; i < this->pDensity[1] ; i++) {
        
        x = scale * cos( this->plissajouRatio[0] * v * i + phase);
        y = scale * sin( this->plissajouRatio[1] * v * i + phase);
        
        // triImg.draw(x,y,triImg.width *  this->imgScales[0], triImg.height * this->imgScales[0]);
        
        iconWidth = eyesImg.width * this->imgScales[1];
        iconHeight = eyesImg.height * this->imgScales[1];
            
        eyesImg.draw(x-iconWidth/2, y-iconHeight/2, iconWidth, iconHeight);
        ofCircle(x,y,1);
        
    }
    
    return phase;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    pparam1 = ofToString(w) + "x" + ofToString(h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}