#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#include "gsub.h"
#include "video.h"
#include "param.h"
#include "ar.h"

#include"obj_parse.h"



	//
// Camera configuration.
//
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

int             xsize, ysize;
int             thresh = 100;
int             count = 0;

char           *cparam_name    = "Data/camera_para.dat";
ARParam         cparam;

char           *patt_name      = "Data/patt.hiro";
int             patt_id;
double          patt_width     = 80.0;
double          patt_center[2] = {0.0, 0.0};
double          patt_trans[3][4];

static void   init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static void   draw( void );

int loadObject(const char* obj_filename)
{

int i;
int FirstFace=0;
int LastFace=0;
int FaceId=0;
int ObjId=0;
int curmat=0;
int k=0;
char line[256];
char *arg[8];
char buf[256];
for (i=0; i<8;i++){
	 arg[i]=(char*)malloc(256);
	 if (arg[i]==NULL)
	 {printf("ERROR mallocing arg\n");exit(1);}
}
std::ifstream objfile;
objfile.open(obj_filename);
//_____READING OBJFILE_____
 if(!objfile.is_open())return -1;
//char buf[256];	
/*while (!objfile.eof()){
	objfile.getline(buf,256);
	coord.push_back(new std::string(buf));
	}
	*/
while (!objfile.eof())
{
		objfile.getline(line,256);
	if(line[0]!='#'){ 
			for(i=0;i<8;i++)strcpy(arg[i],"");
				int num_args=sscanf(line,"%s%s%s%s%s%s%s%s",arg[0],arg[1],arg[2],arg[3],arg[4],arg[5],
							arg[6],arg[7]);	
			
	if(strcmp(arg[0],"g")==0){
		char obj_name[256];
			k++;
		sscanf(arg[1],"%s",obj_name);
		std::cout << obj_name << std::endl;
			std::cout << (k/2)  << std::endl;
	
	//	std::cout << std::endl; 
		

	}

	if(strcmp(arg[0],"v")==0){
			float x,y,z; 
			sscanf(arg[1],"%f",&x);
			sscanf(arg[2],"%f",&y);
			sscanf(arg[3],"%f",&z);
	 		vertex.push_back(new coordinate(x,-z,y));
			//std::cout << vertex.size() << std::endl;
		
		}
	if (strcmp(arg[0],"mtllib")==0){
			char buff[200];
			char mread[256];
			std::ifstream mtlfile;
			int t=0;

			char mtl_objname[200];	//name of the material
			char filename[200];	//filename of the texture
			float amb[3],dif[3],spec[3],alpha,ns,ni;	//colors, shininess, and something else
			int illum;
			unsigned int texture;
			sscanf (arg[1], "%s./", mtlFile);
			std::cout<< "mtllib" << ":" << mtlFile ;
			mtlfile.open(mtlFile);
		/*	while(!mtlfile.eof()){
					mtlfile.getline(buff,256);
					mcoord.push_back(new std::string(buff));
					}*/

			while (!mtlfile.eof())
			{
				mtlfile.getline(buff,256);
		
				if(buff[0]!='#'){ 
					for(i=0;i<8;i++)strcpy(arg[i],"");
					int num_args=sscanf(buff,"%s%s%s%s%s%s%s%s",arg[0],arg[1],arg[2],arg[3],arg[4],arg[5],
							arg[6],arg[7]);	
					if(strcmp(arg[0],"newmtl")==0){	//new material
						sscanf(arg[1],"%s",mtl_objname);
					
						while (!mtlfile.eof() && strcmp(arg[0],"#")!=0){
						mtlfile.getline(mread,256);
						
					for(i=0;i<8;i++)strcpy(arg[i],"");
					int num_args=sscanf(mread,"%s%s%s%s%s%s%s%s",arg[0],arg[1],arg[2],arg[3],arg[4],arg[5],
								arg[6],arg[7]);
					if(strcmp(arg[0],"Ns")==0){	//the shininess
						sscanf(arg[1],"%f",&ns);
						}
					if(strcmp(arg[0],"Ka")==0){   //the ambient
						sscanf(arg[1],"%f",&amb[0]);
						sscanf(arg[2],"%f",&amb[1]);
						sscanf(arg[3],"%f",&amb[2]);
					}
					if(strcmp(arg[0],"Kd")==0){   //the diffuse
						sscanf(arg[1],"%f",&dif[0]);
						sscanf(arg[2],"%f",&dif[1]);
						sscanf(arg[3],"%f",&dif[2]);
					}
					if(strcmp(arg[0],"Ks")==0){   //the specular
						sscanf(arg[1],"%f",&spec[0]);
						sscanf(arg[2],"%f",&spec[1]);
						sscanf(arg[3],"%f",&spec[2]);
					}
					if(strcmp(arg[0],"Ni")==0){   //the I don't know what is this
						sscanf(arg[1],"%f",&ni);
					}
					if(strcmp(arg[0],"d")==0 ){	//the alpha
						sscanf(arg[1],"%f",&alpha);
					/*	if(alpha!=1 )
						alpha = 1 -  alpha;*/
						 
					}
					if(strcmp(arg[0],"illum")==0){	//the illum (I dont know what is this)
						sscanf(arg[1],"illum %d",&illum);
					}
					if(strcmp(arg[0],"map_Kd")==0){
						sscanf(arg[1],"%s",name);
					}
					//materials.push_back(new material(mtl_objname,alpha,ns,ni,dif,amb,spec,illum,-1));
			}
						
			 materials.push_back(new material(mtl_objname,alpha,ns,ni,dif,amb,spec,illum,-1));
		}
	}
}
}
	/*

	if(strcmp(arg[0],"vt")==0){
		float u,v;
		sscanf(arg[1],"%f",&u);
		sscanf(arg[2],"%f",&v);
		texturecoordinate.push_back(new texcoord(u,1-v));
	}*/
	if(strcmp(arg[0],"vn")==0){
		float nx,ny,nz;
		sscanf(arg[1],"%f",&nx);
		sscanf(arg[2],"%f",&ny);
		sscanf(arg[3],"%f",&nz);
		normals.push_back(new coordinate(nx,ny,nz));
		//std::cout << normals.size() << "\t"nx << "\t"ny << "\t"nz << std::endl;
	}
	if(strcmp(arg[0],"usemtl")==0){
		char obj_mtlname[200];
		char read[256];
		
		sscanf (arg[1],"%s",obj_mtlname);
		//std::cout <<obj_mtlname << std::endl;

		for(int k=0;k<materials.size();k++)	//go through all of the materials
		{
			if(strcmp(materials[k]->name.c_str(),obj_mtlname)==0)	//and compare the tmp with the name of the material
			{
				ObjId=k;	//if it's equal then set the current material to that
				
				break;
			}
		}
	while (!objfile.eof() && strcmp(arg[0],"#")!=0){
			objfile.getline(read,256);

			for(i=0;i<8;i++)strcpy(arg[i],"");
				int num_args=sscanf(read,"%s%s%s%s%s%s%s%s",arg[0],arg[1],arg[2],arg[3],arg[4],arg[5],
								arg[6],arg[7]);
			if (strcmp(arg[0],"f")==0){ 
					  int v1,v2,v3,t1,t2,t3,n1,n2,n3;
			
						if (strcmp(arg[1],"%d//%d")==0){
							sscanf(arg[1],"%d//%d",&v1,&n1);
							sscanf(arg[2],"%d//%d",&v2,&n2);
							sscanf(arg[3],"%d//%d",&v3,&n3);
							faces.push_back(new face(v1-1,v2-1,v3-1,0,0,0,n1-1,n2-1,n3-1,curmat));
							FaceId++;
						}
						else { 

						sscanf(arg[1],"%d/%d/%d",&v1,&t1,&n1);
						sscanf(arg[2],"%d/%d/%d",&v2,&t2,&n2);
						sscanf(arg[3],"%d/%d/%d",&v3,&t3,&n3);
						faces.push_back(new face(v1-1,v2-1,v3-1,t1-1,t2-1,t3-1,n1-1,n2-1,n3-1,curmat));
						FaceId++;
					//	std::cout << faces.size() << std::endl;
					//	std::cout <<"a" << ":" << texturecoordinatfaces.at(3)->t1)->u <<  std::endl;
					//	std::cout <<"a" << ":" << texturecoordinate.at(faces.at(3)->t1)->u <<  std::endl;
					}
				}
			}
		//	faces.push_back(new face(v1-1,v2-1,v3-1,t1-1,t2-1,t3-1,n1-1,n2-1,n3-1,curmat));
		//	std::cout << faces.size() << std::endl;
			//	system("PAUSE");
			LastFace = FaceId;
			FaceMaterials.push_back(new FaceMaterial( ObjId,FirstFace, LastFace));
			FirstFace= LastFace;
			ObjId=ObjId+1;
		}
	

	else{	
		continue;	
	}
}
}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	init();
	cube= loadObject("car2.obj"); 
    arVideoCapStart();
    argMainLoop( NULL, keyEvent, mainLoop );
	return (0);
}

