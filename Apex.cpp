#include "Apex.h"

Apex::Apex(double inRotateAlpha, double inRotatePhi, double inLength, double inWidth)
{
	rotateAlpha = inRotateAlpha;
	rotatePhi = inRotatePhi;
	length = inLength;
	width = inWidth;

	isRoot = true;
	level = 0;
	left = 0;
	right = 0;
	localRoot = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Apex::setParent(Apex* inParent) {
	parent = inParent;
	parent->addChild(this);
	isRoot = false;
	
	level = parent->level + 1;

	if (rotateAlpha > 0) {
		left = parent->left + 1;
		right = parent->right;
	}
	else {
		left = parent->left;
		right = parent->right + 1;
	}
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