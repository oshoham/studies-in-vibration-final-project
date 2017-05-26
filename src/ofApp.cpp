#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    mainOutputSyphonServer.setName("Screen Output");
    sender.setup(HOST, PORT);
    
    width = ofGetWidth();
    height = ofGetHeight();
    
    numAnimations = 5;
    animationDuration = 10;
    
    currentAnimation = 0;
    timeOfLastAnimationChange = 0;
    
    ofxOscMessage m;
    m.setAddress("presets/asymmetrical2");
    m.addBoolArg(true);
    sender.sendMessage(m, false);
    
    noiseClip.load("noise_highres.mov");
    noiseClip.setLoopState(OF_LOOP_NORMAL);
    
    linesClip.load("lines.mov");
    linesClip.setLoopState(OF_LOOP_PALINDROME);
}

//--------------------------------------------------------------
void ofApp::update(){
    time = ofGetElapsedTimef();
    
    if (time - timeOfLastAnimationChange > animationDuration) {
        currentAnimation = (currentAnimation + 1) % numAnimations;
        timeOfLastAnimationChange = time;
        
        if (currentAnimation == 0) {
            ofxOscMessage m;
            m.setAddress("presets/asymmetrical2");
            m.addBoolArg(true);
            sender.sendMessage(m, false);
        } else if (currentAnimation == 1) {
            ofxOscMessage m;
            m.setAddress("presets/asymmetrical3");
            m.addBoolArg(true);
            sender.sendMessage(m, false);
        } else if (currentAnimation == 2) {
            ofxOscMessage m;
            m.setAddress("presets/asymmetrical2");
            m.addBoolArg(true);
            sender.sendMessage(m, false);
            
            noiseClip.setFrame(0);
            noiseClip.play();
        } else if (currentAnimation == 3) {
            noiseClip.stop();
            
            ofxOscMessage m;
            m.setAddress("presets/asymmetrical2");
            m.addBoolArg(true);
            sender.sendMessage(m, false);
            
            linesClip.setFrame(0);
            linesClip.play();
        } else if (currentAnimation == 4) {
            linesClip.stop();
            
            ofxOscMessage m;
            m.setAddress("presets/asymmetrical3");
            m.addBoolArg(true);
            sender.sendMessage(m, false);
        }
    }
    
    if (currentAnimation == 2) {
        noiseClip.update();
    } else if (currentAnimation == 3) {
        linesClip.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    if (currentAnimation == 0) {
        drawAdvancingLines();
    } else if (currentAnimation == 1) {
        drawLineMovingHorizontally();
    } else if (currentAnimation == 2) {
        noiseClip.draw(0, 0, width, height);
    } else if (currentAnimation == 3) {
        linesClip.draw(0, 0, width, height);
    } else if (currentAnimation == 4) {
        drawBranchingLines();
    }
    
    mainOutputSyphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::drawBranchingLines(){
    ofSetColor(255);
    ofSetLineWidth(1);
    ofSeedRandom(ofMap(fmod(time, 120), 0, 120, 0, 2500));
    vector<simpleLine> lines;
    lines.push_back((simpleLine){ofPoint(width / 2, 0), ofPoint(width / 2, height)});
    lines.push_back((simpleLine){ofPoint(0, height / 2), ofPoint(width, height / 2)});
    lines.push_back((simpleLine){ofPoint(0, 0), ofPoint(width, height)});
    lines.push_back((simpleLine){ofPoint(0, height), ofPoint(width, 0)});

    
    for (int z = 0; z < 3000; z ++){
        int randomLine = ofRandom(0, lines.size());
        float randomPct = ofRandom(0,1);
        
        ofPoint diff = lines[randomLine].b - lines[randomLine].a;
        
        if (diff.length() < 5) continue;
        float angle = atan2(diff.y, diff.x);
        
        if (ofRandom(0,1) > 0.5){
            angle += PI/2;
        } else {
            angle -= PI/2;
        }
        
        float randomAngle = angle + ofRandom(-0.02, 0.02);
        
        
        ofPoint a = lines[randomLine].a + randomPct * (  lines[randomLine].b-  lines[randomLine].a);
        
        ofPoint b = a + 2000 * ofPoint(cos(randomAngle), sin(randomAngle));
        
        
        float shortestIntersection = 1000000;
        ofPoint smallestIntersection;
        
        for (auto line : lines){
            
            ofPoint intersection;
            
            if (ofLineSegmentIntersection(a, b, line.a, line.b, intersection)){
                
                float distance = (intersection - a).length();
                if (distance < shortestIntersection){
                    shortestIntersection = distance;
                    smallestIntersection = intersection;
                }
            }
        }
        
        if (shortestIntersection < 9999){
            b = smallestIntersection;
        }
        
        
        
        simpleLine tempLine;
        tempLine.a =a;
        tempLine.b =b;
        lines.push_back(tempLine);
    }
    
    for (auto & line : lines){
        ofDrawLine(line.a, line.b);
    }
}

//--------------------------------------------------------------
void ofApp::drawAdvancingLines(){
    ofSetColor(255);
    ofSetLineWidth(10);
    
    int yPos = ofMap(fmod(time, 10), 0, 10, height, 0);
    int spacing = 200;
    int numLines = height / spacing;
    for (int i = 0; i < numLines; i++) {
        int y = (yPos - i * spacing);
        if (y < 0) {
            y = height + y;
        } else if (y >= height) {
            y = y % height;
        }
        ofDrawLine(0, y, width, y);
    }
}

//--------------------------------------------------------------
void ofApp::drawLineMovingVertically(){
    ofSetColor(255);
    ofSetLineWidth(10);
    
    int yPos = ofMap(sin(time), -1, 1, 0, height);
    ofDrawLine(0, yPos, width, yPos);
}

//--------------------------------------------------------------
void ofApp::drawLineMovingHorizontally(){
    ofSetColor(255);
    ofSetLineWidth(10);
    
    int xPos = ofMap(sin(time), -1, 1, 0, width);
    ofDrawLine(xPos, 0, xPos, height);
}

//--------------------------------------------------------------
void ofApp::drawColors(){
    ofShader shader;
    shader.load("", "sunset.frag");
    shader.begin();
    shader.setUniform1f("u_time", time);
    shader.setUniform2f("u_resolution", width, height);
    shader.setUniform2f("u_mouse", ofGetMouseX(), ofGetMouseY());
    ofDrawRectangle(0, 0, width, height);
    shader.end();
}

//--------------------------------------------------------------
void ofApp::drawRotatingTriangles(){
    ofSetColor(255);
//    ofNoFill();
    
    float triangleRadius = min(height, width) / 2;
    float rotation = ofMap(time, 0, 10, 0, TWO_PI);
    
    ofPushMatrix();
    ofTranslate(width / 2, height / 2);
    ofDrawTriangle(
                   triangleRadius * cos(rotation), triangleRadius * sin(rotation),
                   triangleRadius * cos(rotation + TWO_PI / 3.0), triangleRadius * sin(rotation + TWO_PI / 3.0),
                   triangleRadius * cos(rotation + TWO_PI / 1.5), triangleRadius * sin(rotation + TWO_PI / 1.5)
    );
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == '1') {
        ofxOscMessage m;
        m.setAddress("presets/asymmetrical");
        m.addBoolArg(true);
        sender.sendMessage(m, false);
    } else if (key == '2') {
        ofxOscMessage m;
        m.setAddress("presets/asymmetrical2");
        m.addBoolArg(true);
        sender.sendMessage(m, false);
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
