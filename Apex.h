#include<iostream>
using namespace std;

#include "glm\glm.hpp"

#include<vector>

class Apex {
public:
	double rotateAlpha;
	double rotatePhi;
	double length;
	double width;
	Apex* parent;
	bool isRoot;
	glm::vec3 localRoot;
	pair<Apex*, Apex*> children;

	Apex(double rotateAlpha, double rotatePhi, double length, double width);

	void setParent(Apex* inParent);
	void addChild(Apex* child);
	void setLocalRoot(glm::vec3 parentPos);
};