#include"draw.h"
#include<cmath>
#include"point.h"

#define WIDTH 800
#define HEIGHT 800
#define BPP 24
void cude(){
    Screen s(WIDTH,HEIGHT);

	lookAt cam(point(0, 0, 7), Vector(0, 0, -1), Vector(0, 1, 0));
	perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);

	//Sera appliquée à tous les sommets
	transformation Tf;
	Tf.compose(rotation_Z(10))->compose(rotation_X(20))->compose(rotation_Y(30))->compose(cam)->compose(persp)->compose(viewport);

	double v[8][3];
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

	int faces[6][4] = {  /* les indices des sommets des 6 faces du cube. */
		 { 0, 1, 2, 3 },
		 { 3, 2, 6, 7 },
		 { 7, 6, 5, 4 },
		 { 4, 5, 1, 0 },
		 { 5, 6, 2, 1 },
		 { 7, 4, 0, 3 }};
    double k;
	for (int i = 0; i < 6; i++) {

		point p1(&v[faces[i][0]][0]), p2(&v[faces[i][1]][0]), p3(&v[faces[i][2]][0]), p4(&v[faces[i][3]][0]);

		p1.transformer(Tf);
		p2.transformer(Tf);
		p3.transformer(Tf);
		p4.transformer(Tf);

        k = 255 / (i+1);
		//s.Draw_Quad(p1,p2,p3,p4);
		s.trace_triangle(p1,p2,p3,col(k,k,k),col(k,k,k),col(k,k,k));
		s.trace_triangle(p1,p3,p4,col(k,k,k),col(k,k,k),col(k,k,k));

	}
}


void rast(){
    Screen s(WIDTH,HEIGHT);
    s.SetColor(255,0,0);


	lookAt cam(point(0, 0, 7), Vector(0, 0, -1), Vector(0, 1, 0));
	perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);



    double largeur = WIDTH,
           hauteur = HEIGHT;

    s.Draw_LineDDA(point(20,20),point(300,300));
    s.Draw_LineDDA(point(300,300),point(150,300));
    s.Draw_LineDDA(point(150,300),point(20,20));

    point v0(largeur/2.0    , hauteur/4.0 );
    point v1(largeur / 4.0  , hauteur / 2 );
    point v2( largeur / 2.0  , 3.0 *hauteur / 4.0 );
    point v3( 3.0 *largeur/4.0  , hauteur / 2.0 );

    col c0(255, 0, 0 );
    col c1(0, 255, 0 );
    col c2(0, 0, 255 );

    s.trace_triangle(v0, v1, v2, c0, c1, c2);

    s.trace_triangle(v0, v2, v3, c0, c2, c0);


}

void org(){
    Screen s(800,800);
    s.SetColor(255,0,0);

    //s.Draw_Quad(point(0,0),point(100,0),point(100,100),point(0,100));

    //s.Draw_LineDDA(point(0,0,0),point(0,0,700));
    //s.Draw_LineDDA(point(0,0,0),point(0,7000,0));
    //s.Draw_LineDDA(point(0,0,0),point(700,0,0));



    lookAt cam(point(0, 0, 7), Vector(0, 0, 1), Vector(0, 1, 0));
    perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);

    transformation tf;


	tf.compose(rotation_Y(-45))->compose(cam)->compose(persp)->compose(viewport);
	point O(0,0,0);
	point X(200,0,0);
	point Y(0,200,0);
	point Z(0,0,200);

	point Op(0,0,0);
	point Xp(250,0,0);

    O.transformer(tf);
    X.transformer(tf);
    Y.transformer(tf);

    Op.transformer(tf);
    Xp.transformer(tf);


    s.Draw_LineDDA(O,X);
    s.SetColor(255,255,0);
    s.Draw_LineDDA(O,Y);
    s.SetColor(255,0,0);
    s.Draw_LineDDA(O,Z);
    s.SetColor(255,255,255);
    s.Draw_LineDDA(Op,Xp);
}

void pyramide(){
    Screen s(800,800);
    s.SetColor(255,0,0);

        double h= 500,
        b = 200,
        l = 250;

    int pas = 5;
    transformation tf_1;
    tf_1.compose(rotation_Z(45))->compose(rotation_X(-70))->compose(translation(Vector(200,200)));

    for(int z=0;z<=h;z+=pas){
            point p1(((h-z)/h) * b/2,  ((h-z)/h) * l/2,z);
            point p2(((h-z)/h) * b/2,  ((h-z)/h) * -l/2,z);
            point p3(((h-z)/h) * -b/2, ((h-z)/h) * -l/2,z);
            point p4(((h-z)/h) * -b/2, ((h-z)/h) * l/2,z);

            p1.transformer(tf_1);
            p2.transformer(tf_1);
            p3.transformer(tf_1);
            p4.transformer(tf_1);
            if(z == 0 ){
                point e(0,0,h);
                e.transformer(tf_1);
                s.Draw_LineDDA(p1,e);
                s.Draw_LineDDA(p2,e);
                s.Draw_LineDDA(p3,e);
                s.Draw_LineDDA(p4,e);
            }
            s.Draw_Quad(p1,p2,p3,p4);
        }
}

