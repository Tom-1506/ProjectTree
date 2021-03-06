#include <iostream>
#include "Cylinder.h"
#include ".\shaders\Shader.h"

const double PI = 3.14159265358979323846f;

Cylinder::Cylinder() 
{
	cbx, cby, cbz, ctx, cty, ctz = 0.0f;
	r, rb, rt = 0.0;
}

void Cylinder::setRadii(float radBase, float radTop) 
{
    r = radBase;
	rb = radBase;
	rt = radTop;
}

void Cylinder::setBaseCentre(glm::vec3 centre)
{
	cbx = centre.x;
	cby = centre.y;
	cbz = centre.z;
}

glm::vec3 Cylinder::getBaseCentre()
{
	return glm::vec3(cbx, cby, cbz);
}

void Cylinder::setTopCentre(glm::vec3 centre)
{
	ctx = centre.x;
	cty = centre.y;
	ctz = centre.z;
}

glm::vec3 Cylinder::getTopCentre()
{
	return glm::vec3(ctx, cty, ctz);
}

void Cylinder::render()
{
	//draw objects
	glBindVertexArray(m_vaoID);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	//glDrawElements(GL_TRIANGLES, numOfTris * 3, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 0, numOfVerts);

	//Done
	glBindVertexArray(0); //unbind vertex array object
}

void Cylinder::calcUnitCircleVertices(int level)
{
	std::vector<float>().swap(unitVertices);

	const float PI = 3.1415926f;
	float sectorNum = 2 * PI / level;
	float sectorAngle;  // radian
	
	for (int i = 0; i <= level; ++i)
	{
		sectorAngle = i * level;
		unitVertices.push_back(cos(sectorAngle)); // x
		unitVertices.push_back(0);			      // y
		unitVertices.push_back(sin(sectorAngle)); // z
	}
}

void Cylinder::constructGeometry(Shader* myShader, int level)
{
	//Currently being used to generate a line
	verts = new float[3 * 3];
	norms = new float[3 * 3];
	tInds = new unsigned int[1 * 3];

	numOfVerts = 3;
	numOfTris = 1;

	verts[0] = cbx;
	verts[1] = cby;
	verts[2] = cbz;

	norms[0] = 0;
	norms[1] = -1;
	norms[2] = 0;

	verts[3] = ctx;
	verts[4] = cty;
	verts[5] = ctz;

	norms[3] = 0;
	norms[4] = 1;
	norms[5] = 0;

	verts[6] = cbx;
	verts[7] = cby;
	verts[8] = cbz;

	norms[6] = 0;
	norms[7] = -1;
	norms[8] = 0;

	tInds[0] = 1;
	//tInds[1] = 1;
	//tInds[2] = 1;

	//assign the data to the GPU
	// VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(2, m_vboID);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);
	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, numOfVerts * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);
	GLint vertexLocation = glGetAttribLocation(myShader->handle(), "in_Position");
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexLocation);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, numOfVerts * 3 * sizeof(GLfloat), norms, GL_STATIC_DRAW);
	GLint normalLocation = glGetAttribLocation(myShader->handle(), "in_Normal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalLocation);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tInds, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
