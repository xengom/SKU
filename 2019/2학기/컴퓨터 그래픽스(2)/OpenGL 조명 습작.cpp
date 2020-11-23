#include <glut.h>
#include <glu.h>
#include <gl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float f_rot_x = 0.0f;
float f_rot_y = 0.0f;

float* Vertex_array;
int* Faces_array;

int VertexNum=0;
int FaceNum=0;

int i_angle_change=0;
float f_scale = 0.2;

void light_init( )
{
	GLfloat global_ambient[ ] = {0.1, 0.1, 0.1, 1.0};	

	GLfloat light0_ambient[ ] = {0.5, 0.4, 0.3, 1.0}; 
	GLfloat light0_diffuse[ ] = {0.5, 0.4, 0.3, 1.0};
	GLfloat light0_specular[ ] = {1.0, 1.0, 1.0, 1.0};

	GLfloat light1_ambient[ ] =  {0.0, 0.0, 0.0, 1.0};	
	GLfloat light1_diffuse[ ] =  {0.5, 0.2, 0.3, 1.0};
	GLfloat light1_specular[ ] = {0.0, 0.0, 0.0, 1.0};

	GLfloat material_ambient[ ] = {0.3, 0.3, 0.3, 1.0};	
	GLfloat material_diffuse[ ] =  {0.8, 0.8, 0.8, 1.0};
	GLfloat material_specular[ ] =  {0.0, 0.0, 1.0, 1.0};
	GLfloat material_shininess[ ] = {25.0};

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void load_PLY(void)
{
	char* filename;
	FILE* fp;

	filename="Stanford_Bunny.ply";
	fp = fopen(filename,"r");

	int i = 0;
	char buffer[1000];

	fgets(buffer,120,fp);

	while (  strncmp( "element vertex", buffer,strlen("element vertex")) != 0  )
	{
		fgets(buffer,300,fp);			
	}
	strcpy(buffer, buffer+strlen("element vertex"));
	sscanf(buffer,"%i", &VertexNum);

	fseek(fp,0,SEEK_SET);
	while (  strncmp( "element face", buffer,strlen("element face")) != 0  )
	{
		fgets(buffer,300,fp);			
	}
	strcpy(buffer, buffer+strlen("element face"));
	sscanf(buffer,"%i", &FaceNum);

	while (  strncmp( "end_header", buffer,strlen("end_header")) != 0  )
	{
		fgets(buffer,300,fp);			
	}

	Vertex_array = (float*) malloc(sizeof(float)*VertexNum*3);
	Faces_array = (int*) malloc(sizeof(int)*FaceNum*3);

	i =0;
	for (int x = 0; x < VertexNum; x++)
	{
		fgets(buffer,120,fp);
		sscanf(buffer,"%f %f %f", &Vertex_array[i], &Vertex_array[i+1], &Vertex_array[i+2]);
		i += 3;
	}

	i =0;
	for (int x = 0; x < FaceNum; x++)
	{
		fgets(buffer,300,fp);
		if (buffer[0] == '3')
		{
			int vertex1 = 0, vertex2 = 0, vertex3 = 0;
			buffer[0] = ' ';
			sscanf(buffer,"%i%i%i", &vertex1,&vertex2,&vertex3 );
			Faces_array[i] = vertex1;
			Faces_array[i+1] = vertex2;
			Faces_array[i+2] = vertex3;
		}
		i += 3;
	}
	fclose(fp);
}

void Normalize_draw()
{
	float X=0.0;
	float Y=0.0;
	float Z=0.0;

	float f_MaxX=0.0;
	float f_MaxY=0.0;
	float f_MaxZ=0.0;

	float f_MinX=0.0;
	float f_MinY=0.0;
	float f_MinZ=0.0;

	float f_MidX=0.0;
	float f_MidY=0.0;
	float f_MidZ=0.0;

	float f_normalize_x=0.0;
	float f_normalize_Y=0.0;
	float f_normalize_Z=0.0;

	float f_normalize_X1=0.0;
	float f_normalize_X2=0.0;

	float f_normalize_Y1=0.0;
	float f_normalize_Y2=0.0;

	float f_normalize_Z1=0.0;
	float f_normalize_Z2=0.0;

	float f_normalize_size;

	f_MaxX=Vertex_array[0];
	f_MaxY=Vertex_array[1];
	f_MaxZ=Vertex_array[2];

	f_MinX=Vertex_array[0];
	f_MinY=Vertex_array[1];
	f_MinZ=Vertex_array[2];

	int i_num1=0;
	int i_num2=1;
	int i_num3=2;

	for (int x=0; x<VertexNum; x++)
	{
		if (f_MaxX<Vertex_array[i_num1]) {f_MaxX=Vertex_array[i_num1];}
		if (f_MaxY<Vertex_array[i_num2]) {f_MaxY=Vertex_array[i_num2];}
		if (f_MaxZ<Vertex_array[i_num3]) {f_MaxZ=Vertex_array[i_num3];}
		if (f_MinX>Vertex_array[i_num1]) {f_MinX=Vertex_array[i_num1];}
		if (f_MinY>Vertex_array[i_num2]) {f_MinY=Vertex_array[i_num2];}
		if (f_MinZ>Vertex_array[i_num3]) {f_MinZ=Vertex_array[i_num3];}
		i_num1+=3;
		i_num2+=3;
		i_num3+=3;
	}

	f_MidX=(f_MaxX+f_MinX)/2;
	f_MidY=(f_MaxY+f_MinY)/2;
	f_MidZ=(f_MaxZ+f_MinZ)/2;
	X= 2/(f_MaxX-f_MinX);
	Y= 2/(f_MaxY-f_MinY);
	Z= 2/(f_MaxZ-f_MinZ);

	int i=0;
	for(int x = 0; x < FaceNum; x++)
	{
		glBegin(GL_TRIANGLES);

		f_normalize_X1= (Vertex_array[3*Faces_array[i+1]]-f_MidX)*X-(Vertex_array[3*Faces_array[i]]-f_MidX)*X;
		f_normalize_Y1=(Vertex_array[3*Faces_array[i+1]+1]-f_MidY)*Y-(Vertex_array[3*Faces_array[i]+1]-f_MidY)*Y;
		f_normalize_Z1=(Vertex_array[3*Faces_array[i+1]+2]-f_MidZ)*Z-(Vertex_array[3*Faces_array[i]+2]-f_MidZ)*Z;

		f_normalize_X2= (Vertex_array[3*Faces_array[i+2]]-f_MidX)*X-(Vertex_array[3*Faces_array[i]]-f_MidX)*X;
		f_normalize_Y2=(Vertex_array[3*Faces_array[i+2]+1]-f_MidY)*Y-(Vertex_array[3*Faces_array[i]+1]-f_MidY)*Y;
		f_normalize_Z2=(Vertex_array[3*Faces_array[i+2]+2]-f_MidZ)*Z-(Vertex_array[3*Faces_array[i]+2]-f_MidZ)*Z;

		f_normalize_x=f_normalize_Y1*f_normalize_Z2-f_normalize_Z1*f_normalize_Y2;
		f_normalize_Y=f_normalize_X1*f_normalize_Z2-f_normalize_Z1*f_normalize_X2;
		f_normalize_Z=f_normalize_X1*f_normalize_Y2-f_normalize_Y1*f_normalize_X2;
		f_normalize_size=sqrt(f_normalize_x*f_normalize_x+f_normalize_Y*f_normalize_Y+f_normalize_Z*f_normalize_Z);

		glNormal3f(f_normalize_x/f_normalize_size, f_normalize_Y/f_normalize_size, f_normalize_Z/f_normalize_size);
		glVertex3f((Vertex_array[3*Faces_array[i]]-f_MidX)*X,(Vertex_array[3*Faces_array[i]+1]-f_MidY)*Y,(Vertex_array[3*Faces_array[i]+2]-f_MidZ)*Z);
		glVertex3f((Vertex_array[3*Faces_array[i+1]]-f_MidX)*X,(Vertex_array[3*Faces_array[i+1]+1]-f_MidY)*Y,(Vertex_array[3*Faces_array[i+1]+2]-f_MidZ)*Z);
		glVertex3f((Vertex_array[3*Faces_array[i+2]]-f_MidX)*X,(Vertex_array[3*Faces_array[i+2]+1]-f_MidY)*Y,(Vertex_array[3*Faces_array[i+2]+2]-f_MidZ)*Z);

		i+=3;
		glEnd();
	}
}

void MyDisplay()
{
	GLfloat LightPosition0[] ={0.0, 1.0, 2.0, 1.0};
	GLfloat LightPosition1[] ={1.0, 1.0, 1.0, 1.0};
	GLfloat LightDirection1[] ={-1.0, -2.0, -1.0};
	GLfloat SpotAngle1[] ={20.0};

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix( );
		glLoadIdentity();
		glRotatef(f_rot_x, 0.0, 1.0, 0.0);
		glRotatef(f_rot_y, 1.0, 0.0, 0.0);
		glScalef(f_scale, f_scale, f_scale);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);
		Normalize_draw();
	glPopMatrix();
	glPushMatrix();
		glLoadIdentity();
		glRotatef(i_angle_change, 0.0, 1.0, 0.0);
		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);
		glTranslatef(0.0, 0.0, 2.0);
		glDisable(GL_LIGHTING);
		glutWireSphere(0.03, 10, 10);
		glColor3f(0.5, 0.5, 0.5);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glutSwapBuffers();
}

