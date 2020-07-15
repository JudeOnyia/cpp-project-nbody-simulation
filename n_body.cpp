#include "n_body.hpp"
#include<iostream>

int main(){
	using std::cout;
	using std::endl;
	using type_t = long double;
	using vec = typename n_body::vector<type_t>;
	vec objA;
	vec objB(8.9999,3.55);
	vec objC(3.7E39, 2.56E-12);
	vec objD(objB);
	vec objE = objD;
	cout<<(objE==objD)<<endl;
	cout<<(objE==objA)<<endl;
	cout<<(objE!=objA)<<endl;
	cout<<(objE!=objD)<<endl;
	objE+=objD;
	objC-=objB;
	objE*=objD;
	objAA = objE+objD;
	objAB = objE-objD;
	objAC = objC*objB;
	cout<<(objC.norm())<<endl;




}