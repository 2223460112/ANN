#include <ctime>
#include <cstdio>
#include <vector>
using namespace std;
vector<int> a;
int main(){
	for(int i=0;i<100000;i++){
		a.push_back(1);
		printf("%u\n",a.begin());
	}
	return 0;
}
