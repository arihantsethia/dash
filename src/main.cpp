#include "headers/generic.h"
using namespace std;
int counter = 0;

int s(){
	cout<<counter<<endl;
	return counter++;
}


int main(){
	std::vector<int> s(5,s());

	return 0;
}