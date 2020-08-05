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
void timer(int);
void printv(vec v);
void printb(body b);

// Global variables needed by functions from the GLUT library
std::size_t num_of_bodies;
body* bodies;
const std::size_t max_num_of_bodies = 10; // maximum number of bodies in the system
//body bodies[max_num_of_bodies];
type_t max_pos(0); // The maximum position value amongst all objects in the system
type_t obj_space_range(0); // The half of the length and width of the object space

int main(int argc, char** argv){
	// Read in the gravitational constant if one is specified
	type_t g(0);
	cin>>g;
	if(cin.fail()){
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		throw std::runtime_error("Wrong input entered: gravitional constant");
	}
	if(g != type_t(0)){ body::G = g; }

	// Read in the number of bodies
	cin>>num_of_bodies;
	if(cin.fail() || (num_of_bodies > max_num_of_bodies)){
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
		throw std::runtime_error("Wrong input entered: number of bodies");
	}
	n_body::system_of_bodies<type_t> syst(num_of_bodies);
	bodies = syst.get_bodies();
	//bodies = new body[num_of_bodies]; // Create an array of type body
	type_t max_mass(0); // The maximum mass needed for regulating the radius of objects(bodies)	

	// Read in the state of each body.
	// Due to the strict input format that must be followed,
	// If a wrong input is given, the program must be terminated
	// rather than continuing to take inputs that won't be assigned to the
	// intended variable
	for(std::size_t i=0; i<num_of_bodies; ++i){
		try{
			cin>>bodies[i].mass; if(cin.fail()){ throw std::runtime_error(""); }
			if((bodies[i].mass)>(max_mass)){ max_mass = bodies[i].mass; }
			cin>>bodies[i].position.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].position.y; if(cin.fail()){ throw std::runtime_error(""); }
			if(std::abs(bodies[i].position.norm())>(max_pos)){ max_pos = std::abs(bodies[i].position.norm()); }
			cin>>bodies[i].velocity.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].velocity.y; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].acceleration.x; if(cin.fail()){ throw std::runtime_error(""); }
			cin>>bodies[i].acceleration.y; if(cin.fail()){ throw std::runtime_error(""); }
		} catch(...){
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
			//delete[] bodies;
			throw std::runtime_error("Wrong input entered: initial state of one or more bodies");
		}
	}

	// Create vertices for circular objects that correspond to
	// the bodies
	for(std::size_t i=0; i<num_of_bodies; ++i){
		bodies[i].form_circ_obj(max_mass,max_pos);
	}
	obj_space_range = max_pos + (0.25 * max_pos);

	// Circular objects should not overlap at the beginning of the program.
	// This is a critical error that will result in undefined behaviour.
	// Also, the initial velocity of a bodies must have a limit compared to the system's object space,
	// otherwise, the movements of circular objects will be relatively too fast to visually analyze.
	for(std::size_t i=0; i<num_of_bodies; ++i){
		if(((bodies[i].velocity.x) > (type_t(0.001)*max_pos)) || ((bodies[i].velocity.y) > (type_t(0.001)*max_pos))){
			throw std::runtime_error("Velocity of one or more bodies is too high compared to object space for visualization");
		}
		for(std::size_t j=(i+1); j<num_of_bodies; ++j){
			if((bodies[i].radius)>=(((bodies[i].position-bodies[j].position).norm())-(bodies[j].radius))){
				throw std::runtime_error("Circular objects overlapping. Allow more distance between initial positions of bodies");
			}
		}
	}

	// Animation and update of states
	glutInit(&argc,argv); // Initialzation needed by the GLUT library
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode
	glutInitWindowPosition(200,100); // Set window position
	glutInitWindowSize(500,500); // Set window size
	glutCreateWindow("N-body Problem"); // Create a window
	glutDisplayFunc(display); // Tell the GLUT library what display function to use
	glutReshapeFunc(reshape); // Tell the GLUT library to reshape the window according to the reshap function
	glutTimerFunc(0,timer,0); // Tell the GLUT library what function to call at a specified time period
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
	gluOrtho2D(-obj_space_range,obj_space_range,-obj_space_range,obj_space_range);//To specify 2D projection (the object space)
	glMatrixMode(GL_MODELVIEW); // Need to always be in model view matrix, need to be here also to draw stuff
}

void timer(int){
	glutPostRedisplay(); // urges OpenGL to call the display function again when it can
	glutTimerFunc(1000/30,timer,0); //(ms,func,arg) Recursively calling itself, makes this periodic, 20 frames/sec (was 60)
	type_t time_step = 20; // Was 20
	//for(std::size_t i=0; i<100; ++i){
		work_in_time_step(bodies, num_of_bodies, time_step, obj_space_range);
	//}
}



void printv(vec v){ cout<<"("<<(v.x)<<", "<<(v.y)<<")"<<endl; }
void printb(body b){
	cout<<endl;
	cout<<"   mass: "<<(b.mass)<<endl;
	cout<<"   position: "; printv(b.position);
	cout<<"   velocity: "; printv(b.velocity);
	cout<<"   acceleration: "; printv(b.acceleration);
}
