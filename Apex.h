#include<iostream>
using namespace std;

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_inverse.hpp"

#include<vector>

class Apex {
public:
	float rotateAlpha;
	float rotatePhi;
	double length;
	double width;

	Apex* parent;
	pair<Apex*, Apex*> children;

	bool isRoot;
	glm::vec3 localRoot;

	int level;

	glm::mat4 rotationMatrix;

	Apex(float rotateAlpha, float rotatePhi, double length, double width);

	void setParent(Apex* inParent);
	void addChild(Apex* child);
	bool hasChildren();
	void setLocalRoot(glm::vec3 parentPos);
};