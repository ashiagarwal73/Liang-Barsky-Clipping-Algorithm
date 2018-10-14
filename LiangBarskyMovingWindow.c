#include<stdio.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
int ww = 640, wh = 480;  
int first = 0;  
int xi=0, yi=0, xf=0, yf=0; 

int xmin,ymin,xmax,ymax;
double p[4],q[4];
int xt1;
int yt1;
int xt2;
int yt2;
void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
    // Stroing start time 
    clock_t start_time = clock(); 
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) 
        ; 
} 
/* Function to plot a point */
void setPixel(GLint x, GLint y) {
  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();
}
int X1, Y1, X2, Y2; 
void *font = GLUT_BITMAP_TIMES_ROMAN_10;
void *fonts[] = 
{
      GLUT_BITMAP_9_BY_15,
      GLUT_BITMAP_TIMES_ROMAN_10,
      GLUT_BITMAP_TIMES_ROMAN_24
};
//displaying text
void show(int x, int y, char *string)
{
      int len, i;
      glRasterPos2f(x, y);
      len = (int) strlen(string);
      for (i = 0; i < len; i++)
            glutBitmapCharacter(font, string[i]);
}
/* Bresenham's Algorithm */
void Bresenham(int x1, int y1, int x2, int y2)
{
  int dx,dy,Po;
  int k=0;
  dx=(x2-x1);
  dy=(y2-y1);
  if(dy<=dx&&dy>0)
	  {
	  dx=abs(dx);
	  dy=abs(dy);
 	  Po=(2*dy)-dx;
	  setPixel(x1,y1);
	  int xk=x1;
	  int yk=y1;
	  for(k=x1;k<x2;k++)
	  { 
	    if(Po<0)
		{	
			setPixel(++xk,yk);
			Po=Po+(2*dy);
		}
	    else{
			setPixel(++xk,++yk);
			Po=Po+(2*dy)-(2*dx);
		}
	  }
	}
  else if(dy>dx&&dy>0)
  {
	  dx=abs(dx);
	  dy=abs(dy);
	  Po=(2*dx)-dy;
	  setPixel(x1,y1);
	  int xk=x1; int yk=y1;
	  for(k=y1;k<y2;k++)
	  { 
	    if(Po<0)
		{	
			setPixel(xk,++yk);
			Po=Po+(2*dx);
		}
	    else{
			setPixel(++xk,++yk);
			Po=Po+(2*dx)-(2*dy);
		}
	  }			
	}
  else if(dy>=-dx)
	 {
	  dx=abs(dx);
	  dy=abs(dy);
          Po=(2*dy)-dx;
	  setPixel(x1,y1);
	  int xk=x1;
	  int yk=y1;
	  for(k=x1;k<x2;k++)
	  { 
	    if(Po<0)
		{	
			setPixel(++xk,yk);
			Po=Po+(2*dy);
		}
	    else{
			setPixel(++xk,--yk);
			Po=Po+(2*dy)-(2*dx);
		}
	  }
         }
  else if(dy<-dx)
	{
	  dx=abs(dx);
	  dy=abs(dy);
 	  Po=(2*dx)-dy;
	  setPixel(x1,y1);
	  int xk=x1;
	  int yk=y1;
	  for(k=y1;k>y2;k--)
	  { 
	    if(Po<0)
		{	
			setPixel(xk,--yk);
			Po=Po+(2*dx);
		}
	    else{
			setPixel(++xk,--yk);
			Po=Po+(2*dx)-(2*dy);
		}
	  }
	}
  glFlush();
}
int clippingAlgo(int x1,int y1,int x2,int y2)
{
  float t1=0.0,t2=1.0;
  int dx=x2-x1;
  int dy=y2-y1;
    int i;
    float t;
    p[0]=-dx;
    q[0]=x1-xmin;
    p[1]=dx;
    q[1]=xmax-x1;
    p[2]=-dy;
    q[2]=y1-ymin;
    p[3]=dy;
    q[3]=ymax-y1;
    for(i=0;i<4;i++)
    {
        if(p[i]==0 && q[i]<0)//line is parallel and outside
            return 0;
        if(p[i]<0.0)
        {
			     t=q[i]/p[i];
            if(t>t1)
                {
                  t1=t;
                }
        }
        if(p[i]>0.0)
        {  
			      t=q[i]/p[i];
            if(t<t2)
                {t2=t;}
        }
    }
    if(t1<t2)
    {  
         xt1=x1+t1*(x2-x1);
         yt1=y1+t1*(y2-y1);
         xt2=x1+t2*(x2-x1);
         yt2=y1+t2*(y2-y1);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        Bresenham( xmin, ymin, xmax, ymin);
        Bresenham( xmax, ymin, xmax, ymax);
        Bresenham( xmin, ymax,xmax, ymax);
        Bresenham( xmin, ymax, xmin, ymin);
        int i;
	    //draw axis 
	      for(i=-(ww/2);i<(ww/2);i=i+10)
	      {
	      setPixel(i,0);
	      setPixel(0,i);
	      } 
	 glColor3f(0.0,1.0,0.0);
     Bresenham( xt1, yt1, xt2, yt2);
     glColor3f(1.0,0.0,0.0);
     Bresenham( xi, yi, xt1, yt1);
     glColor3f(1.0,0.0,0.0);
	 Bresenham( xt2, yt2, xf, yf);
     return 1;
    }
    else{
      return 0;
    }
}
// moving window searching for line to clip
void radomWindow()
{
  int flag=0;
  while(flag==0)
  {
    int randomX=(rand()%300)-150;
    int randomY=(rand()%300)-150;
    if(randomX<270&&randomX>-320)
    {

        if(randomY<190&&randomY>-240)
        {
          glClear(GL_COLOR_BUFFER_BIT);
          glColor3f(1.0,1.0,1.0);
          glFlush();
          Bresenham( xi, yi, xf, yf);
          xmin=randomX;
          xmax=randomX+100;
          ymin=randomY;
          ymax=randomY+100;
          Bresenham( xmin, ymin, xmax, ymin);
          Bresenham( xmax, ymin, xmax, ymax);
          Bresenham( xmin, ymax,xmax, ymax);
          Bresenham( xmin, ymax, xmin, ymin);
          int x=clippingAlgo(xi,yi,xf,yf);
         
          delay(2000);
        }
    }
  }
}
//when key pressed for Clipping window
void keyboardkey(unsigned char key,int x,int y)
{

    glClear(GL_COLOR_BUFFER_BIT);
    if(key=='c')
    {
        radomWindow();
    }
}
//Initial display method
void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  int i;
  //draw axis	
  for(i=-(ww/2);i<(ww/2);i=i+10)
  {
 	setPixel(i,0);
 	setPixel(0,i);
  } 
    //WRITING AXIS NAMES
    show(-(ww/2),0,"- X-AXIS");
    show((wh/2)+30,0,"X-AXIS");
    show(0,(wh/2)-10,"Y-AXIS");
    show(0,-(wh/2)+5,"- Y-AXIS");    
    glutSwapBuffers();
  glFlush();
}
//when mouse click for points on screen
void mouse(int btn, int state, int x, int y) 
{
	
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
        { 
        switch(first)  
        {  
        case 0:  
        xi = x-(ww/2);  
        yi = (wh/2)-y;  
        first = 1;  
        break;  
        case 1:  
        xf = x-(ww/2);  
        yf = (wh/2)-y;
		if(xi>xf)
		{
		  int temp=xi;
	 	  xi=xf;
		  xf=temp;  
		  temp=yi;
	 	  yi=yf;
		  yf=temp;  
		} 
		show(-300,200,"Press C to start clipping");
		Bresenham( xi, yi, xf, yf);
        first = 2;
        break;
       }  
    }  
}  
void init() 
{ 
  glViewport(-(ww/2),0,(ww/2),0); 
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowPosition(0,0);
  glutInitWindowSize(ww, wh);
  glutCreateWindow("Liang-Barsky Algorithm");
  glClearColor(0.0,0.0,0.0,0);
  glColor3f(1.0,1.0,1.0);
  gluOrtho2D(-(ww/2),(ww/2),-(wh/2),(wh/2)); 
}
int main(int argc, char **argv) 
{
  glutInit(&argc, argv);
  init();
  glutDisplayFunc(draw);
  glutMouseFunc(mouse); 
  glutKeyboardFunc(keyboardkey); 
  glutMainLoop();
  return 0;
}