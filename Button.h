#ifndef Button_h
#define Button_h

#include "Event.h"

class Button : public Attachable
{
public:
    Button(int x, int y, int width, int height, string text);
    ~Button();
    template <typename Func, typename... Args>
    void onClick(Func func, Args &&...args)
    {
        onclick_callback = [func, args...]()
        {
            (func)(args...);
        };
    }

private:
    Window parent;
    int x , y;
    string text;
    // void (*onclick_callback)() = 0;
    std::function<void()> onclick_callback;
    void onMouseEnter(XEvent &event);
    void onMouseLeave(XEvent &event);
    void onRightClick(XEvent &event);
    void show(XWindow *window);
    //override pure virtual
    void reDraw();
    void remove();
};

Button::Button(int x, int y, int width, int height, string text) : Attachable(width, height)
{
    this->x = x;
    this->y = y;
    this->text = text;
    this->supportedEventType = 1;
}
Button::~Button()
{
    XDestroyWindow(this->display, this->currentWindow);
}

void Button::show(XWindow *window)
{
    this->parent = window->currentWindow;
    this->xdisplay = window->display;
    this->currentWindow = XCreateSimpleWindow(this->display, this->parent, x, y, width, height, 1, 0x122a6b, 0xd8e0f2);
    this->window = this->currentWindow;
    XMapWindow(this->display, this->currentWindow);
    XFlush(this->display);
    this->reDraw();
    // // // add some events for buttons
    this->reloadEvent(EnterWindowMask | LeaveWindowMask | ButtonPressMask);
    
}
void Button::reDraw()
{
    char buffer[50];
    strcpy(buffer, this->text.c_str());
    GC currentGC = XCreateGC(this->display, this->currentWindow,0, 0);
    //Font font = XLoadFont(this->display, "7*bold");
    XSetForeground(this->display, currentGC, 0x122a6b);
    //XSetFont(this->display,currentGC,font);
    XDrawString(this->display, this->currentWindow, currentGC, (this->width / 2) - (strlen(buffer) * 3), (this->height / 2) + 4, buffer, strlen(buffer));
    XFlush(this->display);
}
void Button::remove()
{
    if(this->isRemoved) return;
    XDestroyWindow(this->display, this->currentWindow);
    this->isRemoved = true;
}

//do it by event

void Button::onMouseEnter(XEvent &event)
{
    XSetWindowBackground(this->display, this->currentWindow, 0xadbad4);
    XClearWindow(this->display, this->currentWindow);
    this->reDraw();
}
void Button::onMouseLeave(XEvent &event)
{
    XSetWindowBackground(this->display, this->currentWindow, 0xd8e0f2);
    XClearWindow(this->display, this->currentWindow);
    this->reDraw();
}
void Button::onRightClick(XEvent &event)
{
    if(onclick_callback != 0){
        onclick_callback();
    }
}

#endif