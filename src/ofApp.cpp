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
   
   particleMesh.setMode(OF_PRIMITIVE_POINTS);
   heaerMesh.setMode(OF_PRIMITIVE_TRIANGLES);
   
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
   
   particleMesh.clear();
   for( ParticleVec2* particle : _particles ){
      particle->addForce(0.0,0.1);
      particle->update();
      particle->bounceOfWalls();
      particleMesh.addColor( particle->getColor());
      particleMesh.addVertex( ofVec3f(particle->getPosX(), particle->getPosY() ));
      
   }
   
   
   // 唇開口の検出
   mouseWidth = camTracker.getGesture( ofxFaceTracker::MOUTH_WIDTH );
   mouseHeight = camTracker.getGesture( ofxFaceTracker::MOUTH_HEIGHT );
   isOpenMouse = false;
   if(mouseHeight / mouseWidth > 0.5){
      isOpenMouse = true;
      mouseTopPos = camTracker.getImagePoint( 61 );
      mouseButtomPos = camTracker.getImagePoint( 64 );
      mouseCenter = mouseTopPos + (mouseButtomPos - mouseTopPos)/2;
   }
   
   // 右目のウィンクを検出
   rightEyeOpenness = camTracker.getGesture(ofxFaceTracker::RIGHT_EYE_OPENNESS);
   isrightEyeClose =false;
   if (rightEyeOpenness < 0.1 )
      isrightEyeClose = true;
      
   
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
      glPointSize(2.0);
      ofSetLineWidth(1);
      camTracker.getImageMesh().drawVertices();
   }
   
   // 口の開き方で変化
   if (isOpenMouse){
      addParticle(mouseCenter.x, mouseCenter.y);
   }
   
   // 右目のウィンクで変化
   if (isrightEyeClose){
      
   }
   
   glPointSize(6.0);
   particleMesh.draw();
}


void ofApp::exit(){
   camTracker.waitForThread();
   camTracker.stopThread();
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
      case 'p':
         addParticle(100,100);
         break;
         
      case '1':
         particleMesh.setMode(OF_PRIMITIVE_POINTS);
         break;
      case '2':
         particleMesh.setMode(OF_PRIMITIVE_LINES);
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

//--------------------------------------------------------------
// パーティクル出現
//--------------------------------------------------------------
void ofApp::addParticle(int x, int y){
   for( int i =0; i<10; i++){
      if ( _particles.size() > _max_num )
         _particles.erase( _particles.begin() );
      
      ParticleVec2* p = new ParticleVec2();
      p->start(x, y);
      _particles.push_back( move(p) );
      
   }
}





