#include <iostream>
#include "Schelling.h"
using namespace std;

int main () {
	int threshold=30, xPercent=50, emptyPercent=10, size=50;
	bool defaults=0;
	string in;
	cout<<"Accept defaults? [y/n]: ";
	getline(cin,in);
	if (in=="y")
		defaults=1;

	if (!defaults) {
		cout<<"Enter size of grid: ";
		cin>>size;
		cout<<"Enter threshold percent of similar neighbors needed for satisfaction: ";
		cin>>threshold;
		cout<<"Enter percent of cells to be empty: ";
		cin>>emptyPercent;
		cout<<"Enter percent of filled cells to be 'X': ";
		cin>>xPercent;
	}

	Schelling model(threshold, xPercent, emptyPercent, size);
	model.run();

  return 0;
}
