#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 8000

class ofApp : public ofBaseApp{
    
    typedef struct {
        ofPoint a;
        ofPoint b;
    } simpleLine;

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void drawRotatingTriangles();
        void drawLineMovingHorizontally();
        void drawLineMovingVertically();
        void drawCircles();
        void drawAdvancingLines();
        void drawColors();
        void drawBranchingLines();
    
        ofxSyphonServer mainOutputSyphonServer;
        ofxOscSender sender;
    
        int width;
        int height;
        float time;
    
        int numAnimations;
        int currentAnimation;
        float timeOfLastAnimationChange;
        int animationDuration;
    
        ofVideoPlayer noiseClip;
        ofVideoPlayer linesClip;
};
