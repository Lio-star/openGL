#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define sqrt2 (sqrt(2))
#define BACKTOWHIGHT true
#define WHITE_TO_BACK false
typedef struct point
{
    double x,y,z;
} Point;

bool drawAxes = true;
float tyreAngle = 0;

float quadAngle = 0;
float boxAngle = 0;
float boxScaleFactor = 1;
float tyreScaleFactor = 1;
float tinyBoxAngle = 0;
float background = 0;
bool background_transition=false;
bool background_transition_dir=BACKTOWHIGHT;
bool show_road=false;
bool show_grid=false;
Point pos, u, r, l;

typedef struct RGB_{
GLfloat r,g,b;
RGB_(float x,float y, float z){
this->r=x/255.0;
this->g=y/255.0;
this->b=z/255.0;
}
}RGB_;

typedef struct RGBA_{
GLfloat r,g,b,a;
RGBA_(float x,float y, float z,float a){
this->r=x/255.0;
this->g=y/255.0;
this->b=z/255.0;
this->a=a;
}
}RGBA_;

point distance(point a,point b)
{
    point temp;
    temp.x=b.x-a.x;
    temp.y=b.y-a.y;
    temp.z=b.z-a.z;
    float mag=sqrt(temp.x*temp.x+temp.y*temp.y+temp.z*temp.z);
    temp.x/=mag;
    temp.y/=mag;
    temp.z/=mag;
    return temp;

}
point addTo(point p,double amount)
{
    p.x+=amount;
    p.y+=amount;
    p.z+=amount;
    return p;
}
point multiTo(point p,double amount)
{
    p.x*=amount;
    p.y*=amount;
    p.z*=amount;
    return p;
}
point add(point a, point b)
{
    point temp;
    temp.x=b.x+a.x;
    temp.y=b.y+a.y;
    temp.z=b.z+a.z;
    return temp;
}

void displayAxes()
{
    if(drawAxes)
    {
        glBegin(GL_LINES);
        {
            glColor3f(1.0, 0, 0);

            glVertex3f( 500,0,0); // a point on pos x axis
            glVertex3f(-500,0,0); // a point on neg x axis

            glColor3f(0, 1.0, 0);

            glVertex3f(0,-500,0); // a point on pos y axis
            glVertex3f(0, 500,0); // a point on neg y axis

            glColor3f(0, 0, 1.0);

            glVertex3f(0,0, 500);  // a point on pos z axis
            glVertex3f(0,0,-500);   // a point on neg z axis
        }
        glEnd();
    }
}

void moveForward(float spreed=10)
{
    point temp;
    ///                    position vector + look vector
    temp = distance(pos, add(pos,l));
    temp=multiTo(temp,spreed);
    pos=add(pos,temp);

}
void moveRight(float spreed=10)
{

    point temp;
    temp = distance(pos, add(pos,r));
    temp=multiTo(temp,spreed);
    pos=add(pos,temp);
}
Point func(Point vect, Point perp, int dir)
{

    /// rotated two vector point by 1 degree and return unit vector or normalize vector

    double c = cos(pi/180);
    double s = dir * sin(pi/180);
    Point point;
    point.x = c * vect.x + s * perp.x;
    point.y = c * vect.y + s * perp.y;
    point.z = c * vect.z + s * perp.z;
    c = sqrt(point.x*point.x + point.y*point.y + point.z*point.z);
    point.x /= c;
    point.y /= c;
    point.z /= c;
    return point;
}

//void keyboardListener_(unsigned char key, int x,int y)
//{
//    float angless=1*pi/180.0;
//
//
//    switch(key)
//    {
//    case '1':
//    {
//        //cam  rotate left
//
//        Point l1 = func(l, r, -1);
//        r = func(r, l, 1);
//        l = l1;
//        break;
//    }
//    case '2':
//    {
//        //cam  rotate right
//        Point l1 = func(l, r, 1);
//        r = func(r, l, -1);
//        l = l1;
//        break;
//    }
//
//    case '3':
//    {
//        // cam rotate up
//        Point u1 = func(u, l, -1);
//        l = func(l, u, 1);
//        u = u1;
//        break;
//    }
//    case '4':
//    {
//        // cam rotate down
//        Point u1 = func(u, l, 1);
//        l = func(l, u, -1);
//        u = u1;
//        break;
//    }
//    case '5':
//    {
//        Point r1 = func(r, u, -1);
//        u = func(u, r, 1);
//        r = r1;
//        break;
//    }
//    case '6':
//    {
//        Point r1 = func(r, u, 1);
//        u = func(u, r, -1);
//        r = r1;
//        break;
//    }
//    case '7':
//    {
//        tyreScaleFactor += 0.2;
//        break;
//    }
//    case '8':
//    {
//        tyreScaleFactor -= 0.2;
//        break;
//    }
//    case 's':
//    {
//        moveForward(-10);
//        break;
//    }
//    case 'w':
//    {
//
//        moveForward();
//
//        break;
//    }
//    case 'a':
//    {
//        point temp;
//        temp.x=pos.x*cos(-angless)-sin(-angless)*pos.y;
//        temp.y=pos.x*sin(-angless)+cos(-angless)*pos.y;
//        temp.z=pos.z;
//        pos=temp;
//        break;
//    }
//    case 'd':
//    {
//        point temp;
//        temp.x=pos.x*cos(angless)-sin(angless)*pos.y;
//        temp.y=pos.x*sin(angless)+cos(angless)*pos.y;
//        temp.z=pos.z;
//        pos=temp;
//        break;
//    }
//    case 'k':
//    {
//
//        break;
//    }
//    default:
//        break;
//    }
//}






