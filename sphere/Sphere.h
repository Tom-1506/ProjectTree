
#include "../GL/glew.h"
#include "../glm\glm.hpp"

class Shader;

class Sphere
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
	float cx,cy,cz; //centre of the sphere
	float r;		//radius of the sphere
	Sphere();
	void constructGeometry(Shader* myShader, int level);
	void render();
	void setRadius(float rad);
	void setCentre(glm::vec3 centre);
	glm::vec3 getCentre();
};