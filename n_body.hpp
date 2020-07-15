#ifndef NBODYHPP
#define NBODYHPP
#include<cmath>

namespace n_body{

	template<class T>
	struct vector{
		vector() : x_(0), y_(0) {} // Default constructor
		vector(T x, T y) : x_(x), y_(y) {} // Constructor with input parameters
		vector(vector&& other) : x_(other.x_), y_(other.y_) {} // Move constructor
		vector& operator=(vector&& other) : x_(other.x_), y_(other.y_) { return *this; } // Move assignment
		vector(const vector& other) : x_(other.x_), y_(other.y_) {} // Copy constructor
		vector& operator=(const vector& other) : x_(other.x_), y_(other.y_) { return *this;} // Copy assignment
		
		// Operator for compound addition (+=) and binary addition
		vector& operator+=(const vector& obj){ x_ += obj.x_; y_ += obj.y_; return *this; }
		vector operator+(const vector& obj){ return vector((x_ + obj.x_),(y_ + obj.y_)); }

		// Operator for compound subtraction (-=) and binary subtraction
		vector& operator-=(const vector& obj){ x_ -= obj.x_; y_ -= obj.y_; return *this; }
		vector operator-(const vector& obj){ return vector((x_ - obj.x_),(y_ - obj.y_)); }

		// Operator for compound multiplication (*=) and binary multiplication
		vector& operator*=(const vector& obj){ x_ *= obj.x_; y_ *= obj.y_; return *this; }
		vector operator*(const vector& obj){ return vector((x_ * obj.x_),(y_ * obj.y_)); }

		bool operator!() const { return (x_==T(0) && y_==T(0)); } // Operator to check if vector is zero vector

		bool operator==(const vector& obj) const { return(x_==obj.x_ && y_==obj.y_); } // Operator to check equality

		bool operator!=(const vector& obj) const { return(x_!=obj.x_ || y_!=obj.y_); } // Operator to check inequality

		T norm() const { return std::sqrt(x_*x_, y_*y_); }

		

		T x_;
		T y_;
	};





}
#endif