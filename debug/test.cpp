#include <ctime>
#include <cstdio>
#include <vector>
#include <cstdlib>
using namespace std;
double a=114514.1919810,b;
int main(){
	freopen("./testout","wb",stdin);
	for(uint32_t i=0;i<sizeof(double);i++){
		putchar((unsigned char)(&a)[i]);
	}
	fclose(stdin);
	freopen("./testout","rb",stdout);
	for(uint32_t i=0;i<sizeof(double);i++){
		(&a)[i]=getc(stdin);
	}
	fclose(stdout);
	printf("%lf\n",b);
	return 0;
}
