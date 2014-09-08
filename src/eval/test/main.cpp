#include <iostream>
#include <string>
#include "eval.h"

using namespace std;

string cases[] = {"sin(3.14/2)+cos(3.14/2)+pow(2,3)+sqrt(3)+lg(100)+ln(7)",
                  "12.4+2*13-53.345/3+2^3+7%4",
                  "2*sin(3.14/lg(100))+pow(pow(2,1+2),2) + lg(100)",
                  "2.3+5*4--6",
                  "2+3*sin1(3.14/2)"};

int main()
{
	double result;
	int ncase = sizeof(cases)/sizeof(cases[0]);
	for(int i = 0; i < ncase; ++i)
	{
		cout << cases[i] << " = ";
		if(eval(cases[i], &result))
			cout << result << endl;
		else
			cout << "wrong expression" << endl;
	}
	return 0;
}

