
#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED


#include<iostream>
#include<FreeImage.h>
#include<math.h>
#include"point.h"
#include<cmath>

typedef Vector col;

using namespace std;

class Screen{
    private:
        int width;
        int height;
        FIBITMAP *bitmap;
        RGBQUAD color;

    public:
        Screen(int,int);
        ~Screen();
        void SetColor(double,double,double);
        int  Draw_LineDDA(point,point);
        void Draw_Quad(point,point,point,point);
        void Draw_LineBRESENHAM(point p1,point p2);
        void Draw_Triangle(point p1,point p2,point p3);
        void Draw_Circle(point center,int r);

        double edgeFunction(const point &a,const point &b,const point &c){
            return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
        };

        void trace_triangle(point p0,point p1,point p2){

            double w0,w1,w2;
            point p;
            Vector e0,e1,e2;
            bool overlaps;

            double area = edgeFunction(p0,p1,p2);


            ((p2-p0)^(p1-p0)).print();

            if( ((p2-p0)^(p1-p0)).z <= 0){
                p2.Swap(p1);
             }


            for(int j=0;j<height;++j){
                for(int i=0;i<width;++i){
                    p=point (i + 0.5f , j + 0.5f);

                    w0 =edgeFunction(p1,p2,p);
                    w1 =edgeFunction(p2,p0,p);
                    w2 =edgeFunction(p0,p1,p);

                    e0 = Vector(p2.x - p1.x, p2.y - p1.y);
                    e1 = Vector(p0.x - p2.x, p0.y - p2.y);
                    e2 = Vector(p1.x - p0.x, p1.y - p0.y);

                    overlaps = true;


                    overlaps &= (w0 == 0 ? ((e0.y == 0 && e0.x > 0) || e0.y > 0) : (w0 > 0));
                    overlaps &= (w1 == 0 ? ((e1.y == 0 && e1.x > 0) || e1.y > 0) : (w1 > 0));
                    overlaps &= (w2 == 0 ? ((e2.y == 0 && e2.x > 0) || e2.y > 0) : (w2 > 0));

                    if(overlaps){
                    //if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                        FreeImage_SetPixelColor(bitmap,i,j,&color);

                    }

                }
            }
                    p2.Swap(p1);
        };


        void trace_triangle(point p0,point p1,point p2,const Vector &c0,const Vector &c1,const Vector &c2){
            double w0,w1,w2,r,g,b;
            point p;
            Vector e0,e1,e2;
            bool overlaps;
            RGBQUAD clr;

            double area = edgeFunction(p0,p1,p2);

            ((p2-p0)^(p1-p0)).print();

            if( ((p2-p0)^(p1-p0)).z <= 0){
                p2.Swap(p1);
             }

    int x_min = floor()min(min(p0.x,p1.x),p2.x);
    int y_min = min(min(p0.y,p1.y),p2.y);

    int x_max = max(max(p0.x,p1.x),p2.x);
    int y_max = max(max(p0.y,p1.y),p2.y);

            for(int i=0 ;i<width;++i){
                for(int j=0;j<height;++j){
                    p=point (i + 0.5f , j + 0.5f);

                    w0 =edgeFunction(p1,p2,p);
                    w1 =edgeFunction(p2,p0,p);
                    w2 =edgeFunction(p0,p1,p);

                    e0 = Vector(p2.x - p1.x, p2.y - p1.y);
                    e1 = Vector(p0.x - p2.x, p0.y - p2.y);
                    e2 = Vector(p1.x - p0.x, p1.y - p0.y);

                    overlaps = true;


                    overlaps &= (w0 == 0 ? ((e0.y == 0 && e0.x > 0) || e0.y > 0) : (w0 > 0));
                    overlaps &= (w1 == 0 ? ((e1.y == 0 && e1.x > 0) || e1.y > 0) : (w1 > 0));
                    overlaps &= (w2 == 0 ? ((e2.y == 0 && e2.x > 0) || e2.y > 0) : (w2 > 0));

                    if(overlaps){
                    //if (w0 >= 0 && w1 >= 0 && w2 >= 0) {


                        r = (w0 * c0.x + w1 * c1.x + w2 * c2.x)/(w0+w1+w2);
                        g = (w0 * c0.y + w1 * c1.y + w2 * c2.y)/(w0+w1+w2);
                        b = (w0 * c0.z + w1 * c1.z + w2 * c2.z)/(w0+w1+w2);

                        clr.rgbRed   = (unsigned char) abs(r);
                        clr.rgbGreen = (unsigned char) abs(g);
                        clr.rgbBlue  = (unsigned char) abs(b);

                        //cout << "r :"<<r<< ", g : "<< g <<" , b : " <<b<<endl;
                        FreeImage_SetPixelColor(bitmap,i,j,&clr);

                    }

                }
            }

        p2.Swap(p1);
        };


};


#endif // DRAW_H_INCLUDED
