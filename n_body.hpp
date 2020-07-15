#ifndef NBODYHPP
#define NBODYHPP
#include<cstddef>
#include<cmath>
#include<iostream>

namespace n_body{

	// convenient x-y component class
	template<class T>
	struct vector{
		vector() : x(0), y(0) {} // Default constructor
		vector(T inx, T iny) : x(inx), y(iny) {} // Constructor with input parameters
		vector(vector&& other) : x(other.x), y(other.y) {} // Move constructor
		vector& operator=(vector&& other) { x=other.x; y=other.y; return *this; } // Move assignment
		vector(const vector& other) : x(other.x), y(other.y) {} // Copy constructor
		vector& operator=(const vector& other) { x=other.x; y=other.y; return *this; } // Copy assignment
		
		// Operator for compound addition (+=) and binary addition
		vector& operator+=(const vector& obj){ x += obj.x; y += obj.y; return *this; }
		vector operator+(const vector& obj) const { return vector((x + obj.x),(y + obj.y)); }

		// Operator for compound subtraction (-=) and binary subtraction
		vector& operator-=(const vector& obj){ x -= obj.x; y -= obj.y; return *this; }
		vector operator-(const vector& obj) const { return vector((x - obj.x),(y - obj.y)); }

		// Operator for compound multiplication (*=) and binary multiplication
		vector& operator*=(const vector& obj){ x *= obj.x; y *= obj.y; return *this; }
		vector operator*(const vector& obj) const { return vector((x * obj.x),(y * obj.y)); }

		bool operator!() const { return (x==T(0) && y==T(0)); } // Operator to check if vector is zero vector

		bool operator==(const vector& obj) const { return(x==obj.x && y==obj.y); } // Operator to check equality

		bool operator!=(const vector& obj) const { return(x!=obj.x || y!=obj.y); } // Operator to check inequality

		T norm() const { return std::sqrt(x*x + y*y); }

		T x;
		T y;
	};

	// Non-member class for multipling vector with constants
	template<class T>
	vector<T> operator*(const T& num,const vector<T>& obj) { return vector<T>((obj.x * num),(obj.y * num)); }
	template<class T>
	vector<T> operator*(const vector<T>& obj,const T& num) { return vector<T>((obj.x * num),(obj.y * num)); }

	// class for representing celestial bodies
	template<class T>
	struct body{
		
			using vec = vector<T>;
			const T G = T(6.67408E-11); // Gravitational constant

			// Default constructor
			body() : mass(0),position(0,0),velocity(0,0),acceleration(0,0) {}

			// Move constructor
			body(body&& other) : mass(other.mass),position(other.position),velocity(other.velocity),acceleration(other.acceleration) {}
			// Move assignment
			body& operator=(body&& other){ mass=other.mass; position=other.position; velocity=other.velocity; acceleration=other.acceleration; return *this; }
			
			// Copy constructor
			body(const body& other) : mass(other.mass),position(other.position),velocity(other.velocity),acceleration(other.acceleration) {}
			
			// Copy assignment
			body& operator=(const body& other) { mass=other.mass; position=other.position; velocity=other.velocity; acceleration=other.acceleration; return *this; }

			// Constructor to initialize state of body
			body(T m,vec p,vec v,vec a) : mass(m),position(p),velocity(v),acceleration(a) {}	
			
			// Memeber function to calculate the oriented individual force
			// that a body (other) has on another body (*this) 
			vec individual_force(const body& other) const {
				vec r_dist = position - other.position;
				T r = r_dist.norm();
				vec force;
				if(r != T(0)){
					force = (T(-1)*G*mass*other.mass/(r*r*r)) * r_dist;
				}
				return force;
			}

			// Member function to calculate the oriented distance
			// from a body (other) to another body (*this)
			vec distance(const body& other) const{
				return (position - other.position);
			}

			// Member function to update the state of a body
			void update_state(vec net_force, T time_step){
				position += (velocity * time_step);
				velocity += (acceleration * time_step);
				acceleration = (T(1) / mass) * net_force;
			}
		
			T mass;
			vec position;
			vec velocity;
			vec acceleration;
	};

	// Function to help calculate total net forces on each body
	// with an efficient method that avoids redundant calculations
	// by using Newton's 3rd law of equal and opposite reactions.
	// Then, updating the motion of the bodies accordingly
	template<class T>
	void work_in_time_step(body<T>* bd, std::size_t N, T time_step){
		using vec = vector<T>;
		vec* reactions = new vec[N];
		vec summed_forces;
		vec force_i_j;
		for(std::size_t i=0; i<N; ++i){
			summed_forces = (reactions[i]) * T(-1);
			for(std::size_t j=(i+1); j<N; ++j){
				force_i_j = bd[i].individual_force(bd[j]);
				reactions[j] += force_i_j;
				summed_forces += force_i_j;
			}
			bd[i].update_state(summed_forces,time_step);
			//std::cout<<"summed force: ("<<(summed_forces.x)<<" "<<(summed_forces.y)<<std::endl; // Replace with update_state function
		}

		delete[] reactions;
	}




}
#endif
