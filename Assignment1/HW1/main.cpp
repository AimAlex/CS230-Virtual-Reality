/*
SJTU - CS230/CS238: Virtual Reality

Homework 1: Hello Teapot

Tasks:

1. compile and get to run
2. wireframe teapot
3. write mouse and keyboard interaction

	Egemen Ertugrul
	egertu@sjtu.edu.cn

*/

//-----------------------------------------------------------------------------
// includes

#include <stdio.h>
#include <cstdlib>
#include <sdlWrapper.h>
#include <shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <model.h>

#undef main // Needed because SDL defines its own main
#ifdef WIN32
	#include <windows.h>
#endif
//-----------------------------------------------------------------------------
// some global variables

// Relative path to source files
std::string codeDir = "../";

// Relative path to models directory
std::string modelsDir = codeDir + "models/";

// flag to check if render wireframe or filled
bool	bWireframe = true;

// near clipping plane
float	zNear = 1.0;

// far clipping plane
float	zFar = 100000.0;

float rotateX = 0.0f;

float rotateY = 0.0f;
// Function Declarations
bool handleEvents(SDL_Event & evt, sdlWrapper & sdlContext);
enum state {INIT, BUTTON_MIDDLE, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_A};
state State = state::INIT;
// ****************************************************************************
// ************            Insert your code here                   ************
// ****************************************************************************

// You can use these global variables to start thinking about how to implement mouse movements
// You do not have to use these if you don't want

// parameters for the navigation
glm::vec3	viewerPosition	(0.0, 0.0, 50.0);
glm::vec3	viewerCenter	(0.0, 0.0, 0.0);
glm::vec3	viewerUp		(0.0, 1.0, 0.0);

// rotation values for the navigation
float	navigationRotation[2];

// position of the mouse when pressed
int		mousePressedX = 0, mousePressedY = 0;
float	lastXOffset = 0.0, lastYOffset = 0.0, lastZOffset = 0.0, lastXRotate = 0.0, lastYRotate = 0.0;
// mouse button states
//int        leftMouseButtonActive = 0, middleMouseButtonActive = 0, rightMouseButtonActive = 0;
float Offset[3];
// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

void modify(Shader & shad, glm::vec3 & teapotPos, SDL_Event & evt, float aspectRatio, float large = 1, int no = 1){
//    lastXOffset = teapotPos.x;
//    lastYOffset = teapotPos.y;
//    lastZOffset = teapotPos.z;
    switch (State) {
        case state::INIT:
            teapotPos.x = lastXOffset;
            teapotPos.y = lastYOffset;
            teapotPos.z = lastZOffset;
            break;
        case state::BUTTON_MIDDLE:
            teapotPos.x = evt.button.x - mousePressedX + lastXOffset;
            teapotPos.y = mousePressedY - evt.button.y + lastYOffset;
            teapotPos.z = lastZOffset;
            break;
        case state::BUTTON_LEFT:
            teapotPos.x = lastXOffset;
            teapotPos.y = lastYOffset;
            teapotPos.z = lastZOffset;
            rotateY = mousePressedX - evt.motion.x + lastYRotate;
            rotateX = mousePressedY - evt.motion.y + lastXRotate;
            break;
        case state::BUTTON_RIGHT:
            teapotPos.x = lastXOffset;
            teapotPos.y = lastYOffset;
            teapotPos.z = evt.button.x - mousePressedX + lastZOffset;
            break;
        case state::BUTTON_A:
            teapotPos.x = evt.button.x - mousePressedX + lastXOffset;
            teapotPos.y = mousePressedY - evt.button.y + lastYOffset;
            teapotPos.z = lastZOffset;
            break;
    }
    teapotPos.x += Offset[no];
    glm::mat4 projMat = glm::perspective(glm::radians(50.0f), aspectRatio, zNear, zFar);
    
    glm::mat4 modelMat = glm::rotate(glm::mat4(1.0f), glm::radians(rotateX), glm::vec3(1, 0, 0));
    modelMat = glm::rotate(modelMat, glm::radians(rotateY), glm::vec3(0, 1, 0));
    modelMat = glm::scale(glm::translate(modelMat, teapotPos), glm::vec3(10.0f * large));
//    modelMat = glm::rotate(modelMat, glm::radians(rotateX), glm::vec3(1, 0, 0));
//    modelMat = glm::rotate(modelMat, glm::radians(rotateY), glm::vec3(0, 1, 0));
    glm::mat4 viewMat = glm::lookAt(viewerPosition, viewerCenter, viewerUp);
    
    // Attached Projection, Model, and View matricies to the shader
    // In the shader the Proj * View * Model * vertex_coord operation is carried out
    shad.attachToUniform("Proj", projMat);
    shad.attachToUniform("View", viewMat);
    shad.attachToUniform("Model", modelMat);
}