void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) // 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            //printf("Mouse Left Button Clicked\n");
            drawAxes=!drawAxes;
        }
        else if(state == GLUT_UP)
        {
            //printf("Mouse Left Button Released\n");
        }
        break;
    default:
        break;
    }
}

void displayTyre(float tyreRadius, float tyreWidth)
{
    int tyreSlices = 36;
    float sliceWidth = (2 * 3.1416 * tyreRadius) / tyreSlices;
//    float sliceWidth = (2 * M_PI * tyreRadius) / tyreSlices;
    float sliceAngle = 360 / tyreSlices;

    float halfTyreWidth = tyreWidth / 2, quartTyreWidth = halfTyreWidth / 2;
    float halfSliceWidth = sliceWidth / 2;


    glBegin(GL_QUADS);
    {
        glColor3d(228/255.0,37/255.0,37/255.0);
        glVertex3f(halfTyreWidth, tyreRadius, 0);
        glVertex3f(-halfTyreWidth, tyreRadius, 0);
        glVertex3f(-halfTyreWidth, -tyreRadius, 0);
        glColor3d(189/255.0,16/255.0,224/255.0);
        glVertex3f(halfTyreWidth, -tyreRadius, 0);
    }
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    {
        glColor3d(189/255.0,16/255.0,224/255.0);
        glVertex3f(halfTyreWidth, 0, tyreRadius);
        glVertex3f(halfTyreWidth, 0, -tyreRadius);

        glColor3d(228/255.0,37/255.0,37/255.0);
        glVertex3f(-halfTyreWidth, 0, -tyreRadius);
        glVertex3f(-halfTyreWidth, 0, tyreRadius);
    }
    glEnd();
    glColor3f(0.25, 0.5, 1);
    for(int i=0; i<tyreSlices; i++)
    {
        glPushMatrix();
        glRotatef(sliceAngle*i, 1, 0, 0);
        glTranslatef(0, tyreRadius, 0);
        glBegin(GL_QUADS);
        {

            glColor3d(16/255.0,163/255.0,163/255.0);
            glVertex3f(halfTyreWidth, 0, halfSliceWidth);
            glVertex3f(halfTyreWidth, 0, -halfSliceWidth);

            glColor3d(236/255.0,11/255.0,88/255.0);
            glVertex3f(-halfTyreWidth, 0, -halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0, halfSliceWidth);
        }
        glEnd();
        glPopMatrix();
    }
}

void displayQuad(float quadLength, float quadWidth) // width along x axis, length along z axis
{
    float halfQuadWidth = quadWidth, halfQuadLength = quadLength ;

    glBegin(GL_QUADS);
    {
        /** Solid Fill Quad **/



        RGB_ c={240,63,22};//orange color
        RGB_ yellow={255,212,2};//yellow color

        glColor3f(yellow.r, yellow.g, yellow.b);//yellow color
        glVertex3f(halfQuadLength, halfQuadWidth,0 );

        glColor3f(c.r,c.g,c.b);//orange color
        glVertex3f(halfQuadLength, -halfQuadWidth, 0);
        glColor3f(c.r,c.g,c.b);//orange color
        glVertex3f(-halfQuadLength, -halfQuadWidth, 0);

        glColor3f(yellow.r, yellow.g, yellow.b);//yellow color
        glVertex3f(-halfQuadLength, halfQuadWidth, 0);

        /** Gradient Fill Quad **/


//        glColor3f(1, 1, 0);
//
//        glVertex3f(halfQuadWidth, 0, halfQuadLength);
//        glVertex3f(halfQuadWidth, 0, -halfQuadLength);
//
//        glColor3f(0, 1, 1);
//
//        glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
//        glVertex3f(-halfQuadWidth, 0, halfQuadLength);

    }
    glEnd();
}

