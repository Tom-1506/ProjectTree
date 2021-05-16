#include "L-systems.h"

class BinaryTree : public LSystem {
	/*
	Production Rules :

	a->b[a]a
	b->bb

	a : draw line segment ending in a leaf
	b : draw line segment
	[: push position and angle, turn left 45
	] : pop positionand angle, turn right 45
	*/
	public:
		string getSystemString(int recursions) {
			string result = axiom;
			string newString = "";
			int r = 0;

			while (r < recursions) {
				r++;
				newString = "";
				for (char& c : result) {
					if (c == 'a') {
						newString += "b[a]a";
					}
					else if (c == 'b') {
						newString += "bb";
					}
					else {
						newString += c;
					}
				}
				result = newString;
			}
			return result;
		}
};

class BarnsleyFern : public LSystem {
	/*
	Production Rules:

	a -> b+[[a]-a]-b[-ba]+a
	b -> bb

	angle = 25 degrees

	a: not used for drawing, only string construction
	b: draw line segment
	-: turn right by angle
	+: turn left by angle
	[: push position and angle
	]: pop position and angle
	*/
public:
	string getSystemString(int recursions) {
		string result = axiom;
		string newString = "";
		int r = 0;

		while (r < recursions) {
			r++;
			newString = "";
			for (char& c : result) {
				if (c == 'a') {
					newString += "b+[[a]-a]-b[-ba]+a";
				}
				else if (c == 'b') {
					newString += "bb";
				}
				else {
					newString += c;
				}
			}
			result = newString;
		}
		return result;
	}
};

class BasicTree : public LSystem {
	/*
	Production Rules :

	*/
public:
	std::vector<Apex*> getApices(glm::vec3 rootPos, double startBranchLength, double min, double startBranchWidth,
								float a1, float a2, float p1, float p2, double r1, double r2, int count) // TODO add width changers
	{ 
		string result = axiom;
		std::vector<Apex*> apices;

		double w1 = startBranchWidth;
		double w2 = startBranchWidth;

		Apex* root = new Apex(0.0f, 0.0f, startBranchLength, startBranchWidth);
		root->setLocalRoot(rootPos);

		apices = constructTree(root, apices, a1, a2, p1, p2, startBranchLength*r1, startBranchLength*r2, r1, r2, min, w1, w2, count);

		return apices;
	}

	std::vector<Apex*> constructTree(Apex* root, std::vector<Apex*> apices, float a1, float a2, float p1, float p2,
									 double l1, double l2, double r1, double r2, double min, double w1, double w2, int count) {
		Apex* apex1 = new Apex(a1, p1, l1*r1, w1);
		apex1->setParent(root);

		Apex* apex2 = new Apex(a2, p2, l2*r2, w2);
		apex2->setParent(root);

		apices.push_back(root);

		// update variables
		count--;

		if(count > 0){
			if (l1 > min) {
				apices = constructTree(apex1, apices, a1, a2, p1, p2, apex1->length, apex1->length, r1, r2, min, w1, w2, count);
			}
			if (l2 > min) {
				apices = constructTree(apex2, apices, a1, a2, p1, p2, apex2->length, apex2->length, r1, r2, min, w1, w2, count);
			}
		}
		
		return apices;
	}
};