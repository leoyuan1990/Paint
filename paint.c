#include <stdlib.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <math.h>
#include <iostream>
#include <random>

int shape = 0;                      // ID of shape being drawn
int colour = 1;                     // ID of colour being drawn
int pSize = 1;                      // size of points drawn
int counter = 0;					// draws a line, rectangle or circle on every even click
const float PI = 3.141592653;
const float RADIAL_ANGLE = 5.0;		// angle of rotations for radial paint brush
const int NUM_RADIAL = 72;			// number of rotations for radial paint brush

// the first point when drawing a line, rectangle or circle
int waitX = 0;
int waitY = 0;

// defining size of window and initial position in pixels
const int SIZE_X = 400;
const int SIZE_Y = 400;
const int POS_X = 200;
const int POS_Y = 200;

int makeRandom = 0;		// flag for whether to have random colours (odd is yes, even is no)

float c[3] =  {1.0, 1.0, 1.0};	    // colour vector being drawn

float toRad(float a) {				// converts from degrees to radians
	return a * PI/180.0;
}

// transforming cartesian coordinates into relative coordinates
float transformX(int x) {
	return (2.0*float(x)/SIZE_X - 1);
}

float transformY(int y) {
	return (-2.0*float(y)/SIZE_Y + 1);
}

// distance function for drawing a circle
float distance(float x1, float y1, float x2, float y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void swap(float *x1, float *x2) {	// bubble swap used for reversing coordinates
	float temp = *x2;
	*x2 = *x1;
	*x1 = temp;
}

void drawPoint(float x, float y, float c[3]) {
    glBegin(GL_POINTS);
        glColor3fv(c);
        glVertex2f(x, y);
    glEnd();
}

void drawLine(int inX1, int inY1, int inX2, int inY2, float c[3]) {
		
	// transform input cartesian coordinates into relative coordinates	
	float x1 = transformX(inX1);
	float y1 = transformY(inY1);
	float x2 = transformX(inX2);
	float y2 = transformY(inY2);
	
	bool swapped = false;	// flag for whether a swap was made (need to swap points back to reuse them)
	
	// swap the points if the x coordinate of the first point is greater than that of the second point
	if (x1 > x2) {
		swap(&x1, &x2);
		swap(&y1, &y2);
		swapped = true;
	}
	
	float dx = x2 - x1;
	float dy = y2 - y1;
	float d = 2.0*dy - dx;
	float incrE = 2.0*dy;
	float incrNE = 2.0*dy - 2.0*dx;

    // initialize and get slope
	float x = x1;
	float y = y1;
	float m = 0.0;
	if (x2 != x1) {
        m = (y2 - y1)/(x2 - x1);      // slope if not vertical
    }
	drawPoint(x, y, c);
	
    if (x1 == x2) {		// vertical line
		if (y1 > y2) {
			while (y > y2) {
				y -= 2.0/SIZE_Y;
				drawPoint(x, y, c);
			}
		}
		else {
			while (y < y2) {
				y += 2.0/SIZE_Y;
				drawPoint(x, y, c);
			}
		}
	}
	else if (m == 0) {	// horizontal line
		if (x2 > x1) {
			while (x < x2) {
				x += 2.0/SIZE_X;
				drawPoint(x, y, c);
			}
		}
		else {
			while (x > x2) {
				x -= 2.0/SIZE_X;
				drawPoint(x, y, c);
			}
		}
	} 	
	else if (m == 1) {	// positive slope diagonal line
		if (x2 > x1) {
			while (x < x2) {
				x += 2.0/SIZE_X;
				y += 2.0/SIZE_Y;
				drawPoint(x, y, c);
			}
		}
		else {
			while (x > x2) {
				x -= 2.0/SIZE_X;
				y -= 2.0/SIZE_Y;
				drawPoint(x, y, c);
			}
		}
	}  
	else if (m == -1) {	// negative slope diagonal line
		if (x2 > x1) {
			while (x < x2) {
				x += 2.0/SIZE_X;
				y -= 2.0/SIZE_Y;
				drawPoint(x, y, c);
			}
		}
		else {
			while (x > x2) {
				x -= 2.0/SIZE_X;
				y += 2.0/SIZE_Y;
				drawPoint(x, y, c);
			}
		}
	}	
	else if (fabs(m) < 1) {		// absolute value slope < 1
		if (dy >= 0) {			// checks if y is increasing or decreasing
			while (x < x2) {
				x += 2.0/SIZE_X;
				if (d < 0) {
					d += incrE;
				}
				else {
					y += 2.0/SIZE_Y;
					d += incrNE;
				}
				drawPoint(x, y, c);
			}
		} else {
			dy *= -1.0;
			incrE = 2.0*dy;
			incrNE = 2.0*dy - 2.0*dx;
			while (x < x2) {
				x += 2.0/SIZE_X;
				if (d < 0) {
					d += incrE;
				}
				else {
					y -= 2.0/SIZE_Y;
					d += incrNE;
				}
				drawPoint(x, y, c);
			}
		}
    }   
	else if (fabs(m) > 1) {   // absolute value slope > 1
		if (dy >= 0) {
			d = 2.0*dx - dy;
			incrE = 2.0*dx;
			incrNE = 2.0*dx - 2.0*dy;
			while (y < y2) {
				y += 2.0/SIZE_Y;
				if (d <= 0) {
					d += incrE;
				} else {
					d += incrNE;
					x += 2.0/SIZE_X;
				}
				drawPoint(x, y, c);
			}
		} else {
			dy *= -1.0;
			d = 2.0*dx - dy;
			incrE = 2.0*dx;
			incrNE = 2.0*dx - 2.0*dy;
			while (y > y2) {
				y -= 2.0/SIZE_Y;
				if (d <= 0) {
					d += incrE;
				} else {
					d += incrNE;
					x += 2.0/SIZE_X;
				}
				drawPoint(x, y, c);
			}
		}	
    }  

	// swaps points back to original x and y values so they may be reused
	if (swapped) {
		swap(&x1, &x2);
		swap(&y1, &y2);
	}	
}

void drawRect(int inX1, int inY1, int inX2, int inY2, float c[3]) {
	
	int y = inY1;
	if (inY1 < inY2) {
		while (y <= inY2) {
			drawLine(inX1, y, inX2, y, c);
			y++;
		}
	}
	else {
		while (y >= inY2) {
			drawLine(inX1, y, inX2, y, c);
			y--;
		}
	}
}

void drawCircle(int inX1, int inY1, int inX2, int inY2, float c[3]) {
	float x1 = transformX(inX1);
	float y1 = transformY(inY1);
	float x2 = transformX(inX2);
	float y2 = transformY(inY2);
	float r = distance(x1, y1, x2, y2);
	float x = x1 - r;
	float y = y1 - r;
	
	while(x <= x1 + r) {
		while (y <= y1 + r) {
			if (distance(x1, y1, x, y) <= r) {
				drawPoint(x, y, c);
			}
			y += 2.0/SIZE_Y;
		}
		x += 2.0/SIZE_X;
		y = y1 - r;
	}
}

void mouse(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON and state == GLUT_UP and shape == 1){
		drawPoint(transformX(x), transformY(y), c);
	}
	else if (btn == GLUT_LEFT_BUTTON and state == GLUT_UP and shape == 2) {
		counter++;
		if (counter % 2 == 1) {				// stores the first point
			waitX = x;
			waitY = y;
		}
		else {								// draws the line on an even number click
			drawLine(waitX, waitY, x, y, c);
		}
	}
	else if (btn == GLUT_LEFT_BUTTON and state == GLUT_UP and shape == 3) {
		counter++;
		if (counter % 2 == 1) {				// stores the first point
			waitX = x;
			waitY = y;
		}
		else {								// draws the line on an even number click
			drawRect(waitX, waitY, x, y, c);
		}
	}
	else if (btn == GLUT_LEFT_BUTTON and state == GLUT_UP and shape == 4) {
		counter++;
		if (counter % 2 == 1) {				// stores the first point as the center of the circle
			waitX = x;
			waitY = y;
		}
		else {								// draws the circle on an even number click
			drawCircle(waitX, waitY, x, y, c);
		}
	}
	else if (btn == GLUT_LEFT_BUTTON and state == GLUT_UP and shape == 5) {
		glPushMatrix();
		for (int i = 0; i < NUM_RADIAL; i++) {
			drawPoint(transformX(x), transformY(y), c);
			glRotatef(RADIAL_ANGLE, 0.0, 0.0, 1.0);			// rotates around the z axis
		}
		glPopMatrix();	// erases the rotations so that the next click draws from that click
	}
}

