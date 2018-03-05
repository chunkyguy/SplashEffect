//
//  Particles.cpp
//  SplashEffect
//
//  Created by sid on 16/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#include "Particles.h"

Particles::Particles(int count, GLfloat x, GLfloat y, GLfloat radX, GLfloat radY):
centerX_(x),
centerY_(y),
radiusX_(radX),
radiusY_(radY){
	vertData_ = NULL;
	points_ = count;
	sleeping_ = true;
	createVertexData();
}

void Particles::createBuffers(){
	RenderObject::createBuffers();
	
//	sf::Image img;
//	img.loadFromFile(resourcePath() + "Droplet.png");
//	glGenTextures(1, &tex_);
//	glBindTexture(GL_TEXTURE_2D, tex_);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

}
void Particles::createVertexData(){
	if(vertData_ != NULL){
		delete [] vertData_;
	}
	vertData_ = new GLfloat[points_*2];

	life_ = 100;
	floatNo_ = 0;
	GLfloat point[2];
	for(int i = 0; i < points_; i++){
		point[0] = GLfloat(rand())/GLfloat(RAND_MAX)*radiusX_ * 2.0 - 1.0 * centerX_;
		point[1] = GLfloat(rand())/GLfloat(RAND_MAX)*radiusY_ * 2.0 - 1.0 * centerY_;
		addPoint2f(point);
	}
	glBindBuffer(GL_ARRAY_BUFFER, buffer_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * floatNo_, vertData_, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Particles::render(GLuint program){
	if(sleeping_)
		return;
	
	glBindTexture(GL_TEXTURE_2D, tex_);
	GLuint texLoc = glGetUniformLocation(program, "uTex");
	glUniform1i(texLoc, 0);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_POINT, 0, points_);
	glDisableVertexAttribArray(0);
}

void Particles::update(float dt){
	if(sleeping_)
		return;
	
	life_--;
	if(life_ <= 0){
		sleeping_ = true;
	}
}

void Particles::splash(GLfloat x, GLfloat y){
	sleeping_ = false;
	centerX_ = x;
	centerY_ = y;
	createVertexData();
}