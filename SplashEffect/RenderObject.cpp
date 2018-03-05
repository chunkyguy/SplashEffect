//
//  RenderObject.cpp
//  SplashEffect
//
//  Created by sid on 16/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#include "RenderObject.h"

void RenderObject::createBuffers(){
	glGenBuffers(1, &buffer_);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_);
	
	glGenVertexArraysAPPLE(1, &vao_);
	glBindVertexArrayAPPLE(vao_);
}

void RenderObject::render(GLuint program){
}

void RenderObject::update(float dt){
}

void RenderObject::addPoint2f(GLfloat *pt){
	vertData_[floatNo_++] = *pt++;
	vertData_[floatNo_++] = *pt++;
	vertData_[floatNo_++] = 0.0f;
	vertData_[floatNo_++] = 1.0f;
}

void RenderObject::createVertexData(){
}