void Myreshape (int Width, int Heigit)
{
	glViewport(0, 0, Width, Heigit);
	GLfloat widthFactor=(GLfloat)Width/(GLfloat)600;
	GLfloat heightFactor=(GLfloat)Heigit/(GLfloat)600;
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho (widthFactor*(-1.0), widthFactor*(1.0), heightFactor*(-1.0), heightFactor*(1.0), -1.0, 1.0); // 왜곡방지
}

void MyKeyboard(int key, int x, int y)
{
	switch (key){
	case GLUT_KEY_UP:
		f_rot_y -= 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		f_rot_y += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		f_rot_x -= 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		f_rot_x += 1.0f;
		glutPostRedisplay();
		break;
	}
}

void MyKeyboard_spc (unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		f_scale=f_scale-0.1;
		glutPostRedisplay();
		break;
	case 'z':
		f_scale=f_scale+0.1;
		glutPostRedisplay();
		break;
	case 's':
		i_angle_change = (i_angle_change +20)%360;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode ( GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH );
	glutInitWindowSize (600,600);
	glutInitWindowPosition (0,0);
	glutCreateWindow ("최현 조");
	glClearColor(0.4, 0.4, 0.4, 0.4);
	load_PLY();
	light_init( );
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(Myreshape);
	glutSpecialFunc(MyKeyboard);
	glutKeyboardFunc(MyKeyboard_spc);
	glutMainLoop();
}