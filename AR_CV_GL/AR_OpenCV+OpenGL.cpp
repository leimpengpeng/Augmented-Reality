// AR - OpenCV + OpenGL

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include "glew.h"
#include "gl/glut.h"

#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

void drawObject(void);
void renderBitmapString(float x, float y, float z, void *font, char *string);

bool bFullsreen = false;
int	windowID;

// Frame counter variables
char pixelstring[30];
int cframe = 0;
int times = 0;
int timebase = 0;

// Video capture variables
VideoCapture capture(0);
Mat frame, gray;
GLuint cameraImageTextureID;

Size markerSize(5,4);
vector<Point2f> corners;
//Mat persp;
Mat cameraMatrix, dist;
Mat rvec = Mat::zeros( 3, 1, CV_32FC1 );
Mat tvec = Mat::zeros( 3, 1, CV_32FC1 );
Mat rmat = Mat::zeros( 3, 3, CV_32FC1 );

float fx,fy,cx,cy;

//----------------------------------------------------------------------------------------------------

void displayFunc(void)
{
	capture >> frame;
	flip(frame,frame,0);

	cvtColor(frame, gray, CV_BGR2GRAY);
	bool found = findChessboardCorners(gray, markerSize, corners, CV_CALIB_CB_ADAPTIVE_THRESH |CALIB_CB_FAST_CHECK);
	drawChessboardCorners(frame,markerSize,corners,found);

	if( found ) 
	{
		vector<Point3f> q;  // Coordinates of chessboard corner point in 3d
		vector<Point2f> p;	// Corresponding chessboard corner point obtain from the captured image 

		cornerSubPix(gray, corners, Size(11,11),cvSize(-1,-1), cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
				
		q.push_back(Point3f(-2, -1.5, 0));
		q.push_back(Point3f(2, -1.5, 0));
		q.push_back(Point3f(2, 1.5, 0));
		q.push_back(Point3f(-2, 1.5, 0));
		
		p.push_back(Point2f(corners[0].x, corners[0].y));
		p.push_back(Point2f(corners[4].x, corners[4].y));
		p.push_back(Point2f(corners[19].x, corners[19].y));
		p.push_back(Point2f(corners[15].x, corners[15].y));

		solvePnP(q,p,cameraMatrix,dist,rvec,tvec,false);
		Rodrigues(rvec,rmat);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_RECTANGLE_ARB);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLdouble)frame.cols,0.0,(GLdouble)frame.rows);	
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cameraImageTextureID);

	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		
	glBegin(GL_QUADS);
		glTexCoord2i(0,0);						glVertex2i(0,0);
		glTexCoord2i(frame.cols,0);				glVertex2i(frame.cols,0);
		glTexCoord2i(frame.cols,frame.rows);	glVertex2i(frame.cols,frame.rows);
		glTexCoord2i(0,frame.rows);				glVertex2i(0,frame.rows);
	glEnd();

	glDisable(GL_TEXTURE_RECTANGLE_ARB);

	// Count frames
	times=glutGet(GLUT_ELAPSED_TIME);
	cframe++;
	if (times - timebase > 50) {
		sprintf(pixelstring, "fps: %4.2f", cframe*1000.0/(times-timebase));		
		timebase = times;
		cframe = 0;
	}

	glDisable(GL_LIGHTING);
	glColor4f(1.0,1.0,1.0,1.0);

	renderBitmapString(5,5,0.0,GLUT_BITMAP_HELVETICA_10,pixelstring);
	
	if(found)	drawObject();

	glutSwapBuffers();
}
//----------------------------------------------------------------------------------------------------

void idleFunc(void) {
	glutPostRedisplay();
}
//----------------------------------------------------------------------------------------------------

void keyboardFunc(unsigned char key, int x, int y)
{
	switch(key) {
		case 27:
			glDeleteTextures(1, &cameraImageTextureID);
			exit(0);
			
		// Switch to fullscreen
		case 'f':
			bFullsreen = !bFullsreen;
			if(bFullsreen) 
				glutFullScreen();
			else {
				glutSetWindow(windowID);
				glutPositionWindow(100, 100);
				glutReshapeWindow(640, 480);
			}
			break;
	}
}
//----------------------------------------------------------------------------------------------------

void reshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
}
//----------------------------------------------------------------------------------------------------

void mouseFunc(int button, int state, int x, int y) {

}
//----------------------------------------------------------------------------------------------------

void mouseMotionFunc(int x, int y) {
	
}
//----------------------------------------------------------------------------------------------------

void specialFunc(int key, int x, int y) {

}
//----------------------------------------------------------------------------------------------------

void drawAxis()
{
	glLineWidth(1);

	glBegin(GL_LINES);
		glVertex3f( 0.0,  0.0,  0.0);		glVertex3f( 10.0,    0.0,   0.0); // x-axis
		glVertex3f( 0.0,  0.0,  0.0);		glVertex3f(   0.0,  10.0,   0.0); // y-axis
		glVertex3f( 0.0,  0.0, -10.0); 		glVertex3f(   0.0,   0.0,  10.0); // z-axis

		// arrow for x-axis
		glVertex3f( 9.0, 1.0, 0.0);		glVertex3f(10.0,  0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);		glVertex3f( 9.0, -1.0, 0.0); 

		// arrow for y-axis
		glVertex3f( 1.0,  9.0, 0.0);	glVertex3f( 0.0, 10.0, 0.0);
		glVertex3f( 0.0, 10.0, 0.0);	glVertex3f(-1.0,  9.0, 0.0);

		// arrow for z-axis
		glVertex3f( 0.0,  1.0,  9.0);	glVertex3f( 0.0,  0.0, 10.0);
		glVertex3f( 0.0,  0.0, 10.0);	glVertex3f( 0.0, -1.0,  9.0);
	glEnd();

	glRasterPos3i(9, 0, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'x');
	glRasterPos3i(0, 9, 0);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');
	glRasterPos3i(0, 0, 9);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'z');
}

void drawObject()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix(); 

	glFrustum(-1/fx*cx,1/fx*(640-cx),-1/fy*cy,1/fy*(480-cy),1,50);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0.0, 0.0, 0.0, 0.0, 0.0, -10.0, 0.0, 1.0, 0.0);

	glPushMatrix();

	double m[] = {rmat.at<double>(0,0),rmat.at<double>(1,0),-rmat.at<double>(2,0),0.0f,
		rmat.at<double>(0,1),rmat.at<double>(1,1),-rmat.at<double>(2,1),0.0f,
		rmat.at<double>(0,2),rmat.at<double>(1,2),-rmat.at<double>(2,2),0.0f,
		tvec.at<double>(0,0),tvec.at<double>(1,0),-tvec.at<double>(2,0),1.0f};
		
	glLoadMatrixd(m);
	glColor3f(0, 0, 1);
	drawAxis();
	glTranslatef(0,0,-1.5);
	glutSolidCube(3);
	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, float z, void *font, char *string) 
{
	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

//----------------------------------------------------------------------------------------------------

void init()
{
	if(!capture.isOpened()) // check if we succeeded
		exit(-1);

	FileStorage fs("camera.yml", CV_STORAGE_READ);
    if(!fs.isOpened())
    {
         printf("Failed to open file\n");
		 exit(-1);
    }
    
	// Load camera matrix
    fs["camera_matrix"] >> cameraMatrix;
	fs["distortion_coefficients"] >> dist;

	fx = cameraMatrix.at<double>(0,0);
	fy = cameraMatrix.at<double>(1,1);
	cx = cameraMatrix.at<double>(0,2);
	cy = cameraMatrix.at<double>(1,2);
	
	// Initialze OpenGL texture		
	glEnable(GL_TEXTURE_RECTANGLE_ARB);

	glGenTextures(1, &cameraImageTextureID);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, cameraImageTextureID);

	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void main(int argc, char **argv) {

	int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	glutInit (&argc, argv);
	glutInitWindowSize (width, height);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	windowID = glutCreateWindow ("OpenCV_OpenGL");

	init();

	glutDisplayFunc		(displayFunc);
	glutReshapeFunc		(reshapeFunc);
	glutKeyboardFunc	(keyboardFunc);
	//glutSpecialFunc	(specialFunc);
	//glutMouseFunc		(mouseFunc);
	//glutMotionFunc	(mouseMotionFunc);
	glutIdleFunc		(idleFunc);
	glutMainLoop();	
}





