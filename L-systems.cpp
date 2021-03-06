#include<iostream>

using namespace std;

class LSystem {
	public:
		virtual string getSystemString(int recursions) = 0;
		
		void setAxiom(string a) {
			axiom = a;
		}

	protected:
		string axiom;
};

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