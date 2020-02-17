/*I wrote this program myself using the code provided by the textbook.*/
#include <windows.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
/*#include <iostream.h>*/ 
#include "glut.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>

using namespace std;
#define DEG2RAD (3.14159/180.0) 

int screenWidth = 1500;
int screenHeight = 800;
double ar = 1;
GLdouble A, B, C, D;
GLsizei W, H;
int radX = 0; //to be filled by readEllipse
int radY = 0;
int cx = 0;
int cy = 0;

float per;
float sect;
static int lineCount = 0;

//vector <float> red, green, blue;

class Color {
public:
	GLfloat r, g, b;
};

void setColor(Color c) {
	glColor3f(c.r, c.g, c.b);
}
Color Colors[10] = { { 0.7,0.1,0.1 },{ 1.0,0.5,0.0 },{ 0.9,0.9,0.0 },{ 0.0,1.0,0.0 },{ 0.1,0.1,0.7 },{ 1.0,0.0,1.0 }, {1.0,0.5,0.5}, {0.5,1.0,0.5}, {1.0,1.0,1.0}, {0.5,0.5,0.5}}; //ROYGBP

class Group {
public:
	Color c;
	string className;
};
vector <Group> classColors;

float minVal = FLT_MAX;
float maxVal = -1;

class GLintPoint{
public:
	GLint x, y;
};

vector <GLintPoint> sectors;
static int mouseClicks = 0;
static int dataRead = 0;
static int hideEllipses = 0;
static int animated = 0;

struct LineData{
public:
	int numD; //to be filled by readValues
	float locPointD [10]; //8 = max # of dimensions
	GLintPoint matchPoints [10]; //to be filled by readEllipse
	GLintPoint ellipsePoints [10];
	GLintPoint interceptPoints [5];
	string className;

	void setPoint(GLint x, GLint y, int i)
	{
		matchPoints[i].x = x;
		matchPoints[i].y = y;
	}

	void setPointE(GLint x, GLint y, int i)
	{
		ellipsePoints[i].x = x;
		ellipsePoints[i].y = y;
	}

	void setPointI(GLint x, GLint y, int i)
	{
		interceptPoints[i].x = x;
		interceptPoints[i].y = y;
	}
};

vector <LineData> data;

void readEllipse(){
	//read line by line
	string line;
	ifstream myFile("EllipseDim.txt");
	if(myFile.is_open())
	{
		int count = 0;
		while(getline(myFile,line))
		{
			//first = cx
			//second = cy
			//third = radX
			//fourth = radY
			int lineVal = atoi(line.c_str());
			switch(count){
			case 0:
				cx = lineVal;
				break;
			case 1:
				cy = lineVal;
				break;
			case 2:
				radX = lineVal;
				break;
			case 3:
				radY = lineVal;
				break;
			}
			count++;
		}
	}
}

void normEll(){
	//calculate circumference for given section

	//normalize to 0-1 range
	
	//calculate x and y for a given normalized value

}

void normalize(){
	for(int n = 0; n < data.size(); n++){
		for(int i = 0; i < data[n].numD; i++){
			data[n].locPointD[i] = (data[n].locPointD[i] - minVal) / (maxVal - minVal);
		}
	}
}

void swap(int dimOne, int dimTwo) 
{
	for (int i = 0; i < lineCount; i++)
	{
		float temp = data[i].locPointD[dimOne];
		data[i].locPointD[dimOne] = data[i].locPointD[dimTwo];
		data[i].locPointD[dimTwo] = temp;
	}
}

void invert(int dimensionIndex) 
{
	for (int i = 0; i < lineCount; i++) 
	{
		data[i].locPointD[dimensionIndex] = 1 - data[i].locPointD[dimensionIndex];
		//cout << "sector " << dimensionIndex << " inverted for instance " << i << endl;
		//	cout << "instance " << i << ": ";
		/*	for (int j = 0; j < data[i].numD; j++) {
				cout << data[i].locPointD[j] << " ";
			}
			cout << endl;*/
		
	}
	//cout << "-----" << endl;
}