void displayBox(float boxLength, float boxWidth, float boxHeight,int colorlen,Point colors[],bool bidirectional=true)
 // length -> x axis, width -> yaxis, height -> z axis
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;


    glBegin(GL_QUADS);
    {
        //plane 0
        if(colorlen==0)
            glColor3f(1, 0, 0);
        else if(colorlen>=1)
            glColor3f(colors[0].x, colors[0].y, colors[0].z);
        else if(colorlen==-1)
            glColor3f(1, 1, 1);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        //plane 3
        if(colorlen==0)
            glColor3f(1, 0, 0);
        else if(colorlen>=4)
            glColor3f(colors[3].x, colors[3].y, colors[3].z);
        else if (colorlen>0 and bidirectional)
            glColor3f(colors[0].x, colors[0].y, colors[0].z);
        else
            glColor3f(1, 1, 1);
        if(colorlen==-1)
            glColor3f(1, 1, 1);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);



        //plane 1
        if(colorlen==0)
            glColor3f(0, 1, 0);
        else if(colorlen>=2)
            glColor3f(colors[1].x, colors[1].y, colors[1].z);
        else
            glColor3f(1, 1, 1);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        //plane 4
        if(colorlen==0)
            glColor3f(0, 1, 0);
        else if(colorlen>=5)
            glColor3f(colors[4].x, colors[4].y, colors[4].z);
        else if (colorlen>1 and bidirectional)
            glColor3f(colors[1].x, colors[1].y, colors[1].z);
        else
            glColor3f(1, 1, 1);
        if(colorlen==-1)
            glColor3f(1, 1, 1);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);


        //plane 2
        if(colorlen==0)
            glColor3f(0, 0, 1);
        else if(colorlen>=3)
            glColor3f(colors[2].x, colors[2].y, colors[2].z);
        else
            glColor3f(1, 1, 1);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glColor3f(0,1,1);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glColor3f(0,1,0);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        //plane 5
        if(colorlen==0)
            glColor3f(0, 0, 1);
        else if(colorlen>=6)
            glColor3f(colors[5].x, colors[5].y, colors[5].z);
        else if (colorlen>2 and bidirectional)
            glColor3f(colors[2].x, colors[2].y, colors[2].z);
        else
            glColor3f(1, 1, 1);

        if(colorlen==-1)
            glColor3f(1, 1, 1);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}

void displayBox(float boxLength, float boxWidth, float boxHeight)
{
    Point arr[0]= {};
    displayBox(boxLength,boxWidth,boxHeight,0,arr);

}
void displayBox(float boxLength, float boxWidth, float boxHeight,int colorslen)
{
    Point arr[0];
    if(colorslen==-1)
        displayBox(boxLength,boxWidth,boxHeight,-1,arr);

}