void mainMenu(int click) {
	if (click == 3) {	// turn on or off random colours when "Random Colours" is clicked
		makeRandom++;
		if (makeRandom % 2 == 1) {
			printf("RANDOM COLOURS ON. COLOUR MENU DISABLED.\n");
		} else {
			printf("RANDOM COLOURS OFF. COLOUR MENU ENABLED. SETTING COLOUR TO WHITE.\n");
			c[0] = 1.0;
			c[1] = 1.0;
			c[2] = 1.0;
		}
	}
	else if (click == 4) {	// clear the screen when "Clear" is clicked
		glClear(GL_COLOR_BUFFER_BIT);
		printf("CLEARING SCREEN (KEEP CLICKING THIS IF THE BROKEN DOUBLE BUFFER IS CAUSING TROUBLE).\n");
	}
    else if (click == 5) {	// exits the program when "Quit" is clicked
		exit(0);
	}   
}

void colourMenu(int click) {
    if (click == 1) {			// red
		c[0] = 1.0;
		c[1] = 0.0;
		c[2] = 0.0;
		if (makeRandom % 2 == 0) {
			printf("RED SELECTED.\n");
		}
	}
	else if (click == 2) {		// blue
		c[0] = 0.0;
		c[1] = 1.0;
		c[2] = 0.0;
		if (makeRandom % 2 == 0) {
			printf("GREEN SELECTED.\n");
		}
	}
	else if (click == 3) {		// green
		c[0] = 0.0;
		c[1] = 0.0;
		c[2] = 1.0;
		if (makeRandom % 2 == 0) {
			printf("BLUE SELECTED.\n");
		}
	}
	else if (click == 4) {		// purple
		c[0] = 0.5;
		c[1] = 0.0;
		c[2] = 1.0;
		if (makeRandom % 2 == 0) {
			printf("PURPLE SELECTED.\n");
		}
	}
	else if (click == 5) {		// yellow
		c[0] = 1.0;
		c[1] = 1.0;
		c[2] = 0.0;
		if (makeRandom % 2 == 0) {
			printf("YELLOW SELECTED.\n");
		}
	}
	if (makeRandom % 2 == 1) {
			printf("COLOUR MENU DISABLED. CLICK RANDOM COLOURS IN MAIN MENU TO RE-ENABLE.\n");
	}
}

