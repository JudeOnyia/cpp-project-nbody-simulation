#include "n_body.hpp"
#include<iostream>

int main(){
	using std::cout;
	using std::endl;
	using type_t = long double;
	using vec = typename n_body::vector<type_t>;
	using body = typename n_body::body<type_t>;

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
	vec objAA = objE+objD;
	vec objAB = objE-objD;
	vec objAC = objC*objB;
	cout<<(objC.norm())<<endl;
	vec objAD = objAA * type_t(23.78);
	vec objAE = type_t(23.78) * objAA;
	cout<<(objAA.x)<<" "<<(objAA.y)<<endl;
	cout<<(objAD.x)<<" "<<(objAD.y)<<endl;
	cout<<(objAE.x)<<" "<<(objAE.y)<<endl;

	type_t mA(100); vec pA(3,4); vec vA(12,-15); vec aA(-1,0.88);
	body bdA(mA,pA,vA,aA);
	type_t mB(400); vec pB(32.5,-14); vec vB(20,3.11); vec aB(40,57);
	body bdB(mB,pB,vB,aB);
	vec fab = bdA.individual_force(bdB);
	vec dist = bdA.distance(bdB);
	cout<<"fab: ("<<(fab.x)<<","<<(fab.y)<<")"<<endl;
	cout<<"dist: ("<<(dist.x)<<","<<(dist.y)<<")"<<endl;

	body* bd = new body[3];
	bd[0] = bdA;
	bd[1] = bdB;
	type_t mC(713.592); vec pC(-11,54); vec vC(12,-15); vec aC(-10,0.88);
	bd[2] = body(mC,pC,vC,aC);
	n_body::work_in_time_step(bd,3);
	vec fa_sum = bd[0].individual_force(bd[1]);
	fa_sum += bd[0].individual_force(bd[2]);
	cout<<"fa_sum: ("<<(fa_sum.x)<<","<<(fa_sum.y)<<")"<<endl;
	vec fb_sum = bd[1].individual_force(bd[0]);
	fb_sum += bd[1].individual_force(bd[2]);
	cout<<"fb_sum: ("<<(fb_sum.x)<<","<<(fb_sum.y)<<")"<<endl;
	vec fc_sum = bd[2].individual_force(bd[0]);
	fc_sum += bd[2].individual_force(bd[1]);
	cout<<"fc_sum: ("<<(fc_sum.x)<<","<<(fc_sum.y)<<")"<<endl;


	delete[] bd;



}
