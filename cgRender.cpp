//
//  cgRender.cpp
//  coursework1
//
//  Created by Jie on 2/14/13.
//  Copyright (c) 2013 Jie. All rights reserved.
//
#include "cgRender.h"


#define VERTEX 26460
#define POLYGON 52260
#define TEXTURE_PATH "/Users/apple/Downloads/courseworkPart1/data/face.ppm"
#define DATA_PATH "/Users/apple/Downloads/courseworkPart1/data/face.vtk"

float vertex[VERTEX][3];
int polygons[POLYGON][4];
float texture[VERTEX][2];
float lookX = 0.3;

void init()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    cout << "init" << endl;
    
    glShadeModel (GL_SMOOTH);
    
    // Enable lighting
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    
    glEnable (GL_TEXTURE_2D);
    
    GLfloat LightPosition[]= {-0.1, 0.1, 0.2, 0.0};
    GLfloat LightAmbient[]= {0.5, 0.5, 0.5, 1.00};
    GLfloat LightDiffuse[]= {0.5, 0.5, 0.5, 1.00};
    GLfloat LightSpecular[]= {0.5, 0.5, 0.5, 1.00};
    
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  LightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  LightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
    
    // Set material parameters
    GLfloat MaterialAmbient[]= {0.3, 0.3, 0.3, 1.00};
    GLfloat MaterialDiffuse[]= {0.5, 0.5, 0.5, 1.00};
    GLfloat MaterialSpecular[]= {0.5, 0.5, 0.5, 1.0};
    GLfloat MaterialEmission[]= {0.3, 0.3, 0.3, 1.00};
    GLfloat MaterialShininess= 10;
    
    /*   gouraud-3
     GLfloat MaterialAmbient[]= {0.2, 0.2, 0.2, 1.00};
     GLfloat MaterialDiffuse[]= {0.3, 0.3, 0.3, 1.00};
     GLfloat MaterialSpecular[]= {0.1, 0.1, 0.1, 1.0};
     GLfloat MaterialEmission[]= {0.3, 0.3, 0.3, 1.00};
     GLfloat MaterialShininess= 5;
     */
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  MaterialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  MaterialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  MaterialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  MaterialEmission);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MaterialShininess);
    
    // Enable Z-buffering
    glEnable(GL_DEPTH_TEST);
}

//read VTK file
void readFile()
{
    std::string line;
    ifstream myfile (DATA_PATH);
        
    if (myfile.is_open())
    {
        while ( myfile.good() )
        {
            getline (myfile,line);
            
            // store to vertex array
            if (line.substr(0,6).compare("POINTS") == 0)
            {
                for (int i = 0; i < 8820; i++)
                {  
                    for (int j = 0; j < 3; j++)
                    {
                        myfile >> vertex[i*3+j][0] >> vertex[i*3+j][1]>> vertex[i*3+j][2];
                    }
                    getline (myfile,line);
                }
            }
            
            // store to polygon array
            if (line.substr(0,8).compare("POLYGONS") == 0)
            {
                for (int i = 0; i < POLYGON; i++)
                {
                    myfile >> polygons[i][0] >> polygons[i][1]>> polygons[i][2]>>polygons[i][3];
                    getline (myfile,line);
                }
            }
            
            // store to texture array
            if (line.substr(0,10).compare("POINT_DATA") == 0)
            {
                getline (myfile,line);
                for (int i = 0; i < 5880; i++)
                {
                    if (i%2==0) {
                        for (int j = 0; j < 4; j++)
                        {
                            myfile >> texture[(i/2)*9+j][0] >> texture[(i/2)*9+j][1];
                        }
                        myfile >> texture[(i/2)*9+4][0];
                    } else {
                        myfile >> texture[(i/2)*9+4][1];
                        for (int j = 5; j < 9; j++)
                        {
                            myfile >> texture[(i/2)*9+j][0] >> texture[(i/2)*9+j][1];
                        }
                    }
                    getline (myfile,line);
                }
            }
        }
        myfile.close();
    }
    
    else cout << "Unable to open file";   
 }

//Load texture
GLuint LoadTexture(const char *filename)
{
    GLuint textureArr;
	GLubyte *image;
    FILE *file;
	char type;
	int depth, width, height;
    
    // The following code will read in PPM file
    file = fopen(filename, "rb");
    if (file == NULL) return 0;
	
	// read PPM magic number (P1 to P6)
	if(fscanf(file, "P%c \n", &type)!= 1 || type< '1' || type> '8')
	{
        fclose(file);
        return NULL;
	}
	fscanf(file, "%d", &width);
	fscanf(file, "%d", &height);
	fscanf(file, "%d", &depth);
    /* skip max. component and exactly one whitespace */
    fscanf(file, "%*d%*c");
    
    image = (unsigned char *)malloc(width*height*3);
	
	fread(image, width*height*3, 1, file);
    fclose(file);
    
    glGenTextures(1, &textureArr); //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, textureArr); //bind the texture to its array
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
    //Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
    free(image); //free the texture
    return textureArr; //return whether it was successfull
}