//---------------------------------------------------------------
// main function
//---------------------------------------------------------------

int main(int argc, char **argv) {
    
	sdlWrapper sdlContext(1280, 960, "Hello World", 0, false);
	SDL_Event evt;

	float aspectRatio = (float)sdlContext.getWidth() / (float)sdlContext.getHeight();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	printf("\n\nCS230/CS238: Virtual Reality - HW1\n");
	printf("keys:\n\tw\t- toggle wireframe mode\n\tf\t- toggle fullscreen\n\tesc\t- exit\n\n");
	printf("mouse:\n\tleft button\t- rotation\n\tmiddle button\t- panning\n\tright button\t- zoom in and out\n");

    printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
    
	// Shader class instantiation
	Shader shader("../shaders/vertexShader.glsl", "../shaders/fragShader.glsl");
	// ****************************************************************************
	// ************************* Load Your Models Here ****************************
	// ****************************************************************************
	std::cout << "Loading Models ...";
	Model model(modelsDir + "teapot.obj");
    Model model1(modelsDir + "wolf.obj");
    Model model2(modelsDir + "coffee.obj");
	Model axes(modelsDir + "axes.obj");
	std::cout << " Done!" << std::endl;
	// ****************************************************************************
	// ****************************************************************************
	// ****************************************************************************

    Offset[1] = 20;
    Offset[2] = -20;
	// Main rendering loop
	bool running = true;
	while (running) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		running = handleEvents(evt, sdlContext);
		glViewport(0, 0, sdlContext.getWidth(), sdlContext.getHeight());

		// Always call this before you attach something to uniform, or initiate any draw calls
		shader.Use();
		
		// ****************************************************************************
		// ************************** Insert your Code here ***************************
		// ****************************************************************************

		// Setup Projection, Model, and View Matricies
        
        glm::vec3 teapotPos = glm::vec3(0.0f, 0.0f, 0.0f);
        modify(shader, teapotPos, evt, aspectRatio, 1, 0);
        axes.Draw(shader);
        model.Draw(shader);
//        Offset[0] = lastXOffset;
        
        
//        lastXOffset = Offset[1];
        glm::vec3 wolfPos = glm::vec3(20, 0.0f, 0.0f);
        modify(shader, wolfPos, evt, aspectRatio, 0.3, 1);
        model1.Draw(shader);
//        Offset[1] = lastXOffset;

//        lastXOffset = Offset[2];
        glm::vec3 coffeePos = glm::vec3(-20, 0.0f, 0.0f);
        modify(shader, coffeePos, evt, aspectRatio, 0.1, 2);
        model2.Draw(shader);
//        Offset[2] = lastXOffset;
//        lastXOffset = Offset[0];
//        switch (State) {
//            case state::INIT:
//                teapotPos.x = lastXOffset;
//                teapotPos.y = lastYOffset;
//                teapotPos.z = lastZOffset;
//                break;
//            case state::BUTTON_MIDDLE:
//                teapotPos.x = evt.button.x - mousePressedX + lastXOffset;
//                teapotPos.y = mousePressedY - evt.button.y + lastYOffset;
//                teapotPos.z = lastZOffset;
//                break;
//            case state::BUTTON_LEFT:
//                teapotPos.x = lastXOffset;
//                teapotPos.y = lastYOffset;
//                teapotPos.z = lastZOffset;
//                rotateY = mousePressedX - evt.motion.x + lastYRotate;
//                rotateX = mousePressedY - evt.motion.y + lastXRotate;
//                break;
//            case state::BUTTON_RIGHT:
//                teapotPos.x = lastXOffset;
//                teapotPos.y = lastYOffset;
//                teapotPos.z = evt.button.x - mousePressedX + lastZOffset;
//                break;
//            case state::BUTTON_A:
//                teapotPos.x = evt.button.x - mousePressedX + lastXOffset;
//                teapotPos.y = mousePressedY - evt.button.y + lastYOffset;
//                teapotPos.z = lastZOffset;
//                break;
//        }
//        glm::mat4 projMat = glm::perspective(glm::radians(50.0f), aspectRatio, zNear, zFar);
//
//
//        glm::mat4 modelMat = glm::scale(glm::translate(glm::mat4(1.0f), teapotPos), glm::vec3(10.0f));
//        modelMat = glm::rotate(modelMat, glm::radians(rotateX), glm::vec3(1, 0, 0));
//        modelMat = glm::rotate(modelMat, glm::radians(rotateY), glm::vec3(0, 1, 0));
//        glm::mat4 viewMat = glm::lookAt(viewerPosition, viewerCenter, viewerUp);
//
////         Attached Projection, Model, and View matricies to the shader
////         In the shader the Proj * View * Model * vertex_coord operation is carried out
//        shader.attachToUniform("Proj", projMat);
//        shader.attachToUniform("View", viewMat);
//        shader.attachToUniform("Model", modelMat);
//        axes.Draw(shader);
//        model.Draw(shader);
        
		// ****************************************************************************
		// ****************************************************************************
		// ****************************************************************************
		
		sdlContext.swapbuffer();
	}

		
}

