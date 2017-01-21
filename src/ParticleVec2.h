//
//  ParticleVec2.hpp
//  myParticle
//
//  Created by 大矢慎之介 on 2017/01/11.
//
//

#ifndef ParticleVec2_h
#define ParticleVec2_h

#include <stdio.h>
#include "ofMain.h"

class ParticleVec2 {

public:
   ParticleVec2();
   ~ParticleVec2();
   
   void update();
   void draw();
   void start(int x, int y);
   void addForce(ofVec2f force);
   void addForce(float forceX, float forceY);

   // 引力
   void addAttractionForce(float x, float y, float radius, float scale);
   void addAttractionForce(ParticleVec2 &p,float radisu, float scale);
   
   // 斥力
   void addRepulsionForce(float x, float y, float radius, float scale);
   void addRepulsionForce(ParticleVec2 &p,float radisu, float scale);
   
   
   // 壁へのアプローチ
   void throwOfWalls();
   void bounceOfWalls();
   
   // getter
   float getPosX() const { return _position.x; }
   float getPosY() const { return _position.y; }
   ofColor getColor() const {return _color;}
   
private:
   
   ofColor _color; // 色
   ofVec2f _position; // 位置
   ofVec2f _velocity; // 速度
   ofVec2f _acceleration; // 加速度
   float _radius; // 表示する円の半径
   float _friction; // 摩擦
   float _mass; // 質量
};


#endif /* ParticleVec2_hpp */
