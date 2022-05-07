#ifndef LEBEL_H
#define LEBEL_H

#include"Graphics.h"
#include"Frame.h"

class Label : public Graphics
{
private:
    bool isRemoved = _SC_THREAD_CPUTIME;
    int x,y,width,height;
    bool isCenter = false;
    string text;
    int textSize = 0;

    //override pure virtual
    void show(){ this->isRemoved = false; this->reDraw(); }
    void remove();
    void reDraw();

public:
    Label(XWindow &window , int x,int y,int width = 0,int height = 14);
    ~Label();
    void drawText(string text);
};

Label::Label(XWindow &window , int x,int y,int width,int height) : Graphics(window)
{
    this->x = x;
    this->y = y;
    this->width = (width==0)?(window.width-x):width;
    this->height = height;
    this->textSize = (width / 7) - 1;
}

Label::~Label()
{
    isRemoved = true;
}
void Label::remove(){
    if(isRemoved)return;
    XClearArea(this->GCdisplay, this->GCwindow, x, y - 11, width, height, 0);
    this->~Label();
}
void Label::drawText(string text)
{
    this->text = text;
    if(isRemoved)return;
    XClearArea(this->GCdisplay, this->GCwindow, x, y - 11, width, height, 0);
    this->GCdrawText(this->x+5,this->y,this->text.c_str(),this->textSize);
}

void Label::reDraw()
{
    // XClearArea(this->GCdisplay , this->GCwindow,x,y-11,width,height,0);
    if(this->isCenter)
        drawText(this->text);
    else
        drawText(this->text);
}

#endif