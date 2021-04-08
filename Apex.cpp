#include "Apex.h"

Apex::Apex(double inRotateAlpha, double inRotatePhi, double inLength, double inWidth)
{
	rotateAlpha = inRotateAlpha;
	rotatePhi = inRotatePhi;
	length = inLength;
	width = inWidth;

	isRoot = true;
	localRoot = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Apex::setParent(Apex* inParent) {
	parent = inParent;
	parent->addChild(this);
	isRoot = false;
}

void Apex::addChild(Apex* child) {
	if (children.first == NULL) {
		children.first = child;
	}
	else if (children.second == NULL) {
		children.second = child;
	}
}

void Apex::setLocalRoot(glm::vec3 parentPos) {
	localRoot = parentPos;
}