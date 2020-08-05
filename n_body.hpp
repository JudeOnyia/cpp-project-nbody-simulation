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

			// Default constructor
			body() : mass(0),position(0,0),velocity(0,0),acceleration(0,0),collision(false) {}

			// Move constructor
			body(body&& other) : mass(other.mass),position(other.position),velocity(other.velocity),acceleration(other.acceleration),collision(false) {}
			// Move assignment
			body& operator=(body&& other){ mass=other.mass; position=other.position; velocity=other.velocity; acceleration=other.acceleration; return *this; }
			
			// Copy constructor
			body(const body& other) : mass(other.mass),position(other.position),velocity(other.velocity),acceleration(other.acceleration),collision(false) {}
			
			// Copy assignment
			body& operator=(const body& other) { mass=other.mass; position=other.position; velocity=other.velocity; acceleration=other.acceleration; return *this; }

			// Constructor to initialize state of body
			body(T m,vec p,vec v,vec a) : mass(m),position(p),velocity(v),acceleration(a),collision(false) {}	
			
			// Memeber function to calculate the oriented individual force
			// that a body (other) has on another body (*this) 
			vec individual_force(body& other) {
				vec r_dist = position - other.position;
				T r = r_dist.norm();

				/*// Condition for collision with other object
				if((radius>=(r-other.radius)) && (collision==false)){
					velocity = T(-1) * velocity;
					collision = true;
					other.collision = true;
				}
				else if((radius<(r-other.radius)) && (collision==true)){
					collision = false;
					other.collision = false;
				}
				else {}*/

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

			// Member function to calculate the data needed
			// to form the circular object in a window
			void form_circ_obj(T max_mass, T max_pos){
				if(mass < (T(0.25)*max_mass)){ radius = T(0.06)*max_pos; }
				else if(mass < (T(0.50)*max_mass)){ radius = T(0.09)*max_pos; }
				else if(mass < (T(0.75)*max_mass)){ radius = T(0.12)*max_pos; }
				else { radius = T(0.15)*max_pos; }
	                        // Make circular object
				T theta(0);
				for(std::size_t i=0; i<20; ++i){
					theta = T(i)*T(2.0)*T(3.1415926)/T(20);
					vertex[i].x = radius * std::cos(theta);
					vertex[i].y = radius * std::sin(theta);
				}
			}
			
		
			static T G; // Gravitational constant
			T mass;
			vec position;
			vec velocity;
			vec acceleration;
			vec vertex[20]; // Needed to made circle primitive quickly
			T radius; // Needed to compare for deflection conditions
			bool collision;
	};

	template<class T>
	T body<T>::G = T(6.67408E-11);

	// Function to help calculate final velocities under elastic collision rules
	template<class T>
	void elastic_collision(body<T>& A, body<T>& B){
		using vec = vector<T>;
		T mA = A.mass;
		T mB = B.mass;
		vec vAi = A.velocity;
		vec vBi = B.velocity;
		vec vBf = ((T(2)*mA*vAi)+((mB-mA)*vBi)) * (T(1)/(mA+mB));
		vec vAf = vBf + vBi - vAi;
		vec distAB = B.position - A.position;
		vec distBA = A.position - B.position;
		if( (distAB.x * vAi.x) < T(0) ){ vAf.x = -(vAf.x); vBf.x = -(vBf.x); }
		if( (distAB.y * vAi.y) < T(0) ){ vAf.y = -(vAf.y); vBf.y = -(vBf.y); }	
		//if(mA <= mB){ vAf.x = -(vAf.x); }
		//else { vBf.x = -(vBf.x); }
		A.velocity = vAf;
		B.velocity = vBf;
	}

	// Function to help calculate total net forces on each body
	// with an efficient method that avoids redundant calculations
	// by using Newton's 3rd law of equal and opposite reactions.
	// Then, updating the motion of the bodies accordingly
	template<class T>
	void work_in_time_step(body<T>* bd, std::size_t N, T time_step, T obj_space_range){
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
				// Condition for when bodies hit each other
				if((bd[i].radius)>=(((bd[i].position-bd[j].position).norm())-(bd[j].radius))){
					elastic_collision(bd[i],bd[j]);
				}
			}
			bd[i].update_state(summed_forces,time_step);

			// Condition for when a body hits an edge of the window
			if(((bd[i].position.x-bd[i].radius)<=(-obj_space_range)) || ((bd[i].position.x+bd[i].radius)>=(obj_space_range))){
				bd[i].velocity.x *= T(-1);
			}
			if(((bd[i].position.y-bd[i].radius)<=(-obj_space_range)) || ((bd[i].position.y+bd[i].radius)>=(obj_space_range))){
				bd[i].velocity.y *= T(-1);
			}


		}

		delete[] reactions;
	}

	// A dummy class to ensure that memory is deallocated when the program is forced to terminate
	template<class T>
	class system_of_bodies{
		public:
		system_of_bodies(std::size_t num_of_bodies){
			bodies = new body<T>[num_of_bodies]; // Create an array of type body
		}
		~system_of_bodies(){
			delete[] bodies;
		}
		system_of_bodies() = delete;
		system_of_bodies(system_of_bodies&&) = delete;
		system_of_bodies(const system_of_bodies&) = delete;
		system_of_bodies& operator=(system_of_bodies&&) = delete;
		system_of_bodies& operator=(const system_of_bodies&) = delete;
		body<T>* get_bodies() const { return bodies; }

		private:
		body<T>* bodies;

		
	};

	/*class clean_up{
		public:
			clean_up(body<)
	};*/




}
#endif
