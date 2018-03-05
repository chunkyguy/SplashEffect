//
//  Rectangle.h
//  SplashEffect
//
//  Created by sid on 16/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#ifndef __SplashEffect__Rectangle__
#define __SplashEffect__Rectangle__
#include "RenderObject.h"

class Rectangle : public RenderObject{
public:
	void render(GLuint program);
	void update(float dt);
	Rectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat *color4f, int res);
	~Rectangle();
	void moveToLocation(GLfloat x, GLfloat y);
	int hitTest(Rectangle *r);
	bool splash(int index);
	bool hitBottom();
	void getLocation(GLfloat *x, GLfloat *y);
	void createVertexData();

private:
	GLfloat *color_;
	int resolution_;
	GLfloat *maxYs_;
	GLfloat *velocity_;
	GLfloat minX_;
	GLfloat minY_;
	GLfloat maxX_;
	GLfloat maxY_;
	Rectangle();
};


#endif /* defined(__SplashEffect__Rectangle__) */
