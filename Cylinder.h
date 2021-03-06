
#include "./GL/glew.h"
#include "./glm\glm.hpp"
#include <vector>

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

	std::vector<float> unitVertices;
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> texCoords;

public:
	float cbx, cby, cbz; //centre of base circle
	float ctx, cty, ctz; //centre of top circle
	float r, rb, rt; //radius of base and top circle
	float h; //height of cylinder

	Cylinder();
	void calcUnitCircleVertices(int level);
	void constructGeometry(Shader* myShader, int level);
	void render();
	void setRadii(float radBase, float radTop);
	void setBaseCentre(glm::vec3 centre);
	glm::vec3 getBaseCentre();
	void setTopCentre(glm::vec3 centre);
	glm::vec3 getTopCentre();
};