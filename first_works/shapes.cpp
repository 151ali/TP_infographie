#include<cmath>
#include"draw.h"


void Screen::Draw_Quad(point p1,point p2,point p3,point p4){
    Draw_LineDDA(p1,p2);
    Draw_LineDDA(p2,p3);
    Draw_LineDDA(p3,p4);
    Draw_LineDDA(p4,p1);

}

void Screen::Draw_Triangle(point p1,point p2,point p3){
    Draw_LineDDA(p1,p2);
    Draw_LineDDA(p2,p3);
    Draw_LineDDA(p3,p1);
}
