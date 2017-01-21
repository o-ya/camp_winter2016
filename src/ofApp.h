#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"

class ofApp : public ofBaseApp{

public:
   void setup();
   void update();
   void draw();
   void dragEvent( ofDragInfo dragInfo );
   void loadFace( string face );
   void exit();
   void keyPressed( int key );
   
   // カメラ入力によって顔を追跡
   ofxFaceTrackerThreaded camTracker;
   ofVideoGrabber cam;
   
   // 画像から顔を検出
   ofxFaceTracker srcTracker;
   ofImage src;
   vector<ofVec2f> srcPoints;
   
   // 顔のクローンを作成
   bool cloneReady;
   Clone clone;
   
   
   ofFbo srcFbo, maskFbo;
   
   ofDirectory faces;
   int currentFace;
   bool isFaceVert = false;
		
};