void scale(float scalar, int dimensionIndex)
{
	for (int i = 0; i < lineCount; i++)
	{
		data[i].locPointD[dimensionIndex] = data[i].locPointD[dimensionIndex] * scalar;

	}
}

void readValues(){
	//read line
	string line;
	ifstream myFile("Values.txt");

	if(myFile.is_open())
	{
		while (getline(myFile, line))
		{
			string delim = ","; 	//line elements (delimiter = ',')

			int index = 0;
			size_t pos = 0;
			string token;

			LineData d;
			data.push_back(d);
			while ((pos = line.find(delim)) != string::npos) {
				token = line.substr(0, pos);
				data[lineCount].locPointD[index] = stof(token.c_str());
				if (data[lineCount].locPointD[index] < minVal) { minVal = data[lineCount].locPointD[index]; }
				if (data[lineCount].locPointD[index] > maxVal) { maxVal = data[lineCount].locPointD[index]; }
				index++;
				line.erase(0, pos + delim.length());
				data[lineCount].numD = index;
			}
			data[lineCount].className = line; //token.c_str(); //last element = class
			//lineCount++;


			//if new class, assign color
			int i = 0;
			bool newColor = true;
			for (; i < classColors.size(); i++) {
				if (classColors[i].className.compare(data[lineCount].className) == 0) {
					newColor = false;
				}
			}
			if (newColor) {
				Group g;
				g.className = data[lineCount].className;
				g.c = Colors[i];
				classColors.push_back(g);
			}

			lineCount++;
		}

		//odd dimensional data
		for (int i = 0; i < lineCount; i++) {
			if (data[i].numD % 2 == 1) {
				data[i].numD += 1; //increment number of dimensions
				data[i].locPointD[data[i].numD-1] = data[i].locPointD[data[i].numD-2]; //duplicate last dimension value
				
			}
		}
		
	}
	normalize();
}

void myInit(void)
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)screenWidth, 0.0, (GLdouble)screenHeight);
	A = screenWidth / 4.0;
	B = 0.0;
	C = D = screenHeight / 2.0;
}

void randomizeColor(int classIndex) 
{
	/*
	r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);*/
	classColors[classIndex].c.r = rand() / ((float)RAND_MAX + 1);
	classColors[classIndex].c.g = rand() / ((float)RAND_MAX + 1);
	classColors[classIndex].c.b = rand() / ((float)RAND_MAX + 1);
}

