//
//  ParticleVec2.cpp
//  myParticle
//
//  Created by 大矢慎之介 on 2017/01/11.
//
//

#include "ParticleVec2.h"

// ------------------------------
// コンストラクタ
// ------------------------------
ParticleVec2::ParticleVec2()
:  _position(ofVec2f(ofGetWidth()/2.0, ofGetHeight()/2.0)),
   _velocity( ofVec2f(0, 0)),
   _radius( 5.0 ),
   _friction( 0.05 ),
   _mass( 1.0 ),
   _color( ofRandom(0,255),ofRandom(0,255),ofRandom(0,255) )
{};

// ------------------------------
// デストラクタ
// ------------------------------
ParticleVec2::~ParticleVec2(){}


// ------------------------------
// 更新
// ------------------------------
void ParticleVec2::update(){
   _acceleration -= _velocity * _friction;
   _velocity += _acceleration;
   _position += _velocity;
   _acceleration.set(0, 0);
}

// ------------------------------
// 描画
// ------------------------------
void ParticleVec2::draw(){
   bounceOfWalls();
   ofDrawCircle(_position.x, _position.y, _radius);
}

// ------------------------------
// 力
// ------------------------------
void ParticleVec2::addForce(ofVec2f force){
   _acceleration += force / _mass;
}

void ParticleVec2::addForce(float forceX, float forceY){
   _acceleration +=ofVec2f(forceX, forceY);
}


// ------------------------------
// 引力
// ------------------------------
void ParticleVec2::addAttractionForce(float x, float y, float radius, float scale){
   ofVec2f pos(x,y);
   ofVec2f diff = _position - pos;
   float length = diff.length();
   bool is_close = true;
   if(radius > 0){
    if(length > radius)
       is_close = false;
   }
   if (is_close){
      float pct = 1 - (length /radius);
      diff.normalize();
      _acceleration = _acceleration - diff * scale *pct;
   }
   
}

//void ParticleVec2::addAttractionForce(ParticleVec2 &p,float radisu, float scale){}


// ------------------------------
// 斥力
// ------------------------------
void ParticleVec2::addRepulsionForce(float x, float y, float radius, float scale){
   ofVec2f pos(x,y);
   ofVec2f diff = _position - pos;
   float length = diff.length();
   bool is_close = true;
   if(radius > 0){
      if(length > radius)
         is_close = false;
   }
   if (is_close){
      float pct = 1 - (length /radius);
      diff.normalize();
      _acceleration = _acceleration + diff * scale *pct;
   }

}

//void ParticleVec2::addRepulsionForce(ParticleVec2 &p,float radisu, float scale){}


// ------------------------------
// 出現
// ------------------------------
void ParticleVec2::start(int x, int y){
   _position = ofVec2f(x, y);
   _velocity = ofVec2f(0, 0);
   
   float length = ofRandom(20.0);
   float angle = ofRandom(2.0 * PI);
   ofVec2f force = ofVec2f(length*cos(angle), length*sin(angle));
   addForce(force);
}


// ------------------------------
// 通過
// ------------------------------
void ParticleVec2::throwOfWalls(){

   _position.x = ( _position.x > ofGetWidth() )
   ? 0
   : _position.x;
   
   _position.x = ( _position.y > ofGetWidth() )
   ? 0
   : _position.y;
   
   
   _position.x = ( _position.x < 0 )
   ? ofGetWidth()
   : _position.x;
   
   _position.x = ( _position.y < 0 )
   ? ofGetHeight()
   : _position.y;
   
}


// ------------------------------
// バウンド
// ------------------------------
void ParticleVec2::bounceOfWalls(){

   if (_position.x > ofGetWidth() ){
      _position.x = ofGetWidth();
      _velocity.x *= -1;
   }
   
   if (_position.x < 0 ){
      _position.x = 0;
      _velocity.x *= -1;
   }
   
   if (_position.y > ofGetHeight() ){
      _position.y = ofGetHeight();
      _velocity.y *= -1;
   }
   if (_position.y < 0 ){
      _position.y = 0;
      _velocity.y *= -1;
   }
}
