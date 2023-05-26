#include "ekran.h"
#include <QPainter>
#include <QMouseEvent>
#include <cstdio>
#include <cstdlib>
#include <cmath>

Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    im=QImage(1000, 1000, QImage::Format_RGB32);
    im.fill(0);
}

void Ekran::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0,0,im);
}

void Ekran::mouseMoveEvent(QMouseEvent *e)
{
    p2.x=e->pos().x();
    p2.y=e->pos().y();
    if(id!=-1)
    {
        lista[id].x=p2.x;
        lista[id].y=p2.y;
        rysujKrzywa();
    }
}

void Ekran::mousePressEvent(QMouseEvent *e)
{
    id=-1;
    int dl=lista.length()-1;
    p1.x=e->pos().x();
    p1.y=e->pos().y();
    if(e->button()==Qt::RightButton)
    {
        lista.push_back(p1);
        rysujKrzywa();
    }
    else if(e->button()==Qt::MiddleButton)
    {
        for(int i=0;i<=dl;i++)
        {
            if(sqrt(pow((p1.x-lista[i].x),2)+pow((p1.y-lista[i].y),2))<=8)
            {
                lista.erase(lista.begin()+i);
                rysujKrzywa();
                break;
            }
        }
    }
    else if(e->button()==Qt::LeftButton)
    {
        for(int i=0;i<=dl;i++)
        {
            if(sqrt(pow((p1.x-lista[i].x),2)+pow((p1.y-lista[i].y),2))<=8)
            {
                id=i;
                break;
            }
        }
    }
}

void Ekran::drawPunkt(int x,int y,int r,int g,int b)
{
    for(int i=y-2;i<=y+2;i++)
    {
        for(int j=x-2;j<=x+2;j++)
        {
            if((j<im.width()&&j>=0)&&(i<im.height()&&i>=0))
            {
                uchar *pix=im.scanLine(i)+4*j;
                pix[0]=b;
                pix[1]=g;
                pix[2]=r;
            }
        }
    }


}

void Ekran::drawPixel(int x,int y,int r,int g,int b)
{
    if((x<im.width()&&x>=0)&&(y<im.height()&&y>=0))
    {
        uchar *pix=im.scanLine(y)+4*x;
        pix[0]=b;
        pix[1]=g;
        pix[2]=r;
    }
}

void Ekran::linia(int x1,int y1,int x2,int y2,int r,int g,int b)
{
    if(abs(y2-y1)<=abs(x2-x1))
        {
            float y=y1;
            float a=(float)(y2-y1)/(x2-x1);
            if(x1<x2)
            {
                for(int i=x1;i<=x2;i++)
                {
                    drawPixel(i+0.5,y+0.5,r,g,b);
                    y+=a;
                }
            }
            else
            {
                for(int i=x1;i>=x2;i--)
                {
                    drawPixel(i+0.5,y+0.5,r,g,b);
                    y-=a;
                }
            }

        }
        else
        {
            float x=x1;
            float a=(float)(x1-x2)/(y1-y2);
            if(y1<y2)
            {
                for(int i=y1;i<=y2;i++)
                {
                    drawPixel(x+0.5,i+0.5,r,g,b);
                    x+=a;
                }
            }
            else
            {
                for(int i=y1;i>=y2;i--)
                {
                    drawPixel(x+0.5,i+0.5,r,g,b);
                    x-=a;
                }
            }
        }
}

void Ekran::rysujKrzywa()
{
    int dl=lista.length()-1;
    int reszta;
    if(dl<=4)
    {
         reszta=(dl+1)%4;
    }
    else
    {
        reszta=(dl+1-4)%3;
    }
    dl=dl-reszta;
    double t;
    int x,y,xold,yold,xtemp,ytemp;
    bool p=true;
    im.fill(0);
    if(dl+reszta>=3)
    {
            drawPunkt(lista[0].x,lista[0].y,0,0,255);
            drawPunkt(lista[1].x,lista[1].y,102,102,255);
            linia(lista[0].x,lista[0].y,lista[1].x,lista[1].y,128,128,128);
            drawPunkt(lista[2].x,lista[2].y,102,102,255);
            drawPunkt(lista[3].x,lista[3].y,0,0,255);
            linia(lista[2].x,lista[2].y,lista[3].x,lista[3].y,128,128,128);
            for(int i=4;i<=dl;i=i+3)
            {
                drawPunkt(lista[i].x,lista[i].y,102,102,255);
                linia(lista[i].x,lista[i].y,lista[i-1].x,lista[i-1].y,128,128,128);
                drawPunkt(lista[i+1].x,lista[i+1].y,102,102,255);
                drawPunkt(lista[i+2].x,lista[i+2].y,0,0,255);
                linia(lista[i+1].x,lista[i+1].y,lista[i+2].x,lista[i+2].y,128,128,128);
            }

    }
    for(int i=3;i<=dl;i=i+3)
    {
        t=0;
        if(p==true)
        {
            p=false;
            xold=pow((1-t),3)*lista[i-3].x+3*pow((1-t),2)*t*lista[i-2].x+3*(1-t)*t*t*lista[i-1].x+pow(t,3)*lista[i].x;
            yold=pow((1-t),3)*lista[i-3].y+3*pow((1-t),2)*t*lista[i-2].y+3*(1-t)*t*t*lista[i-1].y+pow(t,3)*lista[i].y;
        }
        else
        {
            xtemp=pow((1-t),3)*lista[i-3].x+3*pow((1-t),2)*t*lista[i-2].x+3*(1-t)*t*t*lista[i-1].x+pow(t,3)*lista[i].x;
            ytemp=pow((1-t),3)*lista[i-3].y+3*pow((1-t),2)*t*lista[i-2].y+3*(1-t)*t*t*lista[i-1].y+pow(t,3)*lista[i].y;
            linia(xtemp,ytemp,xold,yold,255,255,255);
            xold=xtemp;
            yold=ytemp;
        }
        t=t+0.001;
    while(t<=1)
    {
        x=pow((1-t),3)*lista[i-3].x+3*pow((1-t),2)*t*lista[i-2].x+3*(1-t)*t*t*lista[i-1].x+pow(t,3)*lista[i].x;
        y=pow((1-t),3)*lista[i-3].y+3*pow((1-t),2)*t*lista[i-2].y+3*(1-t)*t*t*lista[i-1].y+pow(t,3)*lista[i].y;
        t=t+0.001;
        linia(xold,yold,x,y,255,255,255);
        xold=x;
        yold=y;

    }
    }
    for(int i=dl+reszta;i>dl;i--)
    {
        if(i<0)
        {
            break;
        }
        drawPunkt(lista[i].x,lista[i].y,255,255,0);
    }
    update();
}