void drawIntercept(int i, int j, int lineNum)
{
	float hSq = pow(radY, 2);
	float wSq = pow(radX, 2);

	int intY;
	int intX;

	if ((data[lineNum].numD / 2) % 2 == 0) //no down ellipses
	{
		intY = -(pow(data[lineNum].ellipsePoints[i].y, 2) - pow(data[lineNum].ellipsePoints[j].y, 2)) / (2 * (-data[lineNum].ellipsePoints[i].y + data[lineNum].ellipsePoints[j].y));
		intX = data[lineNum].ellipsePoints[i].x;

		if (i < data[lineNum].numD / 2) { //right ellipses
			intX -= sqrtf(wSq - (wSq*pow(intY - data[lineNum].ellipsePoints[i].y, 2) / hSq));
		}
		else { //left ellipse
			intX += sqrtf(wSq - (wSq*pow(intY - data[lineNum].ellipsePoints[i].y, 2) / hSq));
		}

	} 
	else {  // down ellipses
		intY = -(pow(data[lineNum].ellipsePoints[i].y, 2) - pow(data[lineNum].ellipsePoints[j].y, 2)) / (2 * (-data[lineNum].ellipsePoints[i].y + data[lineNum].ellipsePoints[j].y));
		intX = data[lineNum].ellipsePoints[i].x;

		if (i < data[lineNum].numD / 2 - 1) { //right ellipses
			intX -= sqrtf(wSq - (wSq*pow(intY - data[lineNum].ellipsePoints[i].y, 2) / hSq));
		}
		else if (data[lineNum].numD / 2 - 1 == i) { //down ellipses
			//intX and intY calculations
			intX = -(pow(data[lineNum].ellipsePoints[i].x, 2) - pow(data[lineNum].ellipsePoints[j].x, 2)) / (2 * (-data[lineNum].ellipsePoints[i].x + data[lineNum].ellipsePoints[j].x));
			intY = data[lineNum].ellipsePoints[i].y;
			intY += sqrtf(hSq - (hSq*pow(intX - data[lineNum].ellipsePoints[i].x, 2) / wSq));
		}
		else { //left ellipse
			intX += sqrtf(wSq - (wSq*pow(intY - data[lineNum].ellipsePoints[i].y, 2) / hSq));
		}
		
	}

	//intercept
	/*
	if(data[lineNum].className == "Iris-setosa"){
		glColor3f(r1, g1, b1);
	}else if(data[lineNum].className == "Iris-versicolor"){
		glColor3f(r2, g2, b2);
	}else{
		glColor3f(r3, g3, b3);
	}
	*/
	float r, g, b;
	int k = 0;
	while (classColors[k].className.compare(data[lineNum].className) != 0) {
		k++;
	}

	r = classColors[k].c.r;
	g = classColors[k].c.g;
	b = classColors[k].c.b;

	glColor3f(r,g,b);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex2i(intX, intY);
	glEnd();

	data[lineNum].setPointI(intX, intY, i/2);
}



void DrawEllipseRight(int index, int lineNum)
{
   //find intercept
   int x = data[lineNum].matchPoints[index].x;
   int y = data[lineNum].matchPoints[index].y;
   int a = cx + radX;
   float b = 0;

   float hSq = pow(radY,2); //H^2
   float wSq = pow(radX,2); //W^2
   int xa = x - a;
   float xaSq = pow(xa, 2); //(x-a)^2

   if(index % 2 == 0){ // red: top of ellipse
	   glColor3f(1.0f, 0.0f, 0.0f);
		b = y - sqrtf(hSq*(1-(xaSq/wSq)));
   }else{ //blue: bottom of ellipse
	   glColor3f(0.0f, 0.0f, 1.0f);
		b = y + sqrtf(hSq*(1-(xaSq/wSq)));
   }

 
   glBegin(GL_LINE_LOOP);
		//glColor3f(0.0,1.0,0.0);
	   if (hideEllipses == 0)
	   {
		   for (int i = 0; i < 360; i++)
		   {
			   float rad = i*DEG2RAD;
			   glVertex2f(cos(rad)*radX + a, sin(rad)*radY + b);

		   }
	   }
	    data[lineNum].setPointE(a, b, index);
   glEnd();
}

void DrawEllipseLeft(int index, int lineNum)
{
   //find intercept
   int x = data[lineNum].matchPoints[index].x;
   int y = data[lineNum].matchPoints[index].y;
   int a = cx - radX;
   float b = 0;

   float hSq = pow(radY,2);
   float wSq = pow(radX,2);
   int xa = x - a;
   float xaSq = pow(xa, 2);

   if(index % 2 == 1){ // red: top of ellipse
	   glColor3f(1.0f, 0.0f, 0.0f);
		b = y - sqrtf((1-(xaSq/wSq))*hSq);
   }else{ //blue: bottom of ellipse
	   glColor3f(0.0f, 0.0f, 1.0f);
		b = y + sqrtf((1-(xaSq/wSq))*hSq);
   }

	glBegin(GL_LINE_LOOP);
		//glColor3f(0.0,1.0,0.0);
		if (hideEllipses == 0)
		{
			for (int i = 0; i < 360; i++)
			{
				float rad = i*DEG2RAD;
				glVertex2f(cos(rad)*radX + a, sin(rad)*radY + b);
			}
		}
		data[lineNum].setPointE(a, b, index);
	glEnd();
}