static void   keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }
}

/* main loop */
static void mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;

    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }

    arVideoCapNext();

    /* check for object visibility */
    k = -1;
    for( j = 0; j < marker_num; j++ ) {
        if( patt_id == marker_info[j].id ) {
            if( k == -1 ) k = j;
            else if( marker_info[k].cf < marker_info[j].cf ) k = j;
        }
    }
    if( k == -1 ) {
        argSwapBuffers();
        return;
    }

    /* get the transformation between the marker and the real camera */
    arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);

 //   draw();
	meshObj2();
    argSwapBuffers();
}

static void init( void )
{
    ARParam  wparam;
	
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    if( (patt_id=arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        exit(0);
    }

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

static void draw( void )
{
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    /*GLfloat   mat_ambient[]     = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash[]       = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.0, 0.5, 0.5, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
*/

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(patt_trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

	glEnable(GL_COLOR_MATERIAL); // added by tgy
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0); 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
	//glDisable(GL_TEXTURE_2D); // added & commented tgy
	//glColorMaterial(GL_FRONT, GL_AMBIENT); // added & commentedtgy

	glColor3f(1,0,0); // added tgy
    glutSolidCube(50.0); // added tgy
	
	// added by tgy
	glColor3f(0, 1, 1);
	glBegin (GL_QUADS);
		glVertex3f( 0,  0, -10);
		glVertex3f(50,  0, -10);
		glVertex3f(50, 50, -10);
		glVertex3f( 0, 50, -10);
	glEnd();



	glColor3f(0,1,0); // added tgy
	glPushMatrix(); // added tgy
		glTranslatef(70,70,0); // added tgy
		glutSolidCube(10.0);// added tgy
	glPopMatrix();// added tgy

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );
}

void meshObj2(){
	 double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    /*GLfloat   mat_ambient[]     = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash[]       = {1.0, 0.0, 0.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.0, 0.5, 0.5, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
*/

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(patt_trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

	glEnable(GL_COLOR_MATERIAL); // added by tgy
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0); 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); 
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, -25.0 );
glEnable(GL_BLEND);
glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
for(int i=0;i<FaceMaterials.size();i++){
//float diffuse[]={materials[FaceMaterials[i]->ObjId]->dif[0],materials[FaceMaterials[i]->ObjId]->dif[1],materials[FaceMaterials[i]->ObjId]->dif[2],materials[FaceMaterials[i]->ObjId]->alpha};
//float ambient[]={materials[FaceMaterials[i]->ObjId]->amb[0],materials[FaceMaterials[i]->ObjId]->amb[1],materials[FaceMaterials[i]->ObjId]->amb[2],materials[FaceMaterials[i]->ObjId]->alpha};
//float specular[]={materials[FaceMaterials[i]->ObjId]->spec[i],materials[FaceMaterials[i]->ObjId]->spec[1],materials[FaceMaterials[i]->ObjId]->spec[2],materials[FaceMaterials[i]->ObjId]->alpha};
//			glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
//			glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
//			glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
//			glMaterialf(GL_FRONT,GL_SHININESS,materials[FaceMaterials[i]->ObjId]->ns);	
float colorMesh[4] ={materials[FaceMaterials[i]->ObjId]->dif[0],materials[FaceMaterials[i]->ObjId]->dif[1],materials[FaceMaterials[i]->ObjId]->dif[2],materials[FaceMaterials[i]->ObjId]->alpha};
glColor4fv(colorMesh);
				

	
glBegin(GL_TRIANGLES);
	for(int f=FaceMaterials[i]->FirstFace; f < FaceMaterials[i]->LastFace;f++){
			for(int b=0; b<3;b++){
			glVertex3f(vertex[faces[f]->v[b]]->x,vertex[faces[f]->v[b]]->y,vertex[faces[f]->v[b]]->z);
			}
		}
	glEnd();

	
}
glFlush();
glDisable(GL_TEXTURE_2D);
//glDisable(GL_BLEND);
}