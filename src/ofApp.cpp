#include "ofApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
// セットアップ
//--------------------------------------------------------------
void ofApp::setup(){
   
   // ティアリング防止
   ofSetVerticalSync( true );
   
   cloneReady = false;
   cam.initGrabber( 640, 480 );
   clone.setup( cam.getWidth(), cam.getHeight() );
   ofFbo::Settings settings;

   // 高速化のための
   settings.width = cam.getWidth();
   settings.height = cam.getHeight();
   maskFbo.allocate( settings );
   srcFbo.allocate( settings );
   
   camTracker.setup();
   srcTracker.setup();
   srcTracker.setIterations( 25 );
   srcTracker.setAttempts( 4 );
   
   faces.allowExt( "jpg" );
   faces.allowExt( "png" );
   faces.listDir( "faces" );
   
   currentFace = 0;
   if( faces.size() != 0 ){
      loadFace( faces.getPath( currentFace ) );
   }
}

//--------------------------------------------------------------
// アップデート
//--------------------------------------------------------------
void ofApp::update() {
   cam.update();
   if( cam.isFrameNew() ) {
      camTracker.update( toCv(cam) );
      
      cloneReady = camTracker.getFound();
      if( cloneReady ) {
         ofMesh camMesh = camTracker.getImageMesh();
         camMesh.clearTexCoords();
         camMesh.addTexCoords( srcPoints );
         
         maskFbo.begin();
         ofClear(0, 255);
         camMesh.draw();
         maskFbo.end();
         
         
         
         srcFbo.begin();
         ofClear(0, 255);
         src.bind();
         camMesh.draw();
         src.unbind();
         srcFbo.end();
         
         clone.setStrength( 16 );
         clone.update( srcFbo.getTextureReference(),
                       cam.getTextureReference(),
                       maskFbo.getTextureReference() );
      }
   }
}

//--------------------------------------------------------------
// 描画
//--------------------------------------------------------------
void ofApp::draw() {
   
   ofSetColor( 255 );
   
   // フェイスの入力
   if( src.getWidth() > 0 && cloneReady ) {
      clone.draw(0, 0);
   } else {
      cam.draw(0, 0);
   }
   
   // 点の入力
   if(camTracker.getFound() && isFaceVert) {
      ofSetLineWidth(1);
      camTracker.getImageMesh().drawVertices();
   }
}

//--------------------------------------------------------------
// 顔読み込み
//--------------------------------------------------------------
void ofApp::loadFace(string face){
   src.loadImage( face );
   
   if( src.getWidth() > 0 ) {
      srcTracker.update( toCv(src) );
      srcPoints = srcTracker.getImagePoints();
   }
}

//--------------------------------------------------------------
// 顔読み込み
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
   loadFace( dragInfo.files[0] );
}

//--------------------------------------------------------------
//
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
   switch(key){
      case OF_KEY_UP:
         currentFace++;
         break;
      case OF_KEY_DOWN:
         currentFace--;
         break;
      case 'd':
         isFaceVert = !isFaceVert;
         break;
   }
   
   //
   //ofClamp(val, min, max) は，val の値が min〜max の範囲にある場合は val をそのまま返し，min を下回っていれば min を，max を上回っていれば max を返す関数
   //
   currentFace = ofClamp( currentFace, 0, faces.size() );
   if( faces.size() != 0 ){
      loadFace( faces.getPath( currentFace ) );
   }
}

void ofApp::exit(){
   camTracker.waitForThread();
   camTracker.stopThread();
}
