#include "mbed.h"
#include "bbcar.h"
#define D 17
#define R_SPEED 90
#define R_FAC 0.48
#define L_SPEED 95
#define L_FAC -0.52

RawSerial pc(USBTX, USBRX);

RawSerial xbee(D12, D11);

Serial uart(D1, D0);      // tx, rx

void reply_messange(char *xbee_reply, char *messange);

void check_addr(char *xbee_reply, char *messenger);


DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
PwmOut pin9(D9), pin8(D8);
DigitalInOut pin10(D10);
DigitalIn pin3(D3);
Ticker servo_ticker;
Ticker encoder_ticker;

BBCar car(pin8, pin9, servo_ticker);

int main()
{
    led1 = 1;
    led2 = 1;
    led3 = 1;
  pc.baud(9600);
  char s[21];

  char xbee_reply[4];


  // XBee setting

  xbee.baud(9600);

  xbee.printf("+++");

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){

    pc.printf("enter AT mode.\r\n");

    xbee_reply[0] = '\0';

    xbee_reply[1] = '\0';

  }

  xbee.printf("ATMY 0x230\r\n");

  reply_messange(xbee_reply, "setting MY : 0x230");


  xbee.printf("ATDL 0x130\r\n");

  reply_messange(xbee_reply, "setting DL : 0x130");


  xbee.printf("ATID 0x1\r\n");

  reply_messange(xbee_reply, "setting PAN ID : 0x1");


  xbee.printf("ATWR\r\n");

  reply_messange(xbee_reply, "write config");


  xbee.printf("ATMY\r\n");

  check_addr(xbee_reply, "MY");


  xbee.printf("ATDL\r\n");

  check_addr(xbee_reply, "DL");


  xbee.printf("ATCN\r\n");

  reply_messange(xbee_reply, "exit AT mode");

  xbee.getc();


  // start

  pc.printf("start\r\n");

  // t.start(callback(&queue, &EventQueue::dispatch_forever));


  // Setup a serial interrupt function of receiving data from xbee

  // xbee.attach(xbee_rx_interrupt, Serial::RxIrq);

    parallax_ping ping1(pin10);
    parallax_encoder encoder0(pin3, encoder_ticker);
    
    xbee.printf("Go Straight\r\n");
    wait(.01);
    
    car.goStraight(100);
    while (1) {
        if((float)ping1 > D) led2 = 0;
        else {
            led2 = 1;
            car.stop();
            break;
        }
        wait(.01);
    }
    led1 = 1;
    xbee.printf("Turn Left\r\n");
    car.turn(L_SPEED, L_FAC);    // turn left
    wait(1);
    car.stop();
    
    // go into mission 1
    xbee.printf("Go Stright\r\n");
    car.goStraight(100);
    while (1) {
        if((float)ping1 > (D - 2))led2 = 0;
        else {
            led2 = 1;
            car.stop();
            break;
        }
        wait(.01);
    }
    xbee.printf("Turn Right\r\n");
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();

    encoder0.reset();
    xbee.printf("Reverse parking\r\n");
    car.goStraight(-100);
    while (encoder0.get_cm() < 38) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    wait(1);

    xbee.printf("Leave parking\r\n");
    encoder0.reset();
    car.goStraight(100);
    while (encoder0.get_cm() < 35) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();

    xbee.printf("Turn Left\r\n");
    car.turn(L_SPEED, L_FAC);    // turn left
    wait(1);
    car.stop();


    xbee.printf("Go to snapshot\r\n");
    encoder0.reset();
    car.goStraight(-100);
    while (encoder0.get_cm() < 50) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    
    xbee.printf("Turn Right\r\n");
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();

    encoder0.reset();
    xbee.printf("Reverse parking\r\n");
    car.goStraight(-100);
    while (encoder0.get_cm() < 20) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    xbee.printf("snapshot\r\n");
    led1 = 0;
    char recv = '-';
    while (1) {
        sprintf(s, "image_classification");
        uart.puts(s);
        wait(0.5);
        if(uart.readable()){
            recv = uart.getc();
        }
        if (recv != '-') {
            if (recv == 'o')
                recv = '5';
            xbee.printf("%c\r\n", recv);
            led1 = 1;
            break;
        }
    }

    xbee.printf("Leave parking\r\n");
    encoder0.reset();
    car.goStraight(100);
    while (encoder0.get_cm() < 20) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    xbee.printf("Turn Right\r\n");
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();

    // exit mission 1
    xbee.printf("Exit Mission 1\r\n");
    car.goStraight(100);
    encoder0.reset();
    while (encoder0.get_cm() < 10) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();

    // enter mission2
    xbee.printf("Go Straight\r\n");
    car.goStraight(100);
    while (1) {
        if((float)ping1 > D) led2 = 0;
        else {
            led2 = 1;
            car.stop();
            break;
        }
        wait(.01);
    }
    xbee.printf("Enter Mission 2\r\n");
    xbee.printf("Turn right\r\n");
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();

    xbee.printf("Go Straight\r\n");
    car.goStraight(100);
    encoder0.reset();
    while (encoder0.get_cm() < 25) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    xbee.printf("Turn right\r\n");
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();

    xbee.printf("Go Straight\r\n");
    car.goStraight(100);
    encoder0.reset();
    while (encoder0.get_cm() < 23) {
        led2 = 0;
        wait(.01);
    }
    led2 = 1;
    car.stop();
    xbee.printf("Turn left\r\n");
    car.turn(L_SPEED, L_FAC);    // turn left
    wait(1);
    car.stop();

    float x[3];
    xbee.printf("Object detect\r\n");
    car.turn(L_SPEED, L_FAC);    // turn left
    wait(0.5);
    car.stop();
    x[1] = (float)ping1;
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();
    x[2] = (float)ping1;
    car.turn(L_SPEED, L_FAC);    // turn left
    wait(0.5);
    car.stop();
    x[0] = (float)ping1;
    xbee.printf("x[0] = %f, x[1] = %f, x[2] = %f\r\n", 
        x[0], x[1], x[2]);
    if ((x[1] - x[0] < 5) && (x[2] - x[0] < 5))
        xbee.printf("rectangle\r\n");
    else if ((x[0] - x[1] > 5) && (x[0] - x[2] > 5))
        xbee.printf("Two triangle\r\n");
    else if ((x[1] - x[0] > 5) && (x[2] - x[0] > 5))
        xbee.printf("Triangle\r\n");
    else
        xbee.printf("Right triangle\r\n");    

    xbee.printf("Turn left\r\n");
    car.turn(L_SPEED, L_FAC);    // turn left
    wait(1);
    car.stop();
    xbee.printf("Go Straight\r\n");
    car.goStraight(100);
    while (1) {
        if((float)ping1 > D) led2 = 0;
        else {
            led2 = 1;
            car.stop();
            break;
        }
        wait(.01);
    }    
    xbee.printf("Exit Mission 2\r\n");

    xbee.printf("Turn right\r\n");
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();
    
    xbee.printf("Go Straight\r\n");
    car.goStraight(100);
    while (1) {
        if((float)ping1 > D) led2 = 0;
        else {
            led2 = 1;
            car.stop();
            break;
        }
        wait(.01);
    }    
    car.turn(R_SPEED, R_FAC);     // turn right
    wait(1);
    car.stop();
    car.goStraight(100);

    car.goStraight(100);
    wait(10);
    car.stop();
}



void reply_messange(char *xbee_reply, char *messange){

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  xbee_reply[2] = xbee.getc();

  if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){

    pc.printf("%s\r\n", messange);

    xbee_reply[0] = '\0';

    xbee_reply[1] = '\0';

    xbee_reply[2] = '\0';

  }

}


void check_addr(char *xbee_reply, char *messenger){

  xbee_reply[0] = xbee.getc();

  xbee_reply[1] = xbee.getc();

  xbee_reply[2] = xbee.getc();

  xbee_reply[3] = xbee.getc();

  pc.printf("%s = %c%c%c\r\n", messenger, xbee_reply[1], xbee_reply[2], xbee_reply[3]);

  xbee_reply[0] = '\0';

  xbee_reply[1] = '\0';

  xbee_reply[2] = '\0';

  xbee_reply[3] = '\0';

}
