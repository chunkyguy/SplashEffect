//
//  Splash.h
//  SplashEffect
//
//  Created by sid on 15/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#ifndef __SplashEffect__Splash__
#define __SplashEffect__Splash__
#include "stdArgs.h"
/* D_BUG_PARTICLES
 #include "Particles.h"
 */
#include "Rectangle.h"
class SplashWindow {
public:
	SplashWindow();
	~SplashWindow();
	void loop();
	
private:
	sf::Window window_;
	GLuint createShader(GLenum eShaderType, const std::string &strShaderFile);
	GLuint createProgram(const std::vector<GLuint> &shaderList);
	GLuint program_;
	GLuint particleProgram_;
	sf::Clock clock_;
	std::vector<RenderObject *>objects_;
	Rectangle *water_;
	Rectangle *plane_;
	/* D_BUG_PARTICLES
	Particles *particles_;
	 */
	bool isHit_;
};

#endif /* defined(__SplashEffect__Splash__) */