void DrawEllipseDown(int index, int lineNum)
{
	//find intercept
	int x = data[lineNum].matchPoints[index].x;
	int y = data[lineNum].matchPoints[index].y;
	int b = cy - radY;
	float a = 0;

	float hSq = pow(radY, 2);
	float wSq = pow(radX, 2);
	int yb = y - b;
	float ybSq = pow(yb, 2);

	if (index % 2 == 0) { // red: right of ellipse
		glColor3f(1.0f, 0.0f, 0.0f);
		a = x - sqrtf((1 - (ybSq / hSq))*wSq); //
	}
	else { //blue: left of ellipse
		glColor3f(0.0f, 0.0f, 1.0f);
		a = x + sqrtf((1 - (ybSq / hSq))*wSq); //
	}

	glBegin(GL_LINE_LOOP);
	//glColor3f(0.0,1.0,0.0);
	if (hideEllipses == 0)
	{
		for (int i = 0; i < 360; i++)
		{
			float rad = i*DEG2RAD;
			glVertex2f(cos(rad)*radX + a, sin(rad)*radY + b);
		}
	}
	data[lineNum].setPointE(a, b, index);
	glEnd();
}

void elPer (int lineNum){
	per = (3.14159*(3*(radX + radY)) - sqrt((3*radX+radY)*(radX+3*radY)));
	sect = per/data[lineNum].numD;
	
	//red divider points
	for (int j = 0; j < data[lineNum].numD; j++)
	{
		GLintPoint pt;
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0, 0.0, 0.0);
		//float rad = ((360 / data[lineNum].numD*j))*DEG2RAD;
		float rad = (90 - (360 / data[lineNum].numD*j))*DEG2RAD;
		glVertex2i(cos(rad)*radX + cx, sin(rad)*radY + cy);
		if (lineNum == 0) {
			pt.x = (int)(cos(rad)*radX + cx);
			pt.y = (int)(sin(rad)*radY + cy);
			sectors.push_back(pt); //sector creation
		}
		glEnd();
		
	}
	
	if (lineNum == 0) {
		GLintPoint pt;
		pt.x = sectors[0].x;
		pt.y = sectors[0].y;
		sectors.push_back(pt); //sector creation
	}
		/*
		for (int i = 0; i < sectors.size()-1; i++) {
			glColor3f(0.0,0.1*i,0.0);
			glRecti(sectors[i].x, sectors[i].y, sectors[i+1].x, sectors[i+1].y);
		}*/
	/*

	for(int w = data[lineNum].numD/2; w <data[lineNum].numD; w++)
	{
		data[lineNum].locPointD[w] = 1 - data[lineNum].locPointD[w];
	}
	*/
	//blue pts
	
	for(int k = 0; k < data[lineNum].numD; k++)
	{
		glPointSize(5.0f);
		glBegin(GL_POINTS);
			
			//glColor3f(0.0,0.0,1.0); //blue pts
			float r, g, b;
			
			int colorIndex = 0;
			while (classColors[colorIndex].className.compare(data[lineNum].className) != 0) {
				colorIndex++;
			}

			r = classColors[colorIndex].c.r;
			g = classColors[colorIndex].c.g;
			b = classColors[colorIndex].c.b;

			glColor3f(r, g, b); //set color

			float rad = (90-data[lineNum].locPointD[k]*(360/data[lineNum].numD)-(360/data[lineNum].numD*k))*DEG2RAD;
			glVertex2i(cos(rad)*radX + cx, sin(rad)*radY + cy);
			//matchPoints[k] = {((cos(rad)*radX + 500), (sin(rad)*radY + 500))};
			data[lineNum].setPoint((cos(rad)*radX + cx), (sin(rad)*radY + cy), k);
		glEnd();
	}

	if ((data[lineNum].numD / 2) % 2 == 1) //down ellipses
	{
		for (int i = 0; i < data[lineNum].numD / 2 - 1; i++) {
			DrawEllipseRight(i, lineNum);
		}
		for (int i = data[lineNum].numD / 2 - 1; i < data[lineNum].numD / 2 + 1; i++) {
			DrawEllipseDown(i, lineNum);
		}
		for (int i = data[lineNum].numD / 2 + 1; i < data[lineNum].numD; i++) {
			DrawEllipseLeft(i, lineNum);
		}
	}
	else //reg ellipses
	{

		for (int i = 0; i < data[lineNum].numD / 2; i++) {
			DrawEllipseRight(i, lineNum);
		}
		for (int i = data[lineNum].numD / 2; i < data[lineNum].numD; i++) {
			DrawEllipseLeft(i, lineNum);
		}
	}

	for(int i = 0; i < data[lineNum].numD-1; i+=2){
		drawIntercept(i, i+1, lineNum);
	}


	//line connecting intercepts
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i < data[lineNum].numD/2; i++){
		//glColor3ub(0, (mouseClicks*50)+1, 100);
		glVertex2i(data[lineNum].interceptPoints[i].x, data[lineNum].interceptPoints[i].y);
	}
	glEnd();
	
}

