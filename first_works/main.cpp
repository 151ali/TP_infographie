#include <iostream>
#include"draw.h"


#define WIDTH 800
#define HEIGHT 800
#define BPP 24
#define pi 3.1415926535897932384626433832795028841971693993751058209749445923
using namespace std;
int main()
{
/*
    double Zbuffer[WIDTH][HEIGHT];
    for(int i=0;i<WIDTH;i++)
        for(int j=0;j<HEIGHT;j++)
            Zbuffer[i][j] = -INFINITY;

*/

    Screen s(WIDTH,HEIGHT);
    s.SetColor(255,255,255);

    lookAt cam(point(0, 0, 7), Vector(0, 0, -1), Vector(0, 1, 0));

	perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);


	//Sera appliquée à tous les sommets
	transformation tf;
	tf.compose(rotation_X(100))->compose(rotation_Y(150))->compose(rotation_Z(0))->compose(cam)->compose(persp)->compose(viewport);



    double pas = pi/5;
    int max_th  = 2*pi/pas+1,
        max_phi = pi/pas+1;
    point sph[max_th][max_phi];

    int R = 1;


    for(int i=0;i<max_th;i++){
        for(int j=0;j<max_phi;j++){

            sph[i][j].x = (double)(R * cos (i * pas) * sin(j * pas));
            sph[i][j].y = (double)(R * sin (i * pas) * sin(j * pas));
            sph[i][j].z = (double)(R * cos (j * pas));

            sph[i][j].transformer(tf);
        }
    }

    point p1,p2,p3,p4;
    for(int i=0;i<max_th;i++)
        for(int j=0;j<max_phi;j++){
            p1 = sph[i][j];
            p2 = sph[i][(j+1)%max_phi];
            p3 = sph[(i+1)%max_th][(j+1)%max_phi];
            p4 = sph[(i+1)%max_th][j];

            //s.Draw_LineDDA(p1,p2);
            //s.Draw_LineDDA(p2,p3);
            //s.Draw_LineDDA(p3,p4);
            //s.Draw_LineDDA(p4,p1);

        s.trace_triangle(p1,p2,p3,p1.nvect()*255,p2.nvect()*255,p3.nvect()*255);
		s.trace_triangle(p1,p3,p4,p1.nvect()*255,p3.nvect()*255,p4.nvect()*255);
        }


    return 0;
}