void shapeMenu(int click) {		// selects the shape to be drawn from the Shape menu
    shape = click;
	if (click == 1) {
		printf("DRAWING POINTS.\n");
	}
	else if (click == 2) {
		printf("DRAWING LINES.\n");
	}
	else if (click == 3) {
		printf("DRAWING RECTANGLES.\n");
	}
	else if (click == 4) {
		printf("DRAWING CIRCLES.\n");
	}
	else if (click == 5) {
		printf("DRAWING RADIAL POINTS.\n");
	}
}

void makeMenus(void) {			// creates all menus and submenus
    int colour_menu_id = glutCreateMenu(colourMenu);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Purple", 4);
    glutAddMenuEntry("Yellow", 5);

    int shape_menu_id = glutCreateMenu(shapeMenu);
    glutAddMenuEntry("Point", 1);
    glutAddMenuEntry("Line", 2);
    glutAddMenuEntry("Rectangle", 3);
    glutAddMenuEntry("Circle", 4);
	glutAddMenuEntry("Radial Paint Brush", 5);

    int main_menu_id = glutCreateMenu(mainMenu);
    glutAddSubMenu("Colour", colour_menu_id);
    glutAddSubMenu("Shape", shape_menu_id);
	glutAddMenuEntry("Random Colours", 3);
	glutAddMenuEntry("Clear", 4);
    glutAddMenuEntry("Quit", 5);

    glutAttachMenu(GLUT_RIGHT_BUTTON);		// right click to bring up the main menu
}

void FPS(int val) {
	glutTimerFunc(17, FPS, 0);				// 60 FPS 
}

void callBackInit() {
	glutMouseFunc(mouse);
	glutTimerFunc(0, FPS, 0);
}

void display(void) {
	glPointSize(pSize);
	if (makeRandom % 2 == 1) {		// change colours for each new shape drawn if random colours are on
		c[0] = float(rand())/RAND_MAX;
		c[1] = float(rand())/RAND_MAX;
		c[2] = float(rand())/RAND_MAX;
	}
	glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(SIZE_X, SIZE_Y);
    glutInitWindowPosition(POS_X, POS_Y);

	glutCreateWindow("Paint");
	glClear(GL_COLOR_BUFFER_BIT);
	printf("\n\n\nRight click to bring up the menu.\n");
	printf("To begin, select a shape from the Shape menu.\n");
	printf("The initial colour is white but five preset colours may be selected from the Colour menu.\n");
	printf("Or click Random Colours to allow each new shape drawn to have a random colour.\n");
	printf("Doing so disables the Colour menu. You must click Random Colours again to re-able the Colour menu.\n");
	printf("You can also reset the colour to white by disabling and re-enabling the colour menu.\n");
	printf("Radial Paint Brush draws points that are equidistant from the origin and equally spaced out.\n");
	printf("Click Clear to clear the screen. (sort of broken and requires multiple clears cause of broken double buffer.\n");
	printf("Click Quit to exit.\n\n");
	callBackInit();
	glutDisplayFunc(display);
	makeMenus();
	glutMainLoop();

	return 0;
}
