//
//  Particles.h
//  SplashEffect
//
//  Created by sid on 16/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#ifndef __SplashEffect__Particles__
#define __SplashEffect__Particles__
#include "RenderObject.h"

class Particles : public RenderObject{
	
public:
	Particles(int count, GLfloat x, GLfloat y, GLfloat radX, GLfloat radY);
	void createBuffers();
	void render(GLuint program);
	void update(float dt);
	void splash(GLfloat x, GLfloat y);
private:
	GLuint tex_;
	bool sleeping_;
	GLfloat centerX_;
	GLfloat centerY_;
	GLfloat radiusX_;
	GLfloat radiusY_;
	int life_;
	void createVertexData();
};

#endif /* defined(__SplashEffect__Particles__) */
