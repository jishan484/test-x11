#include "Frame.h"
#include "Button.h"
#include "Label.h"
#include <time.h>

void loop(Label currentTime,Label NextAlarm)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentHour = ltm->tm_hour;
    int currentMinute = ltm->tm_min;
    int currentSecond = ltm->tm_sec;

    currentTime.drawText("Current time is " + (currentHour < 10 ? "0" 
    + to_string(currentHour) : to_string(currentHour)) + ":" 
    + (currentMinute < 10 ? "0" + to_string(currentMinute) : to_string(currentMinute))
    + ":" + (currentSecond < 10 ? "0" + to_string(currentSecond) : to_string(currentSecond)));

    NextAlarm.drawText("object size : " + to_string(sizeof(currentTime)) +" Bytes");

    Frame::sleepms(1);
}

int main()
{
    Frame frame(600, 300);
    Label details(frame, 10, 30, 500);
    Label currentTime(frame, 10, 60, 500);
    Label nextalarm(frame, 10, 160,500);
    details.setFontColor(200, 0, 200);
    details.drawText("details of this alarm");
    currentTime.drawText("00:00:00");
    nextalarm.drawText("Next time is 00:00");

    frame.attach(details);
    frame.attach(currentTime);
    frame.attach(nextalarm);
    
    frame.runParallel(loop,currentTime,nextalarm);

    frame.show();
}