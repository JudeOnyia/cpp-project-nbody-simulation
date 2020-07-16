#include "n_body.hpp"
#include<iostream>
#include<cstddef>
#include<limits>
#include<cmath>
#include<stdexcept>
#include<GL/glut.h>
using std::cout;
using std::endl;
using std::cin;
using type_t = long double;
using vec = n_body::vector<type_t>;
using body = n_body::body<type_t>;

// Function declarations
void display();
void init();
void reshape(int,int);
void printv(vec v);
void printb(body b);

// Global variables needed by functions from the GLUT library
std::size_t num_of_bodies;
body* bodies;
type_t max_pos_range(0); // The maximum range of positions needed for reshaping the object space

int main(int argc, char** argv){

	// Read in the number of bodies
	cin>>num_of_bodies;
	if(cin.fail()){
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		throw std::runtime_error("Wrong input entered");
	}
	n_body::system_of_bodies<type_t> syst(num_of_bodies);
	bodies = syst.get_bodies();
	//bodies = new body[num_of_bodies]; // Create an array of type body
	type_t max_mass(0); // The maximum mass needed for regulating the radius of objects(bodies)	

	// Read in the state of each body.
	// Due to the strict input format that must be followed,
	// If a wrong input is given, the program must be terminated
	// rather than continuing to take inputs that won't assigned to the
	// intended variable
	for(std::size_t i=0; i<num_of_bodies; ++i){
		try{
			cin>>bodies[i].mass; if(cin.fail()){ throw std::runtime_error(""); }
			if((bodies[i].mass)>(max_mass)){ max_mass = bodies[i].mass; }
			cin>>bodies[i].position.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].position.y; if(cin.fail()){ throw std::runtime_error(""); }
			if(std::abs(bodies[i].position.norm())>(max_pos_range)){ max_pos_range = std::abs(bodies[i].position.norm()); }
			cin>>bodies[i].velocity.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].velocity.y; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].acceleration.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].acceleration.y; if(cin.fail()){ throw std::runtime_error(""); }
		} catch(...){
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
			//delete[] bodies;
			throw std::runtime_error("Wrong input entered");
		}
	}

	// Create vertices for circular objects that correspond to
	// the bodies
	for(std::size_t i=0; i<num_of_bodies; ++i){
		bodies[i].form_circ_obj(max_mass,max_pos_range);
	}

	// Animation and update of states
	glutInit(&argc,argv); // Initialzation needed by the GLUT library
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode
	glutInitWindowPosition(200,100); // Set window position
	glutInitWindowSize(500,500); // Set window size
	glutCreateWindow("N-body Problem"); // Create a window
	glutDisplayFunc(display); // Tell the GLUT library what display function to use
	glutReshapeFunc(reshape); // Tell the GLUT library to reshape the window according to the reshap function
	init(); // Initial setting of the window features
	//glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutMainLoop();	


	




	




	//delete[] bodies;
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	// Display circular objects
	for(std::size_t i=0; i<num_of_bodies; ++i){
		glLoadIdentity(); // Resets the pixel position matrix
		glTranslatef(bodies[i].position.x,bodies[i].position.y,0.0); // Translate the circular object to its position
		glBegin(GL_POLYGON); // Begin drawing circular object
		glColor3f(0.0,0.0,1.0); // Color object
		for(std::size_t j=0; j<20; ++j){
			glVertex2f(bodies[i].vertex[j].x,bodies[i].vertex[j].y);
		}
		glEnd();
	}

	glutSwapBuffers(); // After drawing all circular objects, switch the drawing frame with the display frame
	
}


void init(){
	glClearColor(0.0,0.0,0.0,1.0); // Clear the background of the window to black
}


void reshape(int w,int h){
	glViewport(0,0,(GLsizei)w,(GLsizei)h); // Specify viewport
	glMatrixMode(GL_PROJECTION); // Switch to different matrix from GL_MODELVIEW
	glLoadIdentity();// Resets the pixel position matrix
	gluOrtho2D(-max_pos_range,max_pos_range,-max_pos_range,max_pos_range); // To specify 2D projection (the object space)
	glMatrixMode(GL_MODELVIEW); // Need to always be in model view matrix, need to be here also to draw stuff
}



void printv(vec v){ cout<<"("<<(v.x)<<", "<<(v.y)<<")"<<endl; }
void printb(body b){
	cout<<endl;
	cout<<"   mass: "<<(b.mass)<<endl;
	cout<<"   position: "; printv(b.position);
	cout<<"   velocity: "; printv(b.velocity);
	cout<<"   acceleration: "; printv(b.acceleration);
}