void DrawEllipse(int lineNum)
{
   int i;
   glColor3f(0.0,0.0,0.0);
   glBegin(GL_LINE_LOOP);
 
   for(i=90;i>-270;i--)
   {
      float rad = i*DEG2RAD;
      glVertex2f(cos(rad)*radX + cx,sin(rad)*radY + cy);
   } 
   glEnd();

   //x-axis
   glBegin(GL_LINE_STRIP);
		glVertex2i(cx, cy + radY + 100);
		glVertex2i(cx,cy - radY - 100);
	glEnd();
	elPer(lineNum);
}

bool isSector(int sectorNum, int mouseX, int mouseY)
{
	int x, y;
	if (mouseX > 750) { x = 1; }
	else { x = 0; }
	if (mouseY > 400) { y = 1; }
	else { y = 0; }
	
	if (x == 1) {
		if (y == 1) {
			if (((sectors[sectorNum].x <= mouseX) && (sectors[sectorNum].y >= mouseY)) && ((sectors[sectorNum + 1].x >= mouseX) && (sectors[sectorNum + 1].y <= mouseY))) {
				return true;
			}
		}
		else {
			if (((sectors[sectorNum].x >= mouseX) && (sectors[sectorNum].y >= mouseY)) && ((sectors[sectorNum + 1].x <= mouseX) && (sectors[sectorNum + 1].y <= mouseY))) {
				return true;
			}
		}
	}
	else {
		if (y == 1) {
			if (((sectors[sectorNum].x <= mouseX) && (sectors[sectorNum].y <= mouseY)) && ((sectors[sectorNum + 1].x >= mouseX) && (sectors[sectorNum + 1].y >= mouseY))) {
				return true;
			}
		}
		else {
			if (((sectors[sectorNum].x >= mouseX) && (sectors[sectorNum].y <= mouseY)) && ((sectors[sectorNum + 1].x <= mouseX) && (sectors[sectorNum + 1].y >= mouseY))) {
				return true;
			}
		}
	}
	return false;
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{

	glClear(GL_COLOR_BUFFER_BIT);
	
	GLint x = mouseX;
	GLint y = screenHeight - mouseY;
	int keyVal = theKey - '0';
	switch (theKey) {
	case 'a':
		if (animated == 1) {
			animated = 0;
		}
		else {
			animated = 1;
		}
		break;
	case 'h':
		if (hideEllipses == 1) {
			hideEllipses = 0;
		}
		else {
			hideEllipses = 1;
		}
		break;
	case 'r':
		//randomize color for each class
		randomizeColor(0);
		//drawInterceptR();
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		//int keyVal = theKey - '0';
		if(keyVal - 1 < classColors.size())
			randomizeColor(keyVal-1);
		break;
	case '0':
		if (10 <= classColors.size())
			randomizeColor(9);
		break;
	default:
		break;
	}
	for(int i = 0; i < lineCount; i++)
		DrawEllipse(i);
	glutPostRedisplay();
}


void myMouse(int button, int state, int mouseX, int mouseY)
{
	GLint x = mouseX;
	GLint y = screenHeight - mouseY;
	//glClear(GL_COLOR_BUFFER_BIT);
	if ((button == GLUT_LEFT_BUTTON) && state == GLUT_DOWN) //invert a dimension
	{
		mouseClicks++;
		int sec = 0;
		for (int i = 0; i < data[0].numD; i++){//sectors.size(); i++) {
			
			if(isSector(i, x, y)) {
				sec = i;
				//cout << "sector " << i << " WAS clicked" << endl;
				invert(sec);
			}
			else {
				//cout << "sector " << i << " was NOT clicked" << endl;
			}
		}
		//cout << "-----" << endl;
		/*for (int i = 0; i < lineCount; i++) {
			cout << "instance " << i << ": ";
			for (int j = 0; j < data[i].numD; j++) {
				cout << data[i].locPointD[j] << " ";
			}
			cout << endl;
		}

		cout << "-----" << endl;*/
		//GLint brushSize = 10;
		//glRecti(x, y, x + brushSize, y + brushSize);
		glClear(GL_COLOR_BUFFER_BIT);
		//glViewport(0, 0, screenWidth, screenHeight);
		for (int i = 0; i < lineCount; i++) {
			DrawEllipse(i);
		}
		glFlush();
		glutPostRedisplay();
	}
	static int swapSects[2];
	static int numSwapSectors = 0;
	//swap 2 dimensions
	if ((button == GLUT_RIGHT_BUTTON) && state == GLUT_DOWN) //invert a dimension
	{
		int sec = 0;
		for (int i = 0; i < data[0].numD; i++) {//sectors.size(); i++) {

			if (isSector(i, x, y)) {
				sec = i;
				cout << "sector " << i << " WAS clicked" << endl;
			}
			else {
				cout << "sector " << i << " was NOT clicked" << endl;
			}
		}
		//STORE SECTOR
		swapSects[numSwapSectors] = sec;
		numSwapSectors++;
		if (numSwapSectors == 2) {
			//SWAP SECTORS
			cout << "swapping " << swapSects[0] << " and " << swapSects[1] << endl;
			swap(swapSects[0], swapSects[1]);
			numSwapSectors = 0;

			for (int i = 0; i < lineCount; i++) {
				DrawEllipse(i);
			}
			glFlush();
			glutPostRedisplay();
		}

	}

	
		
}

void myDisplay(void)
{
	srand(static_cast <unsigned> (time(0)));
	if (dataRead == 0) {
		readEllipse();
		readValues();
		dataRead = 1;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	int lineNum = 0;
	//divided by classs
	/*glViewport(0, 0, screenWidth/2, screenHeight/2); //lower left
	//glViewport(0,0,screenWidth, screenHeight);
	for(int i = 0; i < lineCount && data[i].className == "Iris-setosa"; i++){
		DrawEllipse(i);
		lineNum++;
	}
	glViewport(screenWidth/2, 0, screenWidth/2, screenHeight/2); //lower right
	for(int i = lineNum; i < lineCount && data[i].className == "Iris-versicolor"; i++){
		DrawEllipse(i);
		lineNum++;
	}
	glViewport(0, screenHeight/2, screenWidth/2, screenHeight/2); //upper left
	for(int i = lineNum; i < lineCount && data[i].className == "Iris-virginica"; i++){
		DrawEllipse(i);
		lineNum++;
	}

	glViewport(screenWidth/2, screenHeight/2, screenWidth/2, screenHeight/2); //upper right*/
	glViewport(0, 0, screenWidth, screenHeight);
	for(int i = 0; i < lineCount; i++){
		DrawEllipse(i);
	}

	glFlush();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(10, 150);
	glutCreateWindow("Final Data Visualization");
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
}




