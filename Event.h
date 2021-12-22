#ifndef EVENT_H
#define EVENT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "Window.h"
#include "Graphics.h"

class Events
{

public:
    
protected:
    Window window;
    Display *xdisplay;
    long listen_events = 0;
    void loadEvent(XEvent &e);
    void reloadEvent()
    {
        XSelectInput(this->xdisplay, this->window, listen_events);
    }
    void reloadEvent(int eventMask)
    {
        this->listen_events = this->listen_events | eventMask;
        XSelectInput(this->xdisplay, this->window, listen_events);
    }
    /** OVERRIDE ABLE FUNCTIONS **/
    virtual void onRightClick(XEvent &event) {}
    virtual void onLeftClick(XEvent &event) {}
    virtual void onMouseEnter(XEvent &event) {}
    virtual void onMouseLeave(XEvent &event) {}
    virtual void onKeyPress(XEvent &event) {}
    virtual void onKeyUp(XEvent &event) {}
};


void Events::loadEvent(XEvent &event){
    XFlush(this->xdisplay);
    if (this->window != event.xany.window)
        return;

    switch (event.type)
    {
    case ButtonPress:
        if (event.xbutton.button == 1) // left click
            this->onRightClick(event);
        else if(event.xbutton.button == 2)
            this->onLeftClick(event);
        break;
    case EnterNotify:
        this->onMouseEnter(event);
        break;
    case LeaveNotify:
        this->onMouseLeave(event);
        break;
    case KeyPress:
        this->onKeyPress(event);
        break;
    case KeyRelease:
        this->onKeyUp(event);
        break;
    default:
        break;
    }
}


















// class event_dispatcher [only for frame]

class Attachable : public Events, public XWindow
{
public:
    Attachable(int width, int height) : XWindow(width, height){}
protected:
    bool isRemoved = false;
private:
    virtual void show(XWindow *window) = 0;
    virtual void reDraw() = 0;
    virtual void remove() = 0;
    friend class Frame;
    friend class EventDispatcher;
};

class EventDispatcher
{
public:
private:
    int GEcounter = 0;
    int OEcounter = 0;
    int GCcounter = 0;
    Attachable *global_event_windows[20] = {0};     //like keyboard , mouse [selective by child]
    Attachable *on_expose_event_windows[100] = {0}; //expose [all window] [on expose we have to redraw everything]
    Graphics * graphics_context_element[1000] = {0};
protected:
    void addWindowGE(Attachable &window)
    {
        global_event_windows[GEcounter++] = &window;
    }
    void removeWindowGE(Attachable &window)
    {
        int i = 0;
        bool flag = false;
        while (i < this->GEcounter)
        {
            if (this->global_event_windows[i] == &window)
            {
                flag = true;
                break;
            }
            i++;
        }
        while (i < (this->GEcounter - 1))
        {
            this->global_event_windows[i] = this->global_event_windows[i + 1];
            i++;
        }
        if (flag)
            this->GEcounter--;
    }
    void addWindowOEE(Attachable &window)
    {
        on_expose_event_windows[OEcounter++] = &window;
    }
    void removeWindowOEE(Attachable &window)
    {
        int i = 0;
        while (i++ <= OEcounter)
        {
            if (on_expose_event_windows[i] == &window)
                break;
        }
        while (i++ <= OEcounter)
        {
            on_expose_event_windows[i] == on_expose_event_windows[i + 1];
        }
        if (i == OEcounter)
            OEcounter--;
    }
    void addGraphics(Graphics &gc){
        this->graphics_context_element[this->GCcounter++] = &gc;
    }
    void removeGraphics(Graphics &gc)
    {
        int i = 0;
        bool flag = false;
        while (i < this->GCcounter)
        {
            if (this->graphics_context_element[i] == &gc) {
                flag = true;
                break;
            }
            i++;
        }
        while (i < (this->GCcounter - 1))
        {
            this->graphics_context_element[i] = this->graphics_context_element[i + 1];
            i++;
        }
        if (flag) this->GCcounter--;
    }
    void eventLoop(XEvent &event);
    void targetedEvent(XEvent &event);
};

void EventDispatcher::eventLoop(XEvent &event)
{
        // printf("gc=%d  ge=%d oe=%d\n",GCcounter,GEcounter,OEcounter);

        int i = 0;
        while (i++ < OEcounter)
        {
            this->on_expose_event_windows[i - 1]->reDraw();
        }
        i = 0;
        while (i++ < GEcounter)
        {
            this->global_event_windows[i - 1]->reDraw();
        }
        i = 0;
        while (i < GCcounter)
        {
            this->graphics_context_element[i++]->reDraw();
        }
}
void EventDispatcher::targetedEvent(XEvent &event)
{
    int i = 0;
    while (i++ < GEcounter)
    {
        if (global_event_windows[i - 1]->currentWindow == event.xany.window)
            global_event_windows[i - 1]->loadEvent(event);
    }
}

#endif


/**
oct 15 
june -45 min --> 6
aug 1:15 --> 6
sept last -> 1:30
oct -> 1:45
6 times a day -- 1hr
2:30- 2:45 avg after 4 times a day 
*/
