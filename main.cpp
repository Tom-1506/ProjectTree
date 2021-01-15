#include <iostream>
using namespace std;


//--- OpenGL ---
#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include "GL\freeglut.h"

#include "Images\FreeImage.h"

#include "shaders\Shader.h"

glm::mat4 objectTransformation;

Shader* myShader;  ///shader object 
Shader* myBasicShader;

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"



float amount = 0;
float temp = 0.002f;
float moveSpeed = 0.05f;

ThreeDModel models[3];
ThreeDModel model; //A threeDModel object is needed for each model loaded
OBJLoader objLoader;	//this object is used to load the 3d models.
///END MODEL LOADING

glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelViewMatrix;  // matrix for the modelling and viewing

glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f); //vector for the position of the object.

//Material properties
float Material_Ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
float Material_Diffuse[4] = {0.8f, 0.8f, 0.5f, 1.0f};
float Material_Specular[4] = {0.9f,0.9f,0.8f,1.0f};
float Material_Shininess = 50;

//Light Properties
float Light_Ambient_And_Diffuse[4] = {0.8f, 0.8f, 0.6f, 1.0f};
float Light_Specular[4] = {1.0f,1.0f,1.0f,1.0f};
float LightPos[4] = {0.0f, 0.0f, 1.0f, 0.0f};

//
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;

//booleans to handle when the arrow keys are pressed or released.
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
bool Home = false;
bool End = false;
bool Go = false;

bool Xplus = false;
bool Xminus = false;
bool Yplus = false;
bool Yminus = false;
bool Zplus = false;
bool Zminus = false;

float spin=180;
float speed=0;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void idle();		//idle function
void updateRotate(float xinc, float yinc, float zinc);
void updateTranslate(float xinc, float yinc, float zinc);

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(myShader->handle());  // use the shader

	//amount += temp;
	//if(amount > 1.0f || amount < -1.5f)
	//	temp = -temp;
	//amount = 0;
	//glUniform1f(glGetUniformLocation(myShader->handle(), "displacement"), amount);

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");  
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glm::mat4 viewingMatrix = glm::mat4(1.0f);
	
	//translation and rotation for view
	viewingMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -50));
	/*static float angle = 0.0f;
	angle += 0.0001;
	viewingMatrix = glm::rotate(viewingMatrix, angle, glm::vec3(1.0f, 0.0f, 0.0));*/

	//use of glm::lookAt
	//viewingMatrix = glm::lookAt(glm::vec3(-38, 19, 84), glm::vec3(0,0,0), glm::vec3(0.0f, 1.0f, 0.0));

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), pos);
	ModelViewMatrix = viewingMatrix * modelmatrix * objectTransformation;
	//ModelViewMatrix = viewingMatrix * objectRotation;
	
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);
	
	model.drawElementsUsingVBO(myShader);
	
	
	glUseProgram(myBasicShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	
	//model.drawBoundingBox(myBasicShader);
	//model.drawOctreeLeaves(myBasicShader);
	
	glUseProgram(myShader->handle());  // use the shader

	ModelViewMatrix = glm::translate(viewingMatrix, glm::vec3(0, 0, 0));

	normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	//Pass the uniform for the modelview matrix - in this case just "r"
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
	models[0].drawElementsUsingVBO(myShader);
	models[1].drawElementsUsingVBO(myShader);
	//boxFront.drawElementsUsingVBO(myShader);

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (GLfloat)screenWidth/(GLfloat)screenHeight, 1.0f, 200.0f);
}
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);

	myShader = new Shader();
	//if(!myShader->load("BasicView", "glslfiles/basicTransformationsWithDisplacement.vert", "glslfiles/basicTransformationsWithDisplacement.frag"))
    if(!myShader->load("BasicView", "glslfiles/basicTransformations.vert", "glslfiles/basicTransformations.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	myBasicShader = new Shader();
	if(!myBasicShader->load("Basic", "glslfiles/basic.vert", "glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}		

	glUseProgram(myShader->handle());  // use the shader

	glEnable(GL_TEXTURE_2D);

	//lets initialise our object's rotation transformation 
	//to the identity matrix
	objectTransformation = glm::mat4(1.0f);

	cout << " loading model " << endl;
	if(objLoader.loadModel("TestModels/conjoined.obj", model))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded " << endl;		

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		//model.calcCentrePoint();
		//model.centreOnZero();

	
		model.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.
				

		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model.initDrawElements();
		model.initVBO(myShader);
	}
	else
	{
		cout << " model failed to load " << endl;
	}

	models[0].fileName = "TestModels/boxLeft.obj";
	models[1].fileName = "TestModels/boxRight.obj";
	models[2].fileName = "TestModels/boxFront.obj";

	for (ThreeDModel &box : models) {
		if (objLoader.loadModel(const_cast<char*>(box.fileName.c_str()), box))//returns true if the model is loaded, puts the model in the model parameter
		{
			box.calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.
			box.initDrawElements();
			box.initVBO(myShader);
		}
		else
		{
			cout << " model failed to load " << endl;
		}
	}	
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = true;
		break;
	case GLUT_KEY_RIGHT:
		Right = true;
		break;
	case GLUT_KEY_UP:
		Up = true;
		break;
	case GLUT_KEY_DOWN:
		Down = true;
		break;
	case GLUT_KEY_HOME:
		moveSpeed += 0.03f;
		break;
	case GLUT_KEY_END:
		moveSpeed -= 0.03f;
		break;
	}
}

void specialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = false;
		break;
	case GLUT_KEY_RIGHT:
		Right = false;
		break;
	case GLUT_KEY_UP:
		Up = false;
		break;
	case GLUT_KEY_DOWN:
		Down = false;
		break;
	}
}

