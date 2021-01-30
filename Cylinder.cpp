#include "Cylinder.h"
#include ".\shaders\Shader.h"

const double PI = 3.14159265358979323846f;

Cylinder::Cylinder() 
{
	cx, cy, cz = 0.0;
	rb, rt = 0.0;
}

void Cylinder::setRadii(float radBase, float radTop) 
{
	rb = radBase;
	rt = radTop;
}

void Cylinder::setBaseCentre(glm::vec3 centre)
{
	cx = centre.x;
	cy = centre.y;
	cz = centre.z;
}

glm::vec3 Cylinder::getBaseCentre()
{
	return glm::vec3(cx, cy, cz);
}

void Cylinder::render()
{
	//draw objects
	glBindVertexArray(m_vaoID);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, numOfTris * 3, GL_UNSIGNED_INT, 0);

	//Done
	glBindVertexArray(0); //unbind vertex array object
}

void Cylinder::constructGeometry(Shader* myShader, int level)
{
	verts = new float[2 + (2 * level)]; //2 centre verts for each circle
	norms = new float[2 + (2 * level)]; //and 2 sets of the outer verts of each circle


}
