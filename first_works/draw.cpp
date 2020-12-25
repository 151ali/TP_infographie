#include"draw.h"
#include<cmath>
#include "point.h"
Screen::Screen(int w=800,int h=800){
    FreeImage_Initialise();
    cout << FreeImage_GetVersion()<<endl;
    width  = w;
    height = h;
    int bpp = 24 ;
    bitmap = FreeImage_Allocate(width,height,bpp);
}

Screen::~Screen(){
    cout <<"~ Rendering ... "<<endl;
    FreeImage_Save(FIF_PNG , bitmap,"output.png",0);
    FreeImage_DeInitialise(); //CleanUp
}

void Screen::SetColor(double r=255,double g=255,double b=255){

    RGBQUAD col;
    col.rgbRed   = r;
    col.rgbGreen = g;
    col.rgbBlue  = b;

    color = col;

}

//TODO: A optimiser ...
int Screen::Draw_LineDDA(point p1, point p2) {

	point start_p;
	point end_p;


	if (p1.x <= p2.x)
	{
		start_p.x = p1.x;
		start_p.y = p1.y;

		end_p.x = p2.x;
		end_p.y = p2.y;

	}
	else {
		start_p.x = p2.x;
		start_p.y = p2.y;

		end_p.x = p1.x;
		end_p.y = p1.y;
	}

	//cout << "start ";  start_p.print();
	//cout << "end "; end_p.print();

	int delta_x = end_p.x - start_p.x;
	int delta_y = end_p.y - start_p.y;

	float m = (double)delta_y / (double)delta_x;


	FreeImage_SetPixelColor(bitmap, floor(start_p.x + 0.5), floor(start_p.y + 0.5), &color);

	if (delta_x == 0) {

		if (start_p.y < end_p.y)
			for (int y = start_p.y + 1; y < end_p.y; y++)
				FreeImage_SetPixelColor(bitmap, start_p.x, y, &color);
		else
			for (int y = end_p.y + 1; y < start_p.y; y++)
				FreeImage_SetPixelColor(bitmap, start_p.x, y, &color);

	}
	else if (delta_y == 0)
	{
		if (start_p.x < end_p.x)
			for (int x = start_p.x + 1; x < end_p.x; x++)
				FreeImage_SetPixelColor(bitmap, x, start_p.y, &color);
		else
			for (int x = end_p.x + 1; x < start_p.x; x++)
				FreeImage_SetPixelColor(bitmap, x, start_p.y, &color);


	}
	else if (abs(m) < 1) {

		//cout << "m < 1 " << endl;
		//cout << "m =  " << m << endl;

		float y = start_p.y;

		for (int x = start_p.x + 1; x < end_p.x; x++)
		{
			y = y + m;
			FreeImage_SetPixelColor(bitmap, x, floor(y + 0.5), &color);

		}
	}
	else {
		//cout << "m =  " << m << endl;
		float x = start_p.x;

		if (start_p.y < end_p.y)
			for (int y = start_p.y + 1; y < end_p.y; y++)
			{
				x = x + 1.0 / m;
				FreeImage_SetPixelColor(bitmap, floor(x + 0.5), y, &color);

			}
		else
			for (int y = start_p.y - 1; y > end_p.y; y--)
			{
				x = x - 1.0 / m;
				FreeImage_SetPixelColor(bitmap, floor(x + 0.5), y, &color);

			}

	}

	FreeImage_SetPixelColor(bitmap, floor(end_p.x + 0.5), floor(end_p.y + 0.5), &color);

	return 0;
}


void Screen::Draw_Circle(point center,int r){
    // TODO
}













void Screen::Draw_LineBRESENHAM(point p1,point p2){
    /// deltaX * dk = e
    /// thinking with `e` because deltaX > 0

    double  x1 = p1.x,
            y1 = p1.y,
            x2 = p2.x,
            y2 = p2.y;


    if(x1>x2 && y1>y2) {swap(x1,x2);swap(y1,y2);}



    double  dy = y2 - y1,
            dx = x2 - x1,
            y  = y1,
            e  = 2 * dy - dx,
            m  = dx / dy;


        for(int x=x1;x<=x2;x++){
            if(e < 0 ){
                /// keep the same y

                /// __ mise à jour
                e -= 2*dy;
            }else{
                /// update y
                y+=1;
                /// update e
                e = e + 2 * dy - 2 * dx;
            }
            FreeImage_SetPixelColor(this->bitmap,floor(x+0.5),floor(y+0.5),&this->color);
        }

}
