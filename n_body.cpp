#include "n_body.hpp"
#include<iostream>

	using std::cout;
	using std::endl;
	using type_t = long double;
	using vec = n_body::vector<type_t>;
	using body = n_body::body<type_t>;

void printv(vec v){ cout<<"("<<(v.x)<<", "<<(v.y)<<")"<<endl; }
void printb(body b){
	cout<<endl;
	cout<<"   mass: "<<(b.mass)<<endl;
	cout<<"   position: "; printv(b.position);
	cout<<"   velocity: "; printv(b.velocity);
	cout<<"   acceleration: "; printv(b.acceleration);
}

int main(){
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
	printv(objAA);
	printv(objAD);
	printv(objAE);

	type_t mA(100); vec pA(3,4); vec vA(12,-15); vec aA(-1,0.88);
	body bdA(mA,pA,vA,aA);
	type_t mB(400); vec pB(32.5,-14); vec vB(20,3.11); vec aB(40,57);
	body bdB(mB,pB,vB,aB);
	vec fab = bdA.individual_force(bdB);
	vec dist = bdA.distance(bdB);
	cout<<"fab: "; printv(fab);
	cout<<"dist: "; printv(dist);

	body* bd = new body[3];
	bd[0] = bdA;
	bd[1] = bdB;
	type_t mC(713.592); vec pC(-11,54); vec vC(12,-15); vec aC(-10,0.88);
	bd[2] = body(mC,pC,vC,aC);
	//n_body::work_in_time_step(bd,3,0.05);
	vec fa_sum = bd[0].individual_force(bd[1]);
	fa_sum += bd[0].individual_force(bd[2]);
	cout<<"fa_sum: "; printv(fa_sum);
	vec fb_sum = bd[1].individual_force(bd[0]);
	fb_sum += bd[1].individual_force(bd[2]);
	cout<<"fb_sum: "; printv(fb_sum);
	vec fc_sum = bd[2].individual_force(bd[0]);
	fc_sum += bd[2].individual_force(bd[1]);
	cout<<"fc_sum: "; printv(fc_sum);

	cout<<"body 0: ";printb(bd[0]);
	cout<<"body 1: ";printb(bd[1]);
	cout<<"body 2: ";printb(bd[2]);
	n_body::work_in_time_step(bd,3,type_t(0.05));
	cout<<"Time step of 0.05"<<endl;
	cout<<"body 0: ";printb(bd[0]);
	cout<<"body 1: ";printb(bd[1]);
	cout<<"body 2: ";printb(bd[2]);


	delete[] bd;



}