//void displayScene()
//{
//    //Primitive Object dimensions
//
//    float quadLength = 100, quadWidth = 60;
//    float largeBoxLength = 200, largeBoxWidth = 100, largeBoxHeight = 50;
//    float smallBoxLength = 150, smallBoxWidth = 70, smallBoxHeight = 30;
//    float tinyBoxLength = 10, tinyBoxWidth = 10, tinyBoxHeight = 10;
//    float tyreRadius = 100, tyreWidth = 40;
//
//
//    /** Principal Axes **/
//
//    displayAxes();
//
//    /**/
//
//    /** Quad **/
//
//    //glRotatef(quadAngle, 1, 0, 0); // rotates with respect to x axis
//    //glRotatef(quadAngle, 0, 1, 0); // rotates with respect to y axis
//    //glRotatef(quadAngle, 0, 0, 1); // rotates with respect to z axis
//    //displayQuad(quadLength, quadWidth);
//
//    /**/
//
//    /** Single Box **/
//
//    //glTranslatef(150, 0, 0); // rotation applied first, translation applied second
//    //glRotatef(boxAngle, 1, 0, 0); // rotates with respect to x axis
//    //glRotatef(boxAngle, 0, 1, 0); // rotates with respect to y axis
//    //glRotatef(boxAngle, 0, 0, 1); // roytates with respect to z axis
//    //glTranslatef(150, 0, 0);    // translation applied first, rotation applied second
//    //displayBox(largeBoxLength, largeBoxWidth, largeBoxHeight);
//
//    /**/
//
//    /** Double Box Stack **/
//
//    //glRotatef(boxAngle, 0, 0, 1); // rotation applied to both boxes
//    //glTranslatef(-300, 100, 100); // translation applied to both boxes
//    //displayBox(largeBoxLength, largeBoxWidth, largeBoxHeight); // large box
//    //glRotatef(boxAngle, 0, 0, 1);   // rotation applied only on the small box
//    //glTranslatef(0, 0, (smallBoxHeight/2) + (largeBoxHeight/2)); // places the small box on top of the large box
//    //displayBox(smallBoxLength, smallBoxWidth, smallBoxHeight); // small box
//
//    /**/
//
//    /** Double Box Stack - Push Pop Matrix **/
//
//
//    /*glRotatef(boxAngle, 0, 0, 1); // rotation applied to both boxes
//
//    //glPushMatrix(); // saves the matrix just before scaling, to prevent scale effect to anything other than the large box
//    glScalef(boxScaleFactor, boxScaleFactor, boxScaleFactor); // without push-pop, scale will be applied to both boxes
//    displayBox(largeBoxLength, largeBoxWidth, largeBoxHeight); // large box
//    //glPopMatrix();
//
//    glTranslatef(0, 0, (smallBoxHeight/2) + (largeBoxHeight/2)); // places the small box on top of the large box
//    displayBox(smallBoxLength, smallBoxWidth, smallBoxHeight); // small box
//    */
//
//
//    /**/
//
//    /** Tyre **/
//
//    /*glRotatef(tyreAngle, 1, 0, 0); // roates the tyre w.r.t x axis
//    glRotatef(tyreAngle, 0, 1, 0); // roates the tyre w.r.t y axis
//    glRotatef(tyreAngle, 0, 0, 1); // roates the tyre w.r.t z axis
//    displayTyre(tyreRadius, tyreWidth);
//    */
//    /**/
//
//    /** Tyre Double Box Stack **/
//
//    /*glPushMatrix();
//    glScalef(boxScaleFactor, boxScaleFactor, boxScaleFactor);
//    displayBox(largeBoxLength, largeBoxWidth, largeBoxHeight); // large box
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslatef(0, 0, (smallBoxHeight/2) + (largeBoxHeight/2)); // places the small box on top of the large box
//    displayBox(smallBoxLength, smallBoxWidth, smallBoxHeight); // small box
//    glPopMatrix();
//
//
//    glPushMatrix();
//    glTranslatef(0, 0, tyreRadius+smallBoxHeight+(largeBoxHeight/2)); // places the tyre on top of both boxes
//    glRotatef(tyreAngle, 1, 0, 0); // check what happens to the tyre after exchanging the order of translation and roation
//    glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
//    displayTyre(tyreRadius, tyreWidth);
//    glPopMatrix();
//    */
//
//    /** Animate Function **/
//    glPushMatrix();
//    glTranslated(0,0,20);
//    glRotated(90,0, 0,1);
//    glScalef(2, 2, 2);
//    displayBox(largeBoxLength, largeBoxWidth, largeBoxHeight); // large box
//    glPopMatrix();
//
//    glPushMatrix();
//    glTranslatef(0, 0, (smallBoxHeight/2) + (largeBoxHeight/2)); // places the small box on top of the large box
//    displayBox(smallBoxLength, smallBoxWidth, smallBoxHeight); // small box
//    glPopMatrix();
//
//
//    glPushMatrix();
//    glTranslatef(0, 0, tyreRadius+smallBoxHeight+(largeBoxHeight/2)); // places the tyre on top of both boxes
//    glRotatef(tyreAngle, 1, 0, 0); // check what happens to the tyre after exchanging the order of translation and roation
//    glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
//    displayTyre(tyreRadius, tyreWidth); // tyre
//    glPopMatrix();
//
//    glPushMatrix();
//    glRotatef(tinyBoxAngle, 0, 0, 1); // angle is updated in animate function
//    glTranslatef(250, 40, 50);
//    displayBox(tinyBoxLength, tinyBoxWidth, tinyBoxHeight);
//    glPopMatrix();
//}

float x=500;
float y=500;
void grid ()
{
    if(!show_grid)
        return;

    float stap_Size=60;

    float z=500;
    float w=500;
    float h=500;

    for(float i=-y; i<y; i+=stap_Size)
    {
        //x axis line
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            glVertex3f(x, i, 0.0f);
            glVertex3f(-x, i, 0.0f);
        glEnd();

        //z axis line
        glColor3f(0,0,1);
            glBegin(GL_LINES);
            glVertex3f(-x, i, 0.0);
            glVertex3f(-x, i, z);
        glEnd();
    }

    for(float i=-x; i<x; i+=stap_Size)
    {

        //y axis line
        glColor3f(0,1,0);
        glBegin(GL_LINES);

        glVertex3f(i, -y, 0.0);
        glVertex3f(i, y, 0.0);
        glEnd();
    }



}

void axis()
{
    if(!drawAxes)
        return ;

    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex3f(-1050, l.y+0.0f, 0);
    glVertex3f(l.x+1050.0f,  l.y+0.0f, 0);
    glEnd();

    glColor3f(0,1,0);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 1050.0f, 0);
    glVertex3f(0.0f, -1050.0f, 0.0);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, -1050);
    glVertex3f(0.0f, 0.0f, 1050.0);
    glEnd();

}