//**************************************************************
// SDL event handler function
//**************************************************************

bool handleEvents(SDL_Event & evt, sdlWrapper & sdlContext) {
//    std::cout<<evt.type<<std::endl;
	// Poll all events that have occurred
	while (SDL_PollEvent(&evt)) {
//        std::cout<<"while"<<std::endl;
		// If Quit ( X in window is pressed)
		if (evt.type == SDL_QUIT) {
			return false;
		}

		// Handle Keyboard events
		if (evt.type == SDL_KEYDOWN) {
			// Quit if escape key is pressed
			if (evt.key.keysym.sym == SDLK_ESCAPE) {
				return false;
			}

			// Toggle Fullscreen
			if (evt.key.keysym.sym == SDLK_f) {
				sdlContext.toggleFullScreen();
			}

			// Toggle Wireframe
			if (evt.key.keysym.sym == SDLK_w) {
				if (bWireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					bWireframe = false;
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					bWireframe = true;
				}
			}
		}
        
//        if (evt.type == SDL_KEYDOWN){
//            if (evt.key.keysym.sym == SDLK_a && State != state::BUTTON_A){
//                std::cout<<"a click"<<std::endl;
//                mousePressedX = evt.button.x;
//                mousePressedY = evt.button.y;
//                State = state::BUTTON_A;
//            }
//        }
//
//        if (evt.type == SDL_KEYUP){
//            if (evt.key.keysym.sym == SDLK_a){
//                std::cout<<"sdsdsds"<<std::endl;
//                lastXOffset += evt.button.x - mousePressedX;
//                lastYOffset += mousePressedY - evt.button.y;
//                mousePressedX = 0;
//                mousePressedY = 0;
//                State = state::INIT;
//            }
//        }
		// ****************************************************************************
		// ************************** Insert your Code here ***************************
		// ****************************************************************************
//        std::cout<<"mouse"<<std::endl;
		// Handle Mouse Click Events
		if (evt.type == SDL_MOUSEBUTTONDOWN) {
//            std::cout<<"button"<<std::endl;
			switch (evt.button.button) {
                case SDL_BUTTON_LEFT:
                    std::cout<<"left click"<<std::endl;
                    mousePressedX = evt.button.x;
                    mousePressedY = evt.button.y;
                    State = state::BUTTON_LEFT;
                    break;
                case SDL_BUTTON_RIGHT:
                    std::cout<<"right click"<<std::endl;
                    mousePressedX = evt.button.x;
                    mousePressedY = evt.button.y;
                    State = state::BUTTON_RIGHT;
                    break;
                case SDL_BUTTON_MIDDLE:
                    std::cout<<"middle click"<<std::endl;
                    mousePressedX = evt.button.x;
                    mousePressedY = evt.button.y;
                    State = state::BUTTON_MIDDLE;
                    break;
			}
		}

		if (evt.type == SDL_MOUSEBUTTONUP) {
			switch (evt.button.button) {
                case SDL_BUTTON_LEFT:
                    std::cout<<"sdsdsds"<<std::endl;
                    lastYRotate += mousePressedX - evt.motion.x;
                    lastXRotate += mousePressedY - evt.motion.y;
                    mousePressedX = 0;
                    mousePressedY = 0;
                    State = state::INIT;
                    std::cout<<lastYRotate<<std::endl;
                    break;
                case SDL_BUTTON_RIGHT:
                    lastZOffset += evt.button.x - mousePressedX;
                    mousePressedX = 0;
                    mousePressedY = 0;
                    State = state::INIT;
                    break;
                case SDL_BUTTON_MIDDLE:
                    std::cout<<"sdsdsds"<<std::endl;
                    lastXOffset += evt.button.x - mousePressedX;
                    std::cout<<lastXOffset<<std::endl;
                    lastYOffset += mousePressedY - evt.button.y;
                    mousePressedX = 0;
                    mousePressedY = 0;
                    State = state::INIT;
                    break;
			}
		}

		// Handle Mouse Motion Events
		if (evt.type == SDL_MOUSEMOTION) {
//            std::cout<<"state"<<std::endl;
		}

		// ****************************************************************************
		// ****************************************************************************
		// ****************************************************************************
	}
	return true;
}

