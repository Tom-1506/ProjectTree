#include<iostream>
using namespace std;

#include<vector>

class Apex {
public:
	double rotateAlpha;
	double rotatePhi;
	double length;
	double width;
	Apex* parent;
	Apex* leftChild;
	Apex* rightChild;

	Apex(double rotateAlpha, double rotatePhi, double length, double width,
		Apex parent, Apex leftChild, Apex rightChild);
};

Apex::Apex(double inRotateAlpha, double inRotatePhi, double inLength, double inWidth,
	Apex inParent, Apex inLeftChild, Apex inRightChild) 
{
	rotateAlpha = inRotateAlpha;
	rotatePhi = inRotatePhi;
	length = inLength;
	width = inWidth;
	parent = &inParent;
	leftChild = &inLeftChild;
	rightChild = &inRightChild;
}