void cone(double h,int R){
    double pi = 3.1415926535897932384626433832795028841971693993751058209749445923;
    Screen s(800,800);
    s.SetColor(255,0,0);

    transformation tff;
    tff.compose(rotation_Z(45))->compose(rotation_X(-70))->compose(translation(Vector(600,250)));

    transformation rot;

    point sommet(0,0,h),base(0,R,0),O(0,0,0);
    sommet.transformer(tff);
    O.transformer(tff);
    for(double theta = 0;theta<2*pi;theta+=pi/50){
        point base(R*cos(theta),R*sin(theta),0);
        base.transformer(tff);
        s.Draw_Triangle(sommet,base,O);

    }





}




/*** sphere




    double pi =  3.1415926535897932384626433832795028841971693993751058209749445923;
    Screen s(WIDTH,HEIGHT);
    s.SetColor(255,255,0);

    lookAt cam(point(0, 0, 7), Vector(0, 0, -1), Vector(0, 1, 0));
	perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);


	//Sera appliquée à tous les sommets
	transformation tf;
	tf.compose(rotation_X(90))->compose(rotation_Y(0))->compose(rotation_Z(0))->compose(cam)->compose(persp)->compose(viewport);



    double pas = pi/100;
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

        //s.SetColor(255,255,255);
        //s.Draw_LineDDA(p1,p2);
        //s.Draw_LineDDA(p1,p4);
        s.trace_triangle(p1,p2,p3,              p1.vect()^Vector(255,255,255),p2.vect()^Vector(255,255,255),p3.vect()^Vector(255,255,255));
		s.trace_triangle(p1,p3,p4,      p1.vect()^Vector(255,255,255),p3.vect()^Vector(255,255,255),p4.vect()^Vector(255,255,255));
        //s.trace_triangle(p1,p2,p3,p1.vect()*255,p2.vect()*255,p3.vect()*255);
		//s.trace_triangle(p1,p3,p4,p1.vect()*255,p3.vect()*255,p4.vect()*255);
        }





****/
/**
Cube coloré






  Screen s(WIDTH,HEIGHT);

	lookAt cam(point(0, 0, 7), Vector(0, 0, -1), Vector(0, 1, 0));
	perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);

	//Sera appliquée à tous les sommets
	transformation Tf;
	Tf.compose(rotation_Z(0))->compose(rotation_X(0))->compose(rotation_Y(0))->compose(cam)->compose(persp)->compose(viewport);

	double v[8][3];
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

	int faces[6][4] = {  // les indices des sommets des 6 faces du cube.
		 { 0, 1, 2, 3 },
		 { 3, 2, 6, 7 },
		 { 7, 6, 5, 4 },
		 { 4, 5, 1, 0 },
		 { 5, 6, 2, 1 },
		 { 7, 4, 0, 3 }};


    	int cols[6][3] = {  // les indices des sommets des 6 faces du cube.
		 { 0, 0, 1},
		 { 0, 1, 0},
		 { 0, 1, 1},
		 { 1, 0, 0},
		 { 1, 0, 1},
		 { 1, 1, 1}};

    double k;
	for (int i = 0; i < 6; i++) {

        if(i==5) continue;

		point p1(&v[faces[i][0]][0]), p2(&v[faces[i][1]][0]), p3(&v[faces[i][2]][0]), p4(&v[faces[i][3]][0]);

		p1.transformer(Tf);
		p2.transformer(Tf);
		p3.transformer(Tf);
		p4.transformer(Tf);

		//s.Draw_Quad(p1,p2,p3,p4);
		Vector c(255 * cols[i][0],255 * cols[i][1],255 * cols[i][2]);




        ((p2-p1)^(p3-p1)).print();
        if(  ((p2-p1)^(p3-p1)).z < 0 ){
            s.trace_triangle(p1,p2,p3,c,c,c);
        }else{
            s.trace_triangle(p1,p3,p2,c,c,c);
        }


        if(  ((p3-p1)^(p4-p1)).z < 0  ){
            s.trace_triangle(p1,p3,p4,c,c,c);
        }else{
            s.trace_triangle(p1,p4,p3,c,c,c);
        }
	}

**/


/***


triangle rasti


    Screen s(WIDTH,HEIGHT);
    s.SetColor(255,255,255);
	lookAt cam(point(0, 0, 5), Vector(0, 0, -1), Vector(0, 1, 0));
	perspective persp(45, 1, -0.1, -100);
	viewport viewport(WIDTH, HEIGHT);

	//Sera appliquée à tous les sommets
	transformation Tf;
	Tf.compose(rotation_Z(0))->compose(rotation_X(0))->compose(rotation_Y(0))->compose(cam)->compose(persp)->compose(viewport);

	double r = 1;
	double x0=0,
           y0=0;
    point p1(x0+r*cos(90*3.14/180),y0+r*sin(90*3.14/180));
    point p2(x0+r*cos(-30*3.14/180),y0+r*sin(-30*3.14/180));
    point p3(x0+r*cos(210*3.14/180),y0+r*sin(210*3.14/180));



    p1.transformer(Tf);
    p2.transformer(Tf);
    p3.transformer(Tf);


    //s.Draw_LineDDA(p1,p2);
    //s.Draw_LineDDA(p2,p3);
    //s.Draw_LineDDA(p3,p1);



    s.trace_triangle(p1,p2,p3,col(255,0,0),col(0,255,0),col(0,0,255));



    */
