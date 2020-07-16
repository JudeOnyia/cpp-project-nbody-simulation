#include "n_body.hpp"
#include<iostream>
#include<cstddef>
#include<limits>
#include<stdexcept>
using std::cout;
using std::endl;
using std::cin;
using type_t = long double;
using vec = n_body::vector<type_t>;
using body = n_body::body<type_t>;

void printv(vec v);
void printb(body b);

int main(){
	// Read in the number of bodies
	std::size_t num_of_bodies
	cin>>num_of_bodies;
	if(cin.fail()){
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(),’\n’);
		throw std::runtime_error("Wrong input entered");
	}

	body* bodies = new body[num_of_bodies]; // Create an array of type body

	// Read in the state of each body.
	// Due to the strict input format that must be followed,
	// If a wrong input is given, the program must be terminated.
	for(std::size_t i=0; i<num_of_bodies; ++i){
		try{
			cin>>bodies[i].mass; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].position.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].position.y; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].velocity.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].velocity.y; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].acceleration.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].acceleration.y; if(cin.fail()){ throw std::runtime_error(""); }
		} catch(...){
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),’\n’);
			delete[] bodies;
			throw std::runtime_error("Wrong input entered");
		}
	}

	

	for(std::size_t i=0; i<num_of_bodies; ++i){
		cout<<"bodies["<<i<<"]: "; printb(bodies[i]);
	}




	delete[] bodies;
}

void printv(vec v){ cout<<"("<<(v.x)<<", "<<(v.y)<<")"<<endl; }
void printb(body b){
	cout<<endl;
	cout<<"   mass: "<<(b.mass)<<endl;
	cout<<"   position: "; printv(b.position);
	cout<<"   velocity: "; printv(b.velocity);
	cout<<"   acceleration: "; printv(b.acceleration);
}