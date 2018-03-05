//
//  Splash.cpp
//  SplashEffect
//
//  Created by sid on 15/08/12.
//  Copyright (c) 2012 whackylabs. All rights reserved.
//

#include "SplashWindow.h"
#include <fstream>
#include <sstream>

const int kWindowWidth = 800;
const int kWindowHeight = 600;

GLuint SplashWindow::createShader(GLenum eShaderType, const std::string &strShaderFile){
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	//fprintf(stderr, "compiling: %s",strFileData);
	glShaderSource(shader, 1, &strFileData, NULL);
	
	glCompileShader(shader);
	
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		
		const char *strShaderType = NULL;
		switch(eShaderType)
		{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}
		
		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}
	
	return shader;
}

GLuint SplashWindow::createProgram(const std::vector<GLuint> &shaderList){
	GLuint program = glCreateProgram();
	
	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);
	
	glLinkProgram(program);
	
	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	
	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);
	
	return program;
}

SplashWindow::SplashWindow(){
	window_.create(sf::VideoMode(kWindowWidth, kWindowHeight), "Splash");
	window_.setFramerateLimit(60);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	{
		std::vector<GLuint> shaderList;
		std::string vshPath = resourcePath() + "splash.vertsh";
		std::ifstream vshStream(vshPath.c_str(), std::ifstream::in);
		std::string vSrc((std::istreambuf_iterator<char>(vshStream)),std::istreambuf_iterator<char>());
		shaderList.push_back(createShader(GL_VERTEX_SHADER, vSrc));
		vshStream.close();
		
		std::string fshPath = resourcePath() + "splash.fragsh";
		std::ifstream fshStream(fshPath.c_str(), std::ifstream::in);
		std::string fSrc((std::istreambuf_iterator<char>(fshStream)),std::istreambuf_iterator<char>());
		shaderList.push_back(createShader(GL_FRAGMENT_SHADER, fSrc));
		fshStream.close();
		
		program_ = createProgram(shaderList);
		std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
	}		
	{
		std::vector<GLuint> shaderList2;
		std::string vshPath2 = resourcePath() + "particle.vertsh";
		std::ifstream vshStream2(vshPath2.c_str(), std::ifstream::in);
		std::string vSrc2((std::istreambuf_iterator<char>(vshStream2)),std::istreambuf_iterator<char>());
		shaderList2.push_back(createShader(GL_VERTEX_SHADER, vSrc2));
		vshStream2.close();
		
		std::string fshPath2 = resourcePath() + "particle.fragsh";
		std::ifstream fshStream2(fshPath2.c_str(), std::ifstream::in);
		std::string fSrc2((std::istreambuf_iterator<char>(fshStream2)),std::istreambuf_iterator<char>());
		shaderList2.push_back(createShader(GL_FRAGMENT_SHADER, fSrc2));
		fshStream2.close();
		
		particleProgram_ = createProgram(shaderList2);
		std::for_each(shaderList2.begin(), shaderList2.end(), glDeleteShader);
	}
	
	isHit_ = false;
	GLfloat color1[] ={ 0.57f, 0.85f, 1.0f, 0.9f};
	water_ = new Rectangle(-1.0, -1.0, 1.0, 0.2f, color1, 50);
	objects_.push_back(water_);
	
	GLfloat color2[] = {0.8, 0.6, 0.4, 0.7};
	float orgX = 0.0f; float orgY = 0.8f;
	plane_ = new Rectangle(orgX, orgY, orgX+0.1, orgY+0.1f, color2, 1);
	objects_.push_back(plane_);
	/* D_BUG_PARTICLES
	particles_ = new Particles(10, 0.0, 0.0, 0.5f, 0.5f);
	objects_.push_back(particles_);
	*/
	for (std::vector<RenderObject *>::iterator it = objects_.begin(); it != objects_.end(); it++) {
		(*it)->createBuffers();
	}
}

SplashWindow::~SplashWindow(){
	for (std::vector<RenderObject *>::iterator it = objects_.begin(); it != objects_.end(); it++) {
		delete *it;
	}
}

void SplashWindow::loop(){
	while (window_.isOpen()) {
		sf::Event event;
		while (window_.pollEvent(event)) {
			if(event.type == sf::Event::Closed){
				window_.close();
			}
			if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
				window_.close();
			}
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
				GLfloat posX = GLfloat(event.mouseButton.x)/GLfloat(kWindowWidth) * 2.0f - 1.0f;
				GLfloat posY = 1.0f - GLfloat(event.mouseButton.y)/GLfloat(kWindowHeight) * 2.0f;
				std::cout<<"Mouse hit "<<posX<<" "<< posY <<std::endl;
				plane_->moveToLocation(posX, posY);
			}
			if(event.type == sf::Event::Resized){
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}
		window_.setActive();
		glClearColor(0.37, 0.55, 0.73, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(program_);{
			for (std::vector<RenderObject *>::iterator it = objects_.begin(); it != objects_.end(); it++) {
				(*it)->update(clock_.getElapsedTime().asSeconds());
				(*it)->render(program_);
			}
		}glUseProgram(0);
		if(!isHit_){
			int index = water_->hitTest(plane_);
			if(index >= 0){
				water_->splash(index);
				GLfloat x, y;
				plane_->getLocation(&x, &y);
				/* D_BUG_PARTICLES
				particles_->splash(x, y);
				 */
				isHit_ = true;
			}
		}else if(plane_->hitBottom()){
			plane_->moveToLocation(GLfloat(rand())/GLfloat(RAND_MAX)*2.0 - 1.0, 0.8);
			isHit_ = false;
		}
		window_.display();
	}
}