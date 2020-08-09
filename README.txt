This project demonstrates an estimate of the predicted movements of objects
with masses (referred to as bodies) as each is subjected to the gravitational
force of all others and bound within a confined space.

Let $TOP_DIR denote the directory containing this README file.
Let $INSTALL_DIR denote the directory into which this software is to be installed.

To build and install the software, use the commands:
	cd $TOP_DIR
	cmake -H. -Btmp_cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
	cmake --build tmp_cmake --clean-first --target install

To run a demonstration, use the commands:
	$INSTALL_DIR/bin/demo

Information to be aware of when using this software and updates to the project
specifications since the project proposal can be found below:

1)	The input format requried by the program has been modified to accept
	the following inputs in this order:
	i)	A user defined gravitational constant. If the user wishes to
		use the actual gravitaional constant, the user can supply a
		value of 0 for this input, the program will default to the
		actual constant.

	ii)	The time step (in seconds) used by the program to update the
		states of each body. The user is allowed to provide this value
		according to their visual needs, however, the larger this value
		is, the more error is accumulated in the program's estimation.

	iii)	The number of bodies in the user's arbitrary system. This has a
		maximum value of 20 bodies. If the number read is higher than 20,
		the program will terminate with a runtime error.

	iV)	The mass, the initial position's x and y components, the initial
		velocity's x and y components, the initial acceleration's x and y
		components of the ith body, as i=1,2,..,number of bodies. An example
		of this setup is available in the "initial_state.txt" file located in
		the same directory as this README file.

	The initial positions of bodies must be well spread out to avoid overlap when
	their circumference is calculated initally. If an overlap is found in the
	initial positions, the program terminates with a runtime error. If a body's
	initial velocity is too high compared to the system's arbitrary confined space,
	this is visually undesirable and the program will terminate with a runtime error.

2)	In the case of a collision, this is handled by the mathematics involved in
	elastic collision, which assumes all collisions are head-on, the angle of impact
	is handled by comparing the direction of a body's velocity to the direction of
	the distance to the other body involved. This reduces complexity, which is
	beneficial for the fast computation required between each frame in the animation.

3)	The calculation of the net gravitational force on each body is performed with an
	efficient method that avoids redundant calculations by using Newton's 3rd law of
	equal and opposite reactions.
