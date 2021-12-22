/**
 * Creator : jishan
 * A base class for all XWindow like frame , button , inputbox , input
 */

#ifndef WINDOW_H
#define WINDOW_H

//define event type
#define GLOBAL_EVENT_TYPE 1
#define EXPOSE_EVENT_TYPE 2
#define LOCAL_EVENT_TYPE 3

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <functional>
#include <thread>

using namespace std;

class XWindow
{

public:
    XWindow(int width, int height);
    ~XWindow();
    static Display *display;
    int width = 0;
    int height = 0;
    Window currentWindow;
    //please override
    

private:
    static int enecals;
    
protected:
    static Window *focusedWindow; //only mouse click can make a XWindow focus [default]
    static int screen;
    GC currentGC;
    int supportedEventType = 0;
    friend class Graphics;
};

Display *XWindow::display = XOpenDisplay(NULL);
int XWindow::screen = DefaultScreen(XWindow::display);
int XWindow::enecals = 1;

XWindow::XWindow(int width, int height)
{
    this->width = width;
    this->height = height;
    if (this->display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }
}
XWindow::~XWindow()
{
    if (enecals == 1)
    {
        XCloseDisplay(this->display);
        enecals = 0;
    }
}

#endif