void display_road()
{
    double road_w=500;
    double road_h=1000;
    if (!show_road)
        return;
    Point colors[]=
    {
        {41/255.0,41/255.0,41/255.0},
        {41/255.0,41/255.0,41/255.0},
        {41/255.0,41/255.0,41/255.0},
    };
    displayBox(road_w,road_h,10,3,colors);
//left align
    glPushMatrix();
        glTranslated(road_w*0.4,0,1);
        displayBox(20,road_h,10,-1);
    glPopMatrix();
//right align
    glPushMatrix();
        glTranslated(-road_w*0.4,0,1);
        displayBox(20,road_h,10,-1);
    glPopMatrix();
}
class Car
{
public:
    float len,weight,height;
    float olen,oweight,oheight;
    float maxlen,maxweight,maxheight;
    Point origin_point;
    float angles=0.5;
    float light_Ray_rotated_angel=0;
    int move_direction=1;
    bool car_move=false;
    bool rotate_light=false;
    int rotate_light_dir=1;
    bool car_top_light=true;
    int expend_amount=10;

    Car()
    {
        olen=len=200;
        oweight=weight=500;
        oheight=height=50;

        maxlen=len+200;
        maxweight=weight+200;
        maxheight=height+50;
        origin_point= {0,0,100};
    }
    void expen_len(){
    float t=len+expend_amount;
    if(t<maxlen)
        len=t;

    }
    void shrink_len(){
    float t=len-expend_amount;
    if(t>=0)
        len=t;

    }

    void expen_weight(){
    float t=weight+expend_amount;
    if(t<maxweight)
        weight=t;
    }
    void shrink_weight(){
    float t=weight-expend_amount;
    if(t>=0)
        weight=t;

    }
    void expen_height(){
    float t=height+expend_amount;
    if(t<maxheight)
        height=t;

    }
    void shrink_height(){
    float t=height-expend_amount;
    if(t>=0)
        height=t;

    }
    void light_ray_roate()
    {

        light_Ray_rotated_angel+=(1.5*rotate_light_dir);
    }
    void set_rotate_light(int dir=1)
    {

        rotate_light_dir=dir;
        light_ray_roate();
    }

