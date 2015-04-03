#include "headers/generic.h"
#include "headers/database.h"

using namespace std;

void arihant_func(){
	Database a("tst");
	a.use_table(1);
	vector<key_value> s;
	vector<t_value> k;
	for(int i=1;i<25000000;i++){
		s.push_back(make_pair(i,2*i));
		k.push_back(i);
	}
	cout<<"done"<<endl;
	a.put(s);
	cout<<"write finished1"<<endl;
	//a.put(s);
	cout<<"write finished"<<endl;
	cout<<"get started"<<endl;
	vector<vector<t_value> >&& res = a.get(k);
	cout<<res.size()<<endl;
}

int main(){
	arihant_func();
	return 0;
}