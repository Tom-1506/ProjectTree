#include<iostream>
using namespace std;

#include <string>
#include <vector>

#include "Apex.cpp"

class LSystem {
public:
	void setAxiom(string a) {
		axiom = a;
	}

protected:
	string axiom;
};