    void display()
    {

        ///lower body
        glPushMatrix();
            glTranslated(origin_point.x,origin_point.y,origin_point.z);
            //glRotated(90,0,0,1);/// rotation angle in x and y , freeze z;
            point _colors[]={
            {26/255.0,203/255.0,146/255.0},
            { 0,177/255.0,48/255.0},
            {26/255.0,108/255.0,203/255.0},
            {245/255.0,116/255.0,6/255.0},
            { 183/255.0,183/255.0,56/255.0},
            {72/255.0,63/255.0,5/255.0},
            };
            displayBox(len,weight,height,6,_colors,false);
        glPopMatrix();

        ///upper body
        glPushMatrix();
        glTranslated(origin_point.x,origin_point.y,origin_point.z+height);

        //glRotated(90,0,0,1);/// rotation angle in x and y , freeze z;

        upperbody(len,weight*0.40,height);
        glPopMatrix();


        front_light();

        tyre();
        tyre(1,-1);
        tyre(-1,1);
        tyre(-1,-1);

//top light box base
        glPushMatrix();
            glTranslated(origin_point.x,origin_point.y,origin_point.z+height+height*0.9);
            displayBox(len*0.05,weight*0.025,height*0.9);
            glTranslated(origin_point.x,origin_point.y,origin_point.z);
        glPopMatrix();

Point color[]= {{1,0,0}};//red color

glPushMatrix();
        //top light box

        Point p= {origin_point.x,origin_point.y,origin_point.z+height+height*1.5};
        glTranslated(p.x,p.y,p.z);
        glRotated(light_Ray_rotated_angel,0,0,1);

    glPushMatrix();
    //light box
        point top_light_pos= {len*0.20,weight*0.09,height*0.7};///setting size of light box
        if(car_top_light)
            displayBox(top_light_pos.x,top_light_pos.y,top_light_pos.z,1,color,false);
        else
            displayBox(top_light_pos.x,top_light_pos.y,top_light_pos.z,-1);

        double light_ray_w=200;


        if(car_top_light)
        {


            glPushMatrix();

            //ray bottom

            glRotated(-13,1,0,0);
            glTranslated(0,light_ray_w+14,-top_light_pos.z*0.28);

            displayQuad(3,light_ray_w);

            glPushMatrix();
            glTranslated(top_light_pos.x*0.3,-2,0);
            //glRotated(-90,0,1,0);
            displayQuad(3,light_ray_w);
            glPopMatrix();


            glPushMatrix();
            glTranslated(-top_light_pos.x*0.3,-2,0);
            //glRotated(-90,0,1,0);
            displayQuad(3,light_ray_w);
            glPopMatrix();


            glPopMatrix();

            //light ray top
            glPushMatrix();

            glRotated(13,1,0,0);

            glTranslated(0,light_ray_w+14,top_light_pos.z*0.28);

            displayQuad(3,light_ray_w);

            glPushMatrix();
            //glRotated(-5,0,0,1);
            glTranslated(top_light_pos.x*0.3,-2,0);
            displayQuad(3,light_ray_w);
            glPopMatrix();


            glPushMatrix();
            //glRotated(5,0,0,1);
            glTranslated(-top_light_pos.x*0.3,-2,0);
            displayQuad(3,light_ray_w);
            glPopMatrix();

            glPopMatrix();

            {
                //left
                glPushMatrix();

                //ray bottom
                glRotated(-20,0,0,1);

                glTranslated(top_light_pos.x*0.19,light_ray_w+14,0/*-top_light_pos.z*0.28*/);
                glPushMatrix();
                glRotated(-90,0,1,0);//rotate 90deg anticlock wise  in y
                displayQuad(3,light_ray_w);
                glPopMatrix();

                glPushMatrix();
                glTranslated(0,-2,top_light_pos.z*0.3);
                glRotated(-90,0,1,0);
                displayQuad(3,light_ray_w);
                glPopMatrix();


                glPushMatrix();
                glTranslated(0,-2,-top_light_pos.z*0.3);
                glRotated(-90,0,1,0);
                displayQuad(3,light_ray_w);
                glPopMatrix();


                glPopMatrix();
            }



//right side light rays
            {
                glPushMatrix();

                    //ray bottom
                    glRotated(20,0,0,1);

                    glTranslated(-top_light_pos.x*0.19,light_ray_w+14,0/*-top_light_pos.z*0.28*/);
                    glPushMatrix();
                    glRotated(-90,0,1,0);//rotate 90deg anticlock wise  in y
                    displayQuad(3,light_ray_w);
                    glPopMatrix();

                    glPushMatrix();
                    glTranslated(0,-2,top_light_pos.z*0.3);
                    glRotated(-90,0,1,0);
                    displayQuad(3,light_ray_w);
                    glPopMatrix();


                    glPushMatrix();
                    glTranslated(0,-2,-top_light_pos.z*0.3);
                    glRotated(-90,0,1,0);
                    displayQuad(3,light_ray_w);
                    glPopMatrix();


                glPopMatrix();
            }



        }

    glPopMatrix();//light ray outer pop matrix

glPopMatrix();//top box light pop matrix



    }
    void upperbody(double boxLength,double boxWidth,double boxHeight)
    {
        // length -> x axis, width -> yaxis, height -> z axis
        float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;
        double shift=50;

        glBegin(GL_QUADS);
        {
            RGB_ c={237,217,239};//white color
            RGB_ cc={ 91,8,68};//purple color

            glColor3d(cc.r,cc.g,cc.b);
            //for x axis part

            ///front x top 2
            glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
            glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
            ///front x bottom 2
            glColor3d(c.r,c.g,c.b);
            glVertex3f(-halfBoxLength, halfBoxWidth+weight*0.1, -halfBoxHeight);
            glVertex3f(halfBoxLength, halfBoxWidth+weight*0.1, -halfBoxHeight);

            //backward x
            glColor3d(cc.r,cc.g,cc.b);
            glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
            glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
            glVertex3f(-halfBoxLength, -halfBoxWidth-(weight*0.05), -halfBoxHeight);
            glVertex3f(halfBoxLength, -halfBoxWidth-weight*0.05, -halfBoxHeight);



            RGB_ blue_color={6,24,123};
            glColor3f(blue_color.r,blue_color.g,blue_color.b);

            //glColor3d(1,1,0.3);
            //for y axis part
            ///front TOP 2 point
            glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
            glColor3d(c.r,c.g,c.b);
            glVertex3f(halfBoxLength, halfBoxWidth+weight*0.1, -halfBoxHeight);
            ///front bottom 2 point
            glColor3f( 0,177/255.0,48/255.0);
            glVertex3f(halfBoxLength, -halfBoxWidth-weight*0.05, -halfBoxHeight);
            glColor3d(c.r,c.g,c.b);
            glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

            ///backward point
            glColor3f(blue_color.r,blue_color.g,blue_color.b);
            glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
            glColor3d(c.r,c.g,c.b);
            glVertex3f(-halfBoxLength, halfBoxWidth+weight*0.1, -halfBoxHeight);
            glColor3f( 183/255.0,183/255.0,56/255.0);
            glVertex3f(-halfBoxLength, -halfBoxWidth-weight*0.05, -halfBoxHeight);
            glColor3d(c.r,c.g,c.b);
            glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);


            glColor3d(0,0.1,0.3);
            //for z axis part

            glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
            glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
            glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
            glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

            glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
            glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
            glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
            glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        }
        glEnd();

    }
    void moves_towared_yaxis(double direction=1)
    {
        //car_move=true;
        move_direction=direction;
        moves();
    }
    void moves()
    {
        double dir_y = origin_point.y+(4.5*move_direction);
        if(dir_y>300)
            return;
        if(dir_y<-50)
            return;

        origin_point.y=dir_y;
        angles+=(5*move_direction*(-1));



    }



    void front_light()
    {

        glPushMatrix();

        glTranslated(origin_point.x+(len*0.4),origin_point.y+weight/2,origin_point.z+height*0.1);
        //l,w,h
        displayBox(len*0.1,weight*0.004,height*0.2,-1);//white color box
        glPopMatrix();

        glPushMatrix();

        glTranslated(-(origin_point.x+(len*0.4)),origin_point.y+weight/2,origin_point.z+height*0.1);
        //l,w,h
        displayBox(len*0.1,weight*0.004,height*0.2,-1);//white color box
        glPopMatrix();


    }
    void tyre(int l=1,int w=1)
    {
        glPushMatrix();
        glTranslated(origin_point.x+(len/2)*l,origin_point.y+(weight*0.4)*w,origin_point.z-(height*0.9));
        glRotated(angles,1,0,0);
        displayTyre(height*0.4,len*0.09);
        glPopMatrix();
    }
    void original_size(){
        len=olen;
        weight=oweight;
        height=oheight;
    }

};
/** declaring car obj */
Car testCar;



