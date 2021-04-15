#include<iostream>
using namespace std;

#include <string>
#include <vector>

#include "Apex.h"

class LSystem {
public:
	void setAxiom(string a) {
		axiom = a;
	}

protected:
	string axiom;
};