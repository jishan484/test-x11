#include "Frame.h"
#include "Button.h"
#include "Label.h"



void test(Label * label){
    static int count = 1;
    string text = "Button-1 clicked : "+to_string(count++);
    label->drawText(text);
}
void test2(Frame *frame)
{
    printf("button 2 clicked\n");
    frame->close();
}
void loop(Frame * frame , Label * label){
    static int sec = 0;
    string text = "App running for : "+to_string(sec++)+" ["+to_string(frame->width)+"x"+to_string(frame->height)+"]";
    label->drawText(text);
    Frame::sleep(1);
}
int main()
{
    Frame frame(600,300);
    Button button1(10, 10, 70, 20, "Open");
    Button button2(90, 10, 70, 20, "show");
    Button button3(170, 10, 70, 20, "Close");
    Label label(frame, 60, 70,185);
    Label label1(frame, 60, 85);
    Label label2(frame, 60, 100);
    label.setFontColor(255, 0, 0);
    label.drawText("hello world this is a real test");
    int i = 23;

    button1.onClick(test,&label);
    button2.onClick([&]()->void{
        // frame.detach(label);
        frame.detach(button1);
        string text = "Button1 height is : " + to_string(button1.height);
        label.drawText(text);
        
    });
    button3.onClick(test2,&frame);
    frame.attach(button1);
    frame.attach(button2);
    frame.attach(button3);

    label1.setFontColor(255, 0, 255);
    label1.drawText("hello tester is a test");
    label2.drawText("aaaaaaaaaaaaaaaa");
    frame.attach(label);
    frame.attach(label1);
    frame.attach(label2);

    frame.runParallel(loop,&frame,&label2);
    frame.show();
    // loop(frame);
}