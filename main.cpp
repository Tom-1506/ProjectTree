#include <iostream>
using namespace std;
#define _USE_MATH_DEFINES
#include <math.h>

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

glm::mat4 worldTransformation;
glm::mat4 objectTransformation;

glm::mat4 treeTransformation;
glm::mat4 treeTransforms[3];

Shader* myShader;  ///shader object 
Shader* myBasicShader;

//MODEL LOADING
#include "3DStruct\threeDModel.h"
#include "Obj\OBJLoader.h"
#include "sphere/Sphere.h"
#include "Cylinder.h"
#include "L-systems.cpp"

float amount = 0;
float temp = 0.002f;
float moveSpeed = 0.05f;

glm::vec3 rootPos = glm::vec3(0.0f, 0.0f, 0.0f);
bool printString = true;
BinaryTree bTree;
BarnsleyFern bFern;
BasicTree basicTree;

std::vector<Cylinder> cylinders;

GLUquadric* qobj;

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
float Light_Ambient_And_Diffuse[4] = { 1.4f, 1.4f, 1.34f, 1.6f };
float Light_Specular[4] = { 1.6f,1.6f,1.54f,1.6f };
float LightPos[4] = {0.0f, 10.0f, 1.0f, 0.0f};

//
int	mouse_x=0, mouse_y=0;
bool LeftPressed = false;
int screenWidth=600, screenHeight=600;

//booleans to handle when the arrow keys are pressed or released.
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;

float cameraTilt = 0.0f;
float cameraPan = 0.0f;
bool cameraPanUp = false;
bool cameraPanDown = false;
float zoom = 0.0f;
bool zoomIn = false;
bool zoomOut = false;

//Tree vars
float branchLength = 1;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void reshape(int width, int height);				//called when the window is resized
void init();				//called in winmain when the program starts.
void processKeys();         //called in winmain to process keyboard input
void idle();		//idle function
void updateRotate(float xinc, float yinc, float zinc);
void genBinaryTree(BinaryTree bTree, string axiom, int recursions);
void genBarnsleyFern(BarnsleyFern bFern, string axiom, int recursions);
void genBasicTree(BasicTree basicTree, glm::vec3 rootPos,
	double startBranchLength, double minBranchLength, double startBranchWidth,
	float angleAlpha1, float angleAlpha2, float anglePhi1, float anglePhi2,
	double lengthDegrade1, double lengthDegrade2, int count);
Cylinder generateCylinder(glm::vec3 baseCentre, glm::vec3 topCentre, float baseRadius, float topRadius);
glm::vec3 rotatePoint(glm::vec3 point, double length, double rotationAlpha, double rotationPhi);