void keyboardListener(unsigned char key, int x,int y)
{
    float angless=1*pi/180.0;


    if(key=='1')
    {
        //cam  rotate left
        Point l1 = func(l, r, -1);
        r = func(r, l, 1);
        l = l1;
    }


    else if(key=='2')
    {

        //cam  rotate right
        Point l1 = func(l, r, 1);
        r = func(r, l, -1);
        l = l1;
    }

    else if (key=='3')
    {
        // cam rotate up
        Point u1 = func(u, l, -1);
        l = func(l, u, 1);
        u = u1;
    }
    else if (key=='4')
    {
        // cam rotate down
        Point u1 = func(u, l, 1);
        l = func(l, u, -1);
        u = u1;
    }

    else if (key=='5')
    {
        Point r1 = func(r, u, -1);
        u = func(u, r, 1);
        r = r1;

    }
    else if (key=='6')
    {
        Point r1 = func(r, u, 1);
        u = func(u, r, -1);
        r = r1;

    }
    else if (key=='7')
    {
        testCar.moves_towared_yaxis();

    }
    else if (key=='8')
    {
        testCar.moves_towared_yaxis(-1);

    }
    else if (key=='9')
        testCar.set_rotate_light();
    else if (key=='0')
        testCar.set_rotate_light(-1);
    else if (key=='s')
    {
        moveForward(-10);
    }
    else if (key=='w')
    {

        moveForward();
    }
//    else if (key=='a')
//    {
//        point temp;
//        temp.x=pos.x*cos(-angless)-sin(-angless)*pos.y;
//        temp.y=pos.x*sin(-angless)+cos(-angless)*pos.y;
//        temp.z=pos.z;
//        pos=temp;
//    }
//    else if (key=='d')
//    {
//        point temp;
//        temp.x=pos.x*cos(angless)-sin(angless)*pos.y;
//        temp.y=pos.x*sin(angless)+cos(angless)*pos.y;
//        temp.z=pos.z;
//        pos=temp;
//
//    }
    else if (key=='a' or key=='A')
    {
        testCar.expen_len();

    }
    else if (key=='b' or key=='B')
    {
        //testCar.len-=testCar.expend_amount;
        testCar.shrink_len();

    }
    else if (key=='c' or key=='C')
    {
        testCar.expen_weight();
        //testCar.weight+=testCar.expend_amount;

    }
    else if (key=='d' or key=='D')
    {
        //testCar.weight-=testCar.expend_amount;
        testCar.shrink_weight();

    }
    else if (key=='e' or key=='E')
    {
        testCar.expen_height();
      //  testCar.height+=testCar.expend_amount;

    }
    else if (key=='f' or key=='F')
    {
        testCar.shrink_height();
        //testCar.height-=testCar.expend_amount;

    }

}
void specialKeyListener(int key, int x,int y)
{
    if(key== GLUT_KEY_DOWN) // down arrow
    {
        //quadAngle += 5;
        //printf("Quad Angle %f\n", quadAngle);
        //cam move backward
        moveForward(-10);
    }

    else if(key==GLUT_KEY_UP)   // up arrow
    {
        quadAngle -= 5;
        //printf("Quad Angle %f\n", quadAngle);
        //cam move forward
        moveForward();
    }

    else if(key==GLUT_KEY_RIGHT)    // right arrow
    {
        //boxAngle += 5;
        //printf("Box Angle %f\n", boxAngle);

        ///cam move right

        moveRight();

    }

    else if(key== GLUT_KEY_LEFT)     // left arrow
    {
        boxAngle -= 5;
        //printf("Box Angle %f\n", boxAngle);
        //cam move left
        moveRight(-10);


        ///cam right rotating
//        float sin_theta=sin(1*pi/180.0);
//        float cos_theta=cos(1*pi/180.0);
//        point L_bar;
//        point r_bar;
//        L_bar.x=((l.x*cos_theta-r.x*sin_theta));
//        L_bar.y=((l.y*cos_theta-r.y*sin_theta));
//        L_bar.z=((l.z*cos_theta-r.z*sin_theta));
//
//        r_bar.x=((l.x*sin_theta+r.x*cos_theta));
//        r_bar.y=((l.y*sin_theta+r.y*cos_theta));
//        r_bar.z=((l.z*sin_theta+r.z*cos_theta));
//
//        l=L_bar;
//        r=r_bar;
    }

    else if(key== GLUT_KEY_PAGE_UP)
    {
        //boxScaleFactor += 0.2;
        //cam move up
        pos.z+=5;
       // printf("%f, %f , %f \n",pos.x,pos.y,pos.z);
    }

    else if(key== GLUT_KEY_PAGE_DOWN)
    {
        //boxScaleFactor -= 0.2;
        pos.z-=5;
    }

    else if(key==GLUT_KEY_HOME)
    {
        testCar.expen_height();
        testCar.expen_len();
        testCar.expen_weight();
    }

    else if(key==GLUT_KEY_END)
    {

        testCar.shrink_height();
        testCar.shrink_len();
        testCar.shrink_weight();
    }


}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background, background, background,0);	//sets background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // sets the current matrix to identity
    gluLookAt(pos.x, pos.y, pos.z, pos.x+ l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z); // sets camera position and direction

    /**       display               **/
    //displayScene();

    grid();

    axis();


    testCar.display();

    display_road();


    glutSwapBuffers();
}



