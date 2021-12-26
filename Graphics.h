#ifndef GRAPHICS_H
#define GRAPHICS_H

#include"Window.h"

class Graphics
{
private:
    XGCValues values;
protected:
    //object
    Window GCwindow;
    Display *GCdisplay;
    //config variables
    int fontSize = 7;
    int fontColor = 0x606060;
    //shape and text
    void GCdrawText(int x, int y, string text , int size = 0);
    void GCdrawRect(int x, int y, int width, int height, long color);
    void GCdrawRectFill(int x, int y, int width, int height, long color);
    // like that do for circle and lineremove
    virtual void show() = 0;
    virtual void reDraw() = 0;
    virtual void remove() = 0;

public:
    Graphics(XWindow &window);
    ~Graphics();
    GC currentGC;
    //implement
    void setFont(int size, bool bold);
    void setFontColor(int r, int g, int b); //convert r ,g ,b to long

    //graphics has two best friend to share private things (-_-)
    friend class Frame;
    friend class EventDispatcher;
};


Graphics::Graphics(XWindow &window)
{
    this->GCwindow = window.currentWindow;
    this->GCdisplay = window.display;
    // if(window.currentGC == NULL){
        values.foreground = 0x222222;
        values.background = 0xeeeeee;
        this->currentGC = XCreateGC(this->GCdisplay, this->GCwindow, GCBackground, &this->values);
        if (this->currentGC == 0)
        {
            fprintf(stderr, "Cannot create the graphics context\n");
            exit(1);
        }
        window.currentGC = this->currentGC;
        Font font = XLoadFont(this->GCdisplay, "7x13bold");
        XSetFont(this->GCdisplay, this->currentGC, font);
        XSetForeground(this->GCdisplay, this->currentGC, this->fontColor);
    // }
    // else{
    //     this->currentGC = window.currentGC;
    // }
}

void Graphics::GCdrawText(int x, int y, string text , int size)
{
    const char * charText = text.c_str();
    size = (size == 0 || size > strlen(charText))?strlen(charText):size;
    XDrawString(this->GCdisplay, this->GCwindow, this->currentGC, x, y, charText, size);
    XFlush(this->GCdisplay);
}

void Graphics::setFontColor(int r, int g, int b)
{
    this->fontColor = ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    XSetForeground(this->GCdisplay, this->currentGC, this->fontColor);
}
Graphics::~Graphics()
{

}


#endif