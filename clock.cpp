#include "Frame.h"
#include "Button.h"
#include "Label.h"
#include <time.h>
#include "GPIO.h"

#define FANPIN 26

int alarmHour = 0;
int alarmMinute = 0;

void startAlarm();

void loop(Frame *frame , Label * nextalarm)
{
    //get current time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentHour = ltm->tm_hour;
    int currentMinute = ltm->tm_min;
    int currentSecond = ltm->tm_sec;
    //remaining time for alarm
    int remainingHours = (currentHour < alarmHour)?(alarmHour - currentHour):((currentHour == alarmHour && currentMinute < alarmMinute)? 0:(23 - currentHour + alarmHour));
    int remainingMinutes = (currentMinute <= alarmMinute)?(alarmMinute - currentMinute):(60 - currentMinute + alarmMinute);
    int remainingSeconds = (60 - currentSecond) % 60;
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
    Label details(frame, 234, 150);
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
    nextalarm.drawText("Next alarm in 00:00");

    //set events for all elements [wow copilot]
    stop.onClick([&]() {
        frame.close();
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
    frame.attach(nextalarm);
    frame.attach(hoursP);
    frame.attach(hoursN);
    frame.attach(minutsP);
    frame.attach(minutesN);
    frame.attach(set);
    frame.attach(stop);
    frame.runParallel(loop,&frame, &nextalarm);
    frame.show();
}

void startAlarm()
{
    gpiowrite(FANPIN, 0);
    int i = 600;
    while (i-- > 0)
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
}