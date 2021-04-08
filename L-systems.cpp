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
	string getSystemString(double startBranchLength, double minBranchLength, double startBranchWidth,
		double angleAlpha1, double angleAlpha2, double anglePhi1, double anglePhi2,
		double lengthDegrade1, double lengthDegrade2) //add width changers
	{ 
		string result = axiom;
		std::vector<Apex> apices;

		double branchLength = startBranchLength;
		double branchWidth = startBranchWidth;
		double a1 = angleAlpha1;
		double a2 = angleAlpha2;
		double p1 = anglePhi1;
		double p2 = anglePhi2;
		double r1 = lengthDegrade1;
		double r2 = lengthDegrade2;

		Apex root(0.0f, 0.0f, branchLength, branchWidth);
		Apex apex1(angleAlpha1, anglePhi1, branchLength, branchWidth);
		Apex apex2(angleAlpha2, anglePhi2, branchLength, branchWidth);

		apices.push_back(root);
		apices.push_back(apex1);
		apices.push_back(apex2);

		std::cout << "HERE" << std::endl;

		//separate recursive function to add apices

		return result;
	}

	void addApex() {

	}
};