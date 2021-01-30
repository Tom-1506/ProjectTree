
#include "./GL/glew.h"
#include "./glm\glm.hpp"

class Shader;

class Cylinder
{
private:
	unsigned int m_vaoID;		    // vertex array object
	unsigned int m_vboID[3];		// two VBOs - used for colours, vertex and normals data
	GLuint ibo;                     //identifier for the triangle indices

	int numOfVerts;
	int numOfTris;

	float* verts;
	float* norms;
	unsigned int* tInds;

public:
	float cx, cy, cz; //centre of base circle
	float rb, rt; //radius of base and top circle
	float h; //height of cylinder

	Cylinder();
	void constructGeometry(Shader* myShader, int level);
	void render();
	void setRadii(float radBase, float radTop);
	void setBaseCentre(glm::vec3);
	glm::vec3 getBaseCentre();
};