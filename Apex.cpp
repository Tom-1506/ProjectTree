#include "Apex.h"

Apex::Apex(float inRotateAlpha, float inRotatePhi, double inLength, double inWidth)
{
	rotateAlpha = inRotateAlpha;
	rotatePhi = inRotatePhi;
	length = inLength;
	width = inWidth;

	rotationMatrix = glm::mat4(1.0);

	isRoot = true;
	level = 0;
	localRoot = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Apex::setParent(Apex* inParent) {
	parent = inParent;
	parent->addChild(this);
	isRoot = false;

	rotationMatrix = glm::rotate(parent->rotationMatrix, glm::radians(rotateAlpha), glm::vec3(1.0f,0.0f,0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotatePhi), glm::vec3(0, 1, 0));
	
	level = parent->level + 1;
}

void Apex::addChild(Apex* child) {
	if (children.first == NULL) {
		children.first = child;
	}
	else if (children.second == NULL) {
		children.second = child;
	}
}

bool Apex::hasChildren() {
	if (children.first == NULL && children.second == NULL) {
		return false;
	}
	else {
		return true;
	}
}

void Apex::setLocalRoot(glm::vec3 parentPos) {
	localRoot = parentPos;
}