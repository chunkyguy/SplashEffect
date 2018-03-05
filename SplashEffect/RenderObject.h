//
//  RenderObject.h
//  SplashEffect
//
//  Created by sid on 16/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#ifndef __SplashEffect__RenderObject__
#define __SplashEffect__RenderObject__
#include "stdArgs.h"

class RenderObject {
public:
	virtual void createBuffers();
	virtual void render(GLuint program);
	virtual void update(float dt);
protected:
	GLuint buffer_;
	GLuint vao_;
	int points_;
	int floatNo_;
	GLfloat *vertData_;
	void addPoint2f(GLfloat *pt);
	virtual void createVertexData();
};

#endif /* defined(__SplashEffect__RenderObject__) */
