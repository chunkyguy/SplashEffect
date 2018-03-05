//
//  Rectangle.cpp
//  SplashEffect
//
//  Created by sid on 16/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#include "Rectangle.h"

//const float vertexPositions[] = {
//	-1.0, 0.2f, 0.0f, 1.0f,
//	-1.0f, -1.0f, 0.0f, 1.0f,
//	1.0f, 0.2f, 0.0f, 1.0f,
//	1.0f, -1.0f, 0.0f, 1.0f,
//};
const GLfloat kHeightVariance = 0.0f;
Rectangle::~Rectangle(){
	delete[] color_;
	delete [] vertData_;
	delete [] maxYs_;
	delete [] velocity_;
}

Rectangle::Rectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat *color4f, int res):
minX_(x),
minY_(y),
maxX_(w),
maxY_(h),
resolution_(res){
	color_ = new GLfloat[4];
	for(int i = 0; i < 4; i++){
		color_[i] = color4f[i];
	}
	points_ = 4 + (res-1) * 2;
	vertData_ = NULL;
	maxYs_ = new GLfloat[res+1];
	velocity_ = new GLfloat[res + 1];
	for(int i = 0; i < res+1; i++){
		maxYs_[i] = h;
		velocity_[i] = 0.0f;
	}
	createVertexData();
}

void Rectangle::createVertexData(){
	if(vertData_ != NULL){
		delete [] vertData_;
	}
	
	GLfloat dx = (maxX_ - minX_)/resolution_;
	vertData_ = new GLfloat[points_ * 4];
	GLfloat point[2];
	floatNo_ = 0;
	int s = 0;
	for(; s < resolution_; s++){
		point[0] = minX_ + s*dx; point[1] = maxYs_[s] + GLfloat(rand())/GLfloat(RAND_MAX)*kHeightVariance;	addPoint2f(point);
		point[0] = minX_ + s*dx; point[1] = minY_;	addPoint2f(point);
	}
	point[0] = maxX_; point[1] = maxYs_[s] + GLfloat(rand())/GLfloat(RAND_MAX)*kHeightVariance;	addPoint2f(point);
	point[0] = maxX_; point[1] = minY_;	addPoint2f(point);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floatNo_, vertData_, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Rectangle::render(GLuint program){
	GLuint colorLoc = glGetUniformLocation(program, "uColor");
	glUniform4fv(colorLoc, 1, color_);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, points_);
	glDisableVertexAttribArray(0);
}

const float kSpringConstant = 0.025f; // adjust this value to your liking
const float kDampeningFactor = 0.025f;
const float kSpreadFactor = 0.04f;
void Rectangle::update(float dt){
	if(minY_ > -1.0f){
		moveToLocation(minX_, minY_-0.01);
	}
	for(int i = 0; i < resolution_+1; i++){
		float yStrech = maxYs_[i] - maxY_;
		float acceleration = (-kSpringConstant * yStrech) - (kDampeningFactor * velocity_[i]);
		maxYs_[i] += velocity_[i];
		velocity_[i] += acceleration;
	}
	
	float dtLeft[resolution_+1];
	float dtRight[resolution_+1];
	for(int i = 0; i < 8; i++){
		for(int i = 0; i < resolution_+1; i++){
			if(i > 0){
				dtLeft[i] = kSpreadFactor * (maxYs_[i] - maxYs_[i-1]);
				velocity_[i-1] += dtLeft[i];
			}
			if(i < resolution_){
				dtRight[i] = kSpreadFactor * (maxYs_[i] - maxYs_[i+1]);
				velocity_[i+1] += dtRight[i];
			}
		}
		for(int i = 0; i < resolution_+1; i++){
			if(i > 0){
				maxYs_[i-1] += dtLeft[i];
			}
			if(i < resolution_){
				maxYs_[i+1] += dtRight[i];
			}
		}
	}
	
	
	createVertexData();
}

void Rectangle::moveToLocation(GLfloat x, GLfloat y){
	maxX_ = maxX_ - minX_ + x;
	maxY_ = maxY_ - minY_ + y;
	minX_ = x;
	minY_ = y;
	for(int i = 0; i < resolution_+1; i++){
		maxYs_[i] = maxY_;
		velocity_[i] = 0.0f;
	}
}

int Rectangle::hitTest(Rectangle *r){
	if(r->minY_ < this->maxY_){
		GLfloat dx = (maxX_ - minX_)/resolution_;
		for(int i = 0; i < resolution_+1; i++){
			//fprintf(stderr, "hitTest: {%f,%f}:%f\n",this->minX_+i*dx,this->minX_+(i+1)*dx,r->minX_);
			if(r->minX_ >= this->minX_+i*dx && r->minX_ < this->minX_+(i+1)*dx){
				return i;
			}
		}
	}
	return -1;
}

bool Rectangle::splash(int index){
	if(index >= 0 && index <= resolution_+1){
		velocity_[index] = 0.1;
		return true;
	}
	return false;
}

bool Rectangle::hitBottom(){
	if(minY_ <= -1.0f){
		return true;
	}
	return false;
}

void Rectangle::getLocation(GLfloat *x, GLfloat *y){
	*x = minX_;
	*y = minY_;
}