/*************    START OF OPENGL FUNCTIONS   ****************/
void display()									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(myShader->handle());  // use the shader

	GLuint matLocation = glGetUniformLocation(myShader->handle(), "ProjectionMatrix");  
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	glm::mat4 viewingMatrix = glm::mat4(1.0f);
	
	//translation and rotation for view
	viewingMatrix = glm::lookAt(glm::vec3(4 + zoom, 4 + zoom + cameraPan, 4 + zoom), glm::vec3(0.0f, 3.0f + cameraTilt + cameraPan, 0.0f), glm::vec3(0, 1, 0));

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(myShader->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(myShader->handle(), "material_shininess"), Material_Shininess);

	//CYLINDERS

	glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	ModelViewMatrix = viewingMatrix * modelmatrix * worldTransformation;

	glUseProgram(myShader->handle());

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix)); // lighting normals
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	for (Cylinder c : cylinders) {
		c.render();
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//END CYLINDERS

	// TERRAIN

	modelmatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	ModelViewMatrix = viewingMatrix * modelmatrix * worldTransformation;

	glUniformMatrix4fv(glGetUniformLocation(myShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	normalMatrix = glm::inverseTranspose(glm::mat3(ModelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(myShader->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	//model.drawElementsUsingVBO(myShader);

	glUseProgram(myBasicShader->handle());  // use the shader
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(myBasicShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	// END TERRAIN

	glFlush();
	glutSwapBuffers();
}

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth=width; screenHeight = height;           // to ensure the mouse coordinates match 
														// we will use these values to set the coordinate system

	glViewport(0,0,width,height);						// Reset The Current Viewport

	//Set the projection matrix
	ProjectionMatrix = glm::perspective(glm::radians(60.0f), (GLfloat)screenWidth/(GLfloat)screenHeight, 0.01f, 100.0f);
}

void init()
{
	glClearColor(1.0,1.0,1.0,0.0);						//sets the clear colour to yellow
														//glClear(GL_COLOR_BUFFER_BIT) in the display function
														//will clear the buffer to this colour
	glEnable(GL_DEPTH_TEST);

	myShader = new Shader();
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

	// initialise objects rotation transformations to the identity matrix
	worldTransformation = glm::mat4(1.0f);
	treeTransformation = glm::mat4(1.0f);
	
	cout << " loading model " << endl;
	if(objLoader.loadModel("Models/flatGroundBox.obj", model))//returns true if the model is loaded, puts the model in the model parameter
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

	//genBinaryTree(bTree, "a", 6);
	//genBarnsleyFern(bFern, "a", 5);

	//glm::vec3 rootPos = glm::vec3(0.0f, 0.0f, 0.0f);

	//genBasicTree(basicTree, rootPos, 1.0f, 0.0f, 1, 35.0, -35.0, 0, 0, 0.75, 0.75, 10); //binary tree A
    //genBasicTree(basicTree, rootPos, 1.0f, 0.017f, 1, 27.0f, -68.0f, 0, 0, 0.65, 0.71, 12); //fractal tree B
	//genBasicTree(basicTree, rootPos, 1.0f, 0.005f, 1, 25.0f, -15.0f, 180.0f, 0.0f, 0.5, 0.85, 9); //fractal tree C
	//genBasicTree(basicTree, rootPos, 1.0f, 0.001f, 1, 25.0f, -15.0f, 180.0f, 180.0f, 0.60, 0.85, 10); // tall tree D
	//genBasicTree(basicTree, rootPos, 1.0f, 0.005f, 1, 0, 60.0f, 180.0f, 0, 0.92, 0.37, 15); //fractal tree F
	//genBasicTree(basicTree, glm::vec3(rootPos.x + 7, rootPos.y, rootPos.z), 1.5f, 0.0f, 1, 30.0f, -30.0f, 137.0f, 137.0f, 0.8f, 0.8f, 11); // realistic tree G shifted
	//genBasicTree(basicTree, glm::vec3(rootPos.x, rootPos.y, rootPos.z + 7), 1.0f, 0.25f, 1, 5.0f, -30.0f, -90.0f, 90.0f, 0.95f, 0.75f, 12); // realistic tree H shifted
	//genBasicTree(basicTree, rootPos, 1.5f, 0.0f, 1, 30.0f, -30.0f, 137.0f, 137.0f, 0.8f, 0.8f, 11); // realistic tree G
	//genBasicTree(basicTree, rootPos, 1.0f, 0.25f, 1, 5.0f, -30.0f, -90.0f, 90.0f, 0.95f, 0.75f, 12); // realistic tree H
	genBasicTree(basicTree, rootPos, 1.0f, 0.05f, 1, 30.0f, -5.0f, 137.0f, 137.0f, 0.55f, 0.95f, 12); // realistic tree I
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
	case 43:
		zoomIn = true;
		break;
	case 45:
		zoomOut = true;
		break;
	case 48:
		cameraPanDown = true;
		break;
	case 49:
		cameraPanUp = true;
		break;
	case 27:
		exit(0);
		break;
	}
}

void keyfunctionUp(unsigned char key, int x, int y)
{
	switch (key) {
	case 43:
		zoomIn = false;
		break;
	case 45:
		zoomOut = false;
		break;
	case 48:
		cameraPanDown = false;
		break;
	case 49:
		cameraPanUp = false;
		break;
	}
}

void processKeys()
{
	float spinXinc = 0.0f, spinYinc = 0.0f, spinZinc = 0.0f;
	if (Left)
	{
		spinYinc = -0.015f;
	}
	if (Right)
	{
		spinYinc = 0.015f;
	}
	if (Up) {
		if (cameraTilt < 4.0f) {
			cameraTilt += 0.04;
		}
	}
	if (Down) {
		if (cameraTilt > -4.0f) {
			cameraTilt -= 0.04f;
		}
	}
	if (cameraPanUp) {
		cameraPan += 0.04;
	}
	if (cameraPanDown) {
		if (cameraPan > -4.0f) {
			cameraPan -= 0.04f;
		}
	}
	if (zoomIn) {
		if (zoom > -3.0f) {
			zoom -= 0.04;
		}
	}
	if (zoomOut) {
		zoom += 0.04;
	}
	updateRotate(spinXinc, spinYinc, spinZinc);
}

void updateRotate(float xinc, float yinc, float zinc)
{
	worldTransformation = glm::rotate(worldTransformation, xinc, glm::vec3(1,0,0));
	worldTransformation = glm::rotate(worldTransformation, yinc, glm::vec3(0,1,0));
	worldTransformation = glm::rotate(worldTransformation, zinc, glm::vec3(0,0,1));
}

void idle()
{
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
	glutCreateWindow("Project Tree");

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

	gluDeleteQuadric(qobj);

	return 0;
}

void genBinaryTree(BinaryTree bTree, string axiom, int recursions) {
	bTree.setAxiom(axiom);
	string bTreeString = bTree.getSystemString(recursions);
	if (printString) {
		std::cout << bTreeString << std::endl;
		printString = false;
	}
	
	glm::vec3 currentPos = rootPos;
	std::vector<glm::vec3> posList;
	std::vector<pair<double, double>> angleList;

	double rotationX = 0.0f;
	double rotationZ = 0.0f;
	double length = 0.25f;

	glm::vec3 top;
	glm::vec3 rotatedTop;
	Cylinder cylinder;
	pair<double, double> newAngle;

	for (char& c : bTreeString) {
		if (c == 'a') {
			top = glm::vec3(0.0f, length/2, 0.0f);

			rotatedTop = rotatePoint(top, rotationX, 0.0f, rotationZ);
			top = glm::vec3(rotatedTop.x + currentPos.x, rotatedTop.y + currentPos.y, rotatedTop.z + currentPos.z);
			cylinder = generateCylinder(currentPos, top, 0.5f, 0.5f);
			cylinders.push_back(cylinder);
		}
		else if (c == 'b') {
			top = glm::vec3(0.0f, length, 0.0f);
			rotatedTop = rotatePoint(top, rotationX, 0.0f, rotationZ);
			top = glm::vec3(rotatedTop.x + currentPos.x, rotatedTop.y + currentPos.y, rotatedTop.z + currentPos.z);
			cylinder = generateCylinder(currentPos, top, 0.5f, 0.5f);
			cylinders.push_back(cylinder);
			currentPos = top;
		}
		else if (c == '[') {
			posList.push_back(currentPos);
			angleList.push_back({ rotationX, rotationZ });
			rotationX += 0.6f;
			rotationZ += 0.6f;
		}
		else if (c == ']') {
			currentPos = posList.back();
			posList.pop_back();
			newAngle = angleList.back();
			angleList.pop_back();
			rotationX = newAngle.first;
			rotationZ = newAngle.second;
			rotationX -= 0.6f;
			rotationZ -= 0.6f;
		}
	}
}

void genBarnsleyFern(BarnsleyFern bFern, string axiom, int recursions) {
	bFern.setAxiom(axiom);
	string bFernString = bFern.getSystemString(recursions);
	if (printString) {
		std::cout << bFernString << std::endl;
		printString = false;
	}

	glm::vec3 currentPos = rootPos;
	std::vector<glm::vec3> posList;
	std::vector<pair<double, double>> angleList;

	double rotationX = 0.0f;
	double rotationZ = 0.0f;
	double length = 0.25f;

	glm::vec3 top;
	glm::vec3 rotatedTop;
	Cylinder cylinder;
	pair<double, double> newAngle;
	bool choice;

	for (char& c : bFernString) {
		choice = true;
		if ((rand() % 2) + 1 == 2) {
			choice = false;
		}

		if (choice) {
			rotationX += 0.1f;
		}
		else {
			rotationX -= 0.1f;
		}

		if (c == 'b') {
			top = glm::vec3(0.0f, length, 0.0f);
			rotatedTop = rotatePoint(top, rotationX, 0.0f, rotationZ);
			top = glm::vec3(rotatedTop.x + currentPos.x, rotatedTop.y + currentPos.y, rotatedTop.z + currentPos.z);
			cylinder = generateCylinder(currentPos, top, 0.5f, 0.5f);
			cylinders.push_back(cylinder);
			currentPos = top;
		}
		else if (c == '+') {
			rotationX += 0.2f;
			//rotationZ += 0.2f;
		}
		else if (c == '-') {
			rotationX += 0.2f;
			//rotationZ += 0.2f;
		}
		else if (c == '[') {
			posList.push_back(currentPos);
			angleList.push_back({ rotationX, rotationZ });
		}
		else if (c == ']') {
			currentPos = posList.back();
			posList.pop_back();
			newAngle = angleList.back();
			angleList.pop_back();
			rotationX = newAngle.first;
			rotationZ = newAngle.second;
		}
	}
}

/**
 * Populate cylinder list
 *
 * Using the inputted parameters generate an apices vector
 * and then iterate through the apices and create cylinder
 * objects that can be rendered.
 * 
 * @param basicTree BasicTree object used to call getApices() function
 * @param rootPos Position in world space to generate the root of the tree
 * @param startBranchLength Starting branch length
 * @param minBranchLength Minimum branch length
 * @param startBranchWidth Starting branch width
 * @param angleAlpha1 Rotation about x axis for first child
 * @param angleAlpha2 Rotation about x axis for second child
 * @param anglePhi1 Rotation about y axis for first child
 * @param anglePhi2 Rotation about y axis for second child
 * @param lengthDegrade1 Length degredation value of first child
 * @param lengthDegrade2 Length degredation value of second child
 * @param count number of recursions
 */
void genBasicTree(BasicTree basicTree, glm::vec3 rootPos,
	double startBranchLength, double minBranchLength, double startBranchWidth,
	float angleAlpha1, float angleAlpha2, float anglePhi1, float anglePhi2,
	double lengthDegrade1, double lengthDegrade2, int count) {

	std::vector<Apex*> apices = basicTree.getApices(rootPos, startBranchLength, minBranchLength, startBranchWidth,
													   angleAlpha1, angleAlpha2, anglePhi1, anglePhi2,
													   lengthDegrade1, lengthDegrade2, count);
	glm::vec3 top;
	Cylinder cylinder;

	for (Apex* a : apices) {
		top = glm::vec3(a->rotationMatrix * glm::vec4(0, a->length, 0, 1));
		top = glm::vec3(top.x + a->localRoot.x, top.y + a->localRoot.y, top.z + a->localRoot.z);

		cylinder = generateCylinder(a->localRoot, top, 0.5f, 0.5f);
		cylinders.push_back(cylinder);

		if (a->children.first != NULL) {
			a->children.first->setLocalRoot(top);
		}
		if (a->children.second != NULL) {
			a->children.second->setLocalRoot(top);
		}
	}
}

Cylinder generateCylinder(glm::vec3 baseCentre, glm::vec3 topCentre, float baseRadius, float topRadius) {
	Cylinder c;
	
	c.setBaseCentre(baseCentre);
	c.setTopCentre(topCentre);
	c.setRadii(baseRadius, topRadius);
	c.constructGeometry(myShader, 10);

	return c;
}

// TODO possibly remove since no longer used
glm::vec3 rotatePoint(glm::vec3 point, double length, double rotationAlpha, double rotationPhi) {
	point.x = length * (cos(rotationPhi)) * (sin(rotationAlpha));
	point.y = length * (cos(rotationAlpha));
	point.z = length * (sin(rotationPhi)) * (sin(rotationAlpha));
	return (point);
}