// normalize vector
float *normalize(float *v)
{
    float length;
    
    length = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= length;
    v[1] /= length;
    v[2] /= length;
    
    return v;
}

// render face
void rendering()
{
    // coordinates of each vertex
    float px[POLYGON], py[POLYGON], pz[POLYGON];
    
    // difference for two vertexes
    float px01[POLYGON], py01[POLYGON], pz01[POLYGON];
    float px02[POLYGON], py02[POLYGON], pz02[POLYGON];
    
    // normal vector of each polygon
    float nv[POLYGON][3];
    
    // sum of normal vector for same vertex
    float nvSum[POLYGON][3];
    
    // average for same vertex
    float nvAve[POLYGON][3];
    
    // normalization
    float norm[POLYGON][3];
    
    // for all polygons
    for (int i = 0; i < POLYGON; i++)
    {
        int index[3];
        for (int j = 0; j < 3; j++)
        {
            index[j] = polygons[i][j+1];
            px[index[j]] = vertex[index[j]][0];
            py[index[j]] = vertex[index[j]][1];
            pz[index[j]] = vertex[index[j]][2];
        }
        
        px01[i] = px[index[0]] - px[index[1]];
        py01[i] = py[index[0]] - py[index[1]];
        pz01[i] = pz[index[0]] - pz[index[1]];
        
        px02[i] = px[index[0]] - px[index[2]];
        py02[i] = py[index[0]] - py[index[2]];
        pz02[i] = pz[index[0]] - pz[index[2]];
        
        nv[i][0] = py01[i]*pz02[i] - py02[i]*pz01[i];
        nv[i][1] = pz01[i]*px02[i] - pz02[i]*px01[i];
        nv[i][2] = px01[i]*py02[i] - px02[i]*py01[i];
    }
    
    // calculate sum
    int count[VERTEX];
    for (int i = 0; i < POLYGON; i++)
    {
        for (int k = 1; k < 4; k++)
        {
            count[polygons[i][k]]++;
            nvSum[polygons[i][k]][0] += nv[i][0];
            nvSum[polygons[i][k]][1] += nv[i][1];
            nvSum[polygons[i][k]][2] += nv[i][2];
        }
    }
    
    // calculate average
    for (int i = 0; i < VERTEX; i++)
    {
        nvAve[i][0] = nvSum[i][0]/count[i];
        nvAve[i][1] = nvSum[i][1]/count[i];
        nvAve[i][2] = nvSum[i][2]/count[i];
        
        float* p= normalize(nvAve[i]);
        norm[i][0]=p[0];
        norm[i][1]=p[1];
        norm[i][2]=p[2];
    }
    
    // render face
    for (int i = 0; i < POLYGON; i++)
    {
        glBegin(GL_POLYGON);

        int index[3];
        for (int j = 0; j < 3; j++)
        {
            index[j] = polygons[i][j+1];
            glTexCoord2f(texture[index[j]][0],texture[index[j]][1]);
            glNormal3f(norm[index[j]][0],norm[index[j]][1],norm[index[j]][2]);
            glVertex3f(vertex[index[j]][0],vertex[index[j]][1],vertex[index[j]][2]);
        }
        
        glEnd();
    }
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cout << "display" << endl;
    
    // render face
    rendering();
    
    glFlush();    
}

void reshape (int w, int h)
{
    cout << "reshape" << endl;
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluPerspective(42, (GLfloat) w / (GLfloat) h, 0.001, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(lookX, -0.12, -0.1, -0.6, -0.15, 0.0, -0.5, 1.0, 0.0);
    // gouraud-1 gluLookAt(0.3, -0.1, -0.1, -0.6, -0.2, 0.0, -0.5, 1.0, 0.0);
    // gouraud-2 gluLookAt(0.4, -0.1, -0.1, -0.6, -0.2, 0.0, -1.0, 1.0, 0.0);
    // gouraud-3 gluLookAt(0.4, -0.05, -0.15, -0.6, -0.2, 0.0, -1.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27: // ESC
            exit(0);
            break;
        case 'A': // A
            lookX += 0.1;
            break;
    }
}

int main(int argc, char** argv)
{
    // Initialize graphics window
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    
    glutInitWindowSize (512, 512);
    glutInitWindowPosition (0, 0);
    glutCreateWindow (argv[0]);
    
    // Initialize OpenGL
    init();
    
    // Read file
    readFile();
    
    // Initialize callback functions
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    //Load texture
    GLuint textureArr = LoadTexture(TEXTURE_PATH);
    
    // Start rendering 
    glutMainLoop();
    
    glDeleteTextures(1, &textureArr); 
}
