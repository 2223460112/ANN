#include<bits/stdc++.h>
using namespace std;
int main(){
	auto time0=clock();
	vector<int> t;
	for(int i=0;i<1000000;i++)t.resize(t.size()+1);
	printf("%lu\n",clock()-time0);
	return 0;
}



