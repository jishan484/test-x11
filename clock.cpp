#include "Frame.h"
#include "Button.h"
#include "Label.h"
#include <time.h>
#include "GPIO.h"

#define FANPIN 26

bool stopNow = false;

int alarmHour = 0;
int alarmMinute = 0;

void startAlarm();

void loop(Frame *frame , Label * nextalarm,Label * currentTime)
{
    //get current time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentHour = ltm->tm_hour;
    int currentMinute = ltm->tm_min;
    int currentSecond = ltm->tm_sec;
    //set current time
    currentTime->drawText("Current time is " + (currentHour < 10 ? "0" + to_string(currentHour) : to_string(currentHour)) 
    + ":" + (currentMinute < 10 ? "0" + to_string(currentMinute) : to_string(currentMinute))
    + ":" + (currentSecond < 10 ? "0" + to_string(currentSecond) : to_string(currentSecond)));
    //get remaining time for alarm
    int remainingSeconds = (60 - currentSecond);
    int remainingMinutes = (alarmMinute > currentMinute ? (alarmMinute - currentMinute) : (60 - currentMinute + alarmMinute));
    //calculate remaining hours for alarm
    int remainingHours = 0;
    if(alarmHour > currentHour)
    {
        remainingHours = alarmHour - currentHour;
        if(currentMinute > alarmMinute) remainingHours -= 1;
    }
    else if(alarmHour == currentHour)
    {
        if(alarmMinute > currentMinute) remainingHours = 0;
        else remainingHours = 23;
    }
    else remainingHours = 23 - currentHour + alarmHour;
    
    //int remainingHours = (currentHour<alarmHour)?(alarmHour-currentHour):(currentHour == alarmHour && currentMinute < alarmMinute)?0:((currentHour - alarmHour) == 1 && currentMinute> alarmMinute)?0:(23-currentHour+alarmHour);
    //int remainingMinutes = (currentMinute <= alarmMinute)?(alarmMinute - currentMinute):(60 - currentMinute + alarmMinute);
    
    nextalarm->drawText("Next alarm in " + (remainingHours < 10 ? "0" + to_string(remainingHours) : to_string(remainingHours)) 
    + ":" + (remainingMinutes < 10 ? "0" + to_string(remainingMinutes) : to_string(remainingMinutes))
    + ":" + (remainingSeconds < 10 ? "0" + to_string(remainingSeconds) : to_string(remainingSeconds)));
    Frame::sleep(1);

    //if time is up
    if(remainingHours == 0 && remainingMinutes == 0){
        //alarm start
        startAlarm();
    }
}

int main()
{
    int hour = 0;
    int minute = 0;
    
    //setip GPIO
    gpiosetup();
    gpiosetout(FANPIN);
    gpiowrite(FANPIN, 0);

    Frame frame(600, 300);
    //define all elements
    Label hours(frame, 175, 30 , 50);
    Label minutes(frame, 265, 30 , 50);
    Label details(frame, 234, 130);
    Label currentTime(frame, 214, 150);
    Label nextalarm(frame, 220, 170);
    Button hoursP(150, 40, 70, 20, "H + 1");
    Button hoursN(150, 70, 70, 20, "H - 1");
    Button minutsP(240, 40, 70, 20, "M + 10");
    Button minutesN(240, 70, 70, 20, "M - 10");
    Button set(330, 40, 110, 50, "Set");
    Button stop(200, 200, 200, 70, "STOP ALARM");

    //set text for all elements
    hours.drawText("00");
    minutes.drawText("00");
    details.setFontColor(200, 0, 200);
    nextalarm.setFontColor(200, 100, 20);
    details.drawText("Alarm set at 00:00");
    currentTime.drawText("Current time is 00:00:00");
    nextalarm.drawText("Next alarm in 00:00");

    //set events for all elements [wow copilot]
    stop.onClick([&]() {
        stopNow = true;
        gpiowrite(FANPIN, 0);
    });
    hoursP.onClick([&]() {
        if (hour < 23) hour++;
        hours.drawText((hour < 10) ? "0" + to_string(hour) : to_string(hour));
    });
    hoursN.onClick([&]() {
        if (hour > 0) hour--;
        hours.drawText((hour < 10) ? "0" + to_string(hour) : to_string(hour));
    });
    minutsP.onClick([&]() {
        if (minute < 59) minute += 10;
        if(minute == 60) minute = 59;
        minutes.drawText((minute < 10) ? "0" + to_string(minute) : to_string(minute));
    });
    minutesN.onClick([&]() {
        if (minute > 9) minute -= 10;
        minutes.drawText((minute < 10) ? "0" + to_string(minute) : to_string(minute));
    });
    set.onClick([&]() {
        alarmHour = hour;
        alarmMinute = minute;
        details.drawText("Alarm set at "+ (hour < 10 ? "0" + to_string(hour) : to_string(hour)) + ":" + (minute < 10 ? "0" + to_string(minute) : to_string(minute)));
    });
    //attach all elements
    frame.attach(hours);
    frame.attach(minutes);
    frame.attach(details);
    frame.attach(currentTime);
    frame.attach(nextalarm);
    frame.attach(hoursP);
    frame.attach(hoursN);
    frame.attach(minutsP);
    frame.attach(minutesN);
    frame.attach(set);
    frame.attach(stop);
    frame.runParallel(loop,&frame, &nextalarm, &currentTime);
    frame.show();
}

void startAlarm()
{
    gpiowrite(FANPIN, 0);
    int i = 600;
    while (i-- > 0 || !stopNow)
    {
        printf("bip\n");
        gpiowrite(FANPIN, 1);
        usleep(100000);
        gpiowrite(FANPIN, 0);
        usleep(100000);
        gpiowrite(FANPIN, 1);
        usleep(100000);
        gpiowrite(FANPIN, 0);
        usleep(100000);
        gpiowrite(FANPIN, 1);
        usleep(100000);
        gpiowrite(FANPIN, 0);
        usleep(100000);
        gpiowrite(FANPIN, 1);
        usleep(400000);
        gpiowrite(FANPIN, 0);
        usleep(1000000);
    }
    gpiowrite(FANPIN, 0);
    stopNow = false;
}