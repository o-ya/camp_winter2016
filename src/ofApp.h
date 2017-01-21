#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "Clone.h"
#include "ofxFaceTracker.h"
#include "ofxFaceTrackerThreaded.h"
#include "ParticleVec2.h"

class ofApp : public ofBaseApp{

public:
   
   // base
   void setup();
   void update();
   void draw();
   void dragEvent( ofDragInfo dragInfo );
   void keyPressed( int key );
   void exit();
   
   // plus
   void loadFace( string face );
   void addParticle(int x , int y);
   void addHeart(int x, int y);
   
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
		
   vector<ParticleVec2*> _particles;
   unsigned int _max_num = 1000;
   
   ofVboMesh particleMesh;
   ofVboMesh heaerMesh;
   
   float rightEyeOpenness;
   bool isrightEyeClose = false;
   
   float mouseWidth;
   float mouseHeight;
   bool isOpenMouse= false;
   
   ofVec2f mouseTopPos,mouseButtomPos,mouseCenter;

   
};