void keyfunction(unsigned char key, int x, int y)
{
	switch (key) {
	case 48:
		Home = true;
		break;
	case 49:
		End = true;
		break;
	case 111:
		Yplus = true;
		break;
	case 112:
		Yminus = true;
		break;
	case 97:
		Xplus = true;
		break;
	case 100:
		Xminus = true;
		break;
	case 119:
		Zplus = true;
		break;
	case 115:
		Zminus = true;
		break;
	}
}

void keyfunctionUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 48:
		Home = false;
		break;
	case 49:
		End = false;
		break;
	case 111:
		Yplus = false;
		break;
	case 112:
		Yminus = false;
		break;
	case 97:
		Xplus = false;
		break;
	case 100:
		Xminus = false;
		break;
	case 119:
		Zplus = false;
		break;
	case 115:
		Zminus = false;
		break;
	}
}

void processKeys()
{
	float spinXinc = 0.0f, spinYinc = 0.0f, spinZinc = 0.0f;
	if (Left)
	{
		spinYinc = -0.05f;
	}
	if (Right)
	{
		spinYinc = 0.05f;
	}
	if (Up)
	{
		spinXinc = 0.05f;
	}
	if (Down)
	{
		spinXinc = -0.05f;
	}
	if (Home)
	{
		spinZinc = 0.05f;
	}
	if (End)
	{
		spinZinc = -0.05f;
	}
	if (Xplus) {
		pos.x += objectTransformation[0][0] * moveSpeed;
		pos.y += objectTransformation[0][1] * moveSpeed;
		pos.z += objectTransformation[0][2] * moveSpeed;
	}
	if (Xminus) {
		pos.x -= objectTransformation[0][0] * moveSpeed;
		pos.y -= objectTransformation[0][1] * moveSpeed;
		pos.z -= objectTransformation[0][2] * moveSpeed;
	}
	if (Yplus) {
		pos.x += objectTransformation[1][0] * moveSpeed;
		pos.y += objectTransformation[1][1] * moveSpeed;
		pos.z += objectTransformation[1][2] * moveSpeed;
	}
	if (Yminus) {
		pos.x -= objectTransformation[1][0] * moveSpeed;
		pos.y -= objectTransformation[1][1] * moveSpeed;
		pos.z -= objectTransformation[1][2] * moveSpeed;
	}
	if (Zplus) {
		pos.x += objectTransformation[2][0] * moveSpeed;
		pos.y += objectTransformation[2][1] * moveSpeed;
		pos.z += objectTransformation[2][2] * moveSpeed;
	}
	if (Zminus) {
		pos.x -= objectTransformation[2][0] * moveSpeed;
		pos.y -= objectTransformation[2][1] * moveSpeed;
		pos.z -= objectTransformation[2][2] * moveSpeed;
	}
	updateRotate(spinXinc, spinYinc, spinZinc);
}

void updateRotate(float xinc, float yinc, float zinc)
{
	objectTransformation = glm::rotate(objectTransformation, xinc, glm::vec3(1,0,0));
	objectTransformation = glm::rotate(objectTransformation, yinc, glm::vec3(0,1,0));
	objectTransformation = glm::rotate(objectTransformation, zinc, glm::vec3(0,0,1));
}

void idle()
{
	spin += speed;
	if(spin > 360)
		spin = 0;

	processKeys();

	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Obj loading");

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;


	//initialise the objects for rendering
	init();

	glutReshapeFunc(reshape);
	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	glutKeyboardFunc(keyfunction);
	glutKeyboardUpFunc(keyfunctionUp);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}