void animate()
{

    if (background_transition==true)
    {
        if(background_transition_dir  )
        {

            background+=0.00001;
            if(background>=1)
                background_transition_dir=WHITE_TO_BACK;
        }
        else
        {
            background-=0.00001;
            if(background<=0)
                background_transition_dir=BACKTOWHIGHT;


        }


    }
    //testCar.moves();

    //testCar.light_ray_roate();



    glutPostRedisplay();
}

void init()
{

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(background, background, background,0); 	//sets background color
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity(); // sets the current matrix to identity
    gluPerspective(80,	1,	1,	2000.0);

    // to understand the following values, read the specification pdf
    double d = 1 / sqrt2;
    pos = {322.274352, 508.533571, 264.776856};
    u = {0, 0, 1};
    r = {-d, d, 0};
    l = {-d, -d, 0};
}

void subMenu(int id)
{
    if(id == 3)
    {
        background = 1;
    }
    else if(id == 4)
    {
        background = 0;
    }
    else if(id == 1)
    {
        background_transition=true;
    }
    else if(id == 2)
    {

        background_transition=false;
    }
    else if(id==5)
    {
        testCar.car_top_light=true;
        testCar.set_rotate_light();
    }
    else if(id==6)
    {
        testCar.car_top_light=false;


    }
    else if(id==7)
    {
        show_road=true;
    }
    else if(id==8)
    {
        show_road=false;

    }
}

void mainMenu(int id)
{
    if(id == 1)
    {
        drawAxes = true;
    }
    else if(id == 2)
    {
        drawAxes = false;
    }
    else if(id == 3)
    {
        show_grid=true;
    }
    else if(id == 4)
    {
        show_grid = false;
    }
    else if(id == 5)
    {
        testCar.original_size();
    }

}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("my GL project");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    int subMenuNo = glutCreateMenu(subMenu);
    glutAddMenuEntry("White", 3);
    glutAddMenuEntry("Black", 4);
    glutAddMenuEntry("Auto Transition on", 1);
    glutAddMenuEntry("Auto Transition off", 2);

    int subMenuLight = glutCreateMenu(subMenu);
    glutAddMenuEntry("Turn On", 5);
    glutAddMenuEntry("Turn Off", 6);

    int subMenuRoad = glutCreateMenu(subMenu);
    glutAddMenuEntry("Show", 7);
    glutAddMenuEntry("Hide", 8);



    glutCreateMenu(mainMenu);
    glutAddMenuEntry("Axes On", 1);
    glutAddMenuEntry("Axes Off", 2);
    glutAddMenuEntry("Grid On", 3);
    glutAddMenuEntry("Grid Off", 4);
    glutAddMenuEntry("Standard Dimension", 5);

    glutAddSubMenu("Light", subMenuLight);
    glutAddSubMenu("Road", subMenuRoad);
    glutAddSubMenu("Background", subMenuNo);
    glutAttachMenu(GLUT_RIGHT_BUTTON);


    glutMainLoop();		//The main loop of OpenGL, this function never returns

    return 0;
}
