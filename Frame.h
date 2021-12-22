#ifndef FRAME_H
#define FRAME_H

#include "Window.h"
#include "Event.h"

class Frame : public XWindow, protected EventDispatcher
{
public:
    Frame(int width, int height);
    ~Frame();
    void attach(Attachable &window);
    void attach(Graphics &gc);
    void detach(Attachable &window);
    void detach(Graphics &gc);
    XEvent event;
    void close();
    void show();
    static bool isActive;
    void runParallel(std::function<void()> &f) { this->task = f; }
    void runParallel(const std::function<void()> &f) { this->task = f; }
    /** to sleep for n milis in parallel task thread*/
    static void sleepms(long milis = 100);
    /** to sleep for n seconds in parallel task thread*/
    static void sleep(int sec = 1){sleepms(sec * 1000);}

private:
    Atom WM_DELETE_WINDOW;
    void loop();
    thread * parallelTask = 0;
    thread * frameLoopTask = 0;
    std::function<void()> task;
//need a redesign here

protected:
    static Window *focusedWindow; //only mouse click can make a XWindow focus [default]
};
bool Frame::isActive = true;
Frame::Frame(int width, int height) : XWindow(width, height)
{
    this->currentWindow = XCreateSimpleWindow(this->display, RootWindow(this->display, this->screen), 0, 0, width, height, 1,
                                              CopyFromParent, WhitePixel(this->display, this->screen));
    XSelectInput(this->display, this->currentWindow, ExposureMask | KeyPressMask );
    XMapWindow(this->display, this->currentWindow);

    XStoreName(this->display, this->currentWindow, "NotePad++ Lite for pi zero");
    this->WM_DELETE_WINDOW = XInternAtom(this->display, "WM_DELETE_WINDOW", False);
    
    XSetWMProtocols(this->display, this->currentWindow, &WM_DELETE_WINDOW, 1);
    //just to settle things this need to wait for Expose event
    XEvent e;
    while (1)
    {
        XNextEvent(this->display, &e);
        if (e.type == Expose)
        {
            break;
        }
    }
}
Frame::~Frame()
{
    XDestroyWindow(this->display, this->currentWindow);
    free(this->parallelTask);
    free(this->frameLoopTask);
}
void Frame::show(){
    this->frameLoopTask = new thread([this]{loop();});
    this->parallelTask = new thread([this]{
        if(this->task == 0){ while(this->isActive)Frame::sleep(1);}
        while(this->isActive){
            this->task();
        }
    });    
    this->frameLoopTask->join();
    this->parallelTask->join();
}
void Frame::loop()
{
    while (this->isActive)
    {
        XNextEvent(this->display, &this->event);
        if (this->event.xany.window == this->currentWindow)
        {
            if ((this->event.type == ClientMessage) && (static_cast<unsigned int>(this->event.xclient.data.l[0]) == this->WM_DELETE_WINDOW))
            {
                this->isActive = false;
            }
            else if (event.type == Expose && event.xexpose.count == 1)
            {
                eventLoop(event);
            }
        }
        else
        {
            //dispatch event to selected or intented window:
            // keyboard event to selected only
            // mouse to intented only
            // Expose to all
            // rest intented only or frame only
            targetedEvent(event);
        }
    }
}

void Frame::attach(Attachable &window)
{
    window.show(this);
    if (window.supportedEventType == GLOBAL_EVENT_TYPE)
    {
        addWindowGE(window);
    }
    else
    {
        addWindowOEE(window);
    }
}
void Frame::attach(Graphics &gc)
{
    gc.show();
    addGraphics(gc);
}
void Frame::detach(Attachable &window)
{
    XSync(this->display, this->screen);
    if (window.supportedEventType == GLOBAL_EVENT_TYPE)
    {
        removeWindowGE(window);
        window.remove();
    }
    else
    {
        removeWindowOEE(window);
    }
}
void Frame::detach(Graphics &gc)
{
    removeGraphics(gc);
    gc.remove();
}

void Frame::close(){
    this->isActive = false;
}
void Frame::sleepms(long time){
    if(time < 100){ usleep(100000); return;}
    while(time > 1 && isActive){
        usleep(100000);
        time-=100;
    }
}

#endif