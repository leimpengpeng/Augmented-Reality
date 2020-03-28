

#ifndef OBJ_PARSE 
#define OBJ_PARSE 

#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>


#define STRINGSIZE 80

void my_model();
void meshObj2();
void door1();
void door2();
void door3();
void door4();
void sofa();
void meshObj();
void tyre();
void other();

void car_model();
void indicate();
void antenna();
void bumper();
void type();
void hood();
void window();
void mirror();
void HeadLight();
void FogLight();
void TailLight();
void Interior();
void Exhaust(); 
void Trunk();
void Grilles();
void Roof();
void statical();



int loadObject();

int cube;//Function prototype
char mtlFile[STRINGSIZE];
char name[200];//textureName

float rotate_d1=0;
float rotate_d2=0;
float rotate_d3=0;
float rotate_d4=0;
int moveX,moveY=0;
int toX,toY,toZ=0;
int typeX=0;
int hoodZ=0;
int fenderX,fenderY,fenderZ=0;
int doorX=0;
int doorR=0;
float mirrorL,mirrorR=0;

int headlight,foglight,taillight=0;
int interior_, exhaust_,trunk_,grilles_,roof_=0;

int rotate_=0;
struct coordinate{
	float x,y,z;
	coordinate(float a,float b,float c) : x(a),y(b),z(c) {};
};
struct texcoord{
	float u,v;
	texcoord(float a,float b):u(a),v(b){};
};
struct face{
// int facenum;
 int v[3];
 int t[3];
 int n[3];
 int mat;
 face(int v1,int v2,int v3, int t1, int t2
			, int t3, int n1, int n2,int n3,int m);
};
/*
struct material {
char *name; 
int texture ; //the id for texture
material ( char*name , int t);
};
*/
struct material{
	std::string name;	//the name of the material
	float alpha,ns,ni;	//some property, alpha, shininess, and some other, which we not used
	float dif[3],amb[3],spec[3];	//the color property (diffuse, ambient, specular)
	int illum;	//illum - we not use it
	int texture;	//the id for the texture, if there is no texture than -1
	material(const char* na,float al,float n,float ni2,float* d,float* a,float* s,int i,int t);
};
struct FaceMaterial{
int FirstFace,LastFace,ObjId;

FaceMaterial(int ObjId , int FirstFace,int LastFace);
};

face::face(int v1,int v2,int v3, int t1, int t2
	, int t3, int n1, int n2,int n3,int m){
//	facenum=facen;
	v[0]=v1;
	v[1]=v2;
	v[2]=v3;
	t[0]=t1;
	t[1]=t2;
	t[2]=t3;
	n[0]=n1;
	n[1]=n2;
	n[2]=n3;
	mat=m;
}

material::material(const char* na,float al,float n,float ni2,float* d,float* a,float* s,int i,int t)
	{
		name=na;
		alpha=al;
		ni=ni2;
		ns=n;
		dif[0]=d[0];
		dif[1]=d[1];
		dif[2]=d[2];
		
		amb[0]=a[0];
		amb[1]=a[1];
		amb[2]=a[2];
		
		spec[0]=s[0];
		spec[1]=s[1];
		spec[2]=s[2];
		
		illum=i;
		texture=t;
	}

FaceMaterial:: FaceMaterial(int c,int a,int b){
	ObjId=c;
	FirstFace=a;
	LastFace=b;
}
/*
material::material( char*name,int t){
 name=name;
 texture=t;
}*/

std::vector<std::string*> coord;
std::vector<std::string*>mcoord;
std::vector<coordinate*> vertex;
std::vector<coordinate*> normals;
std::vector<texcoord*> texturecoordinate;
std::vector<face*>faces;
std::vector<FaceMaterial*>FaceMaterials;
std::vector<material*> materials;
std::vector<std::string*> texture;//the id for all the textures







#endif 