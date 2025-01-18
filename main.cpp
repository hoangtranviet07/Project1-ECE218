//=====[Libraries]=============================================================
#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============
DigitalIn Pass_Belt_Button(D4);
DigitalIn Driver_Belt_Button(D5);
DigitalIn Pass_Seat_Button(D6);
DigitalIn Driver_Seat_Button(D7);
DigitalIn ignitionButton(D3);

DigitalOut ignitionLed(LED1);
DigitalOut engineLed(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

DigitalInOut sirenPin(PE_10);

//=====[Declarations (prototypes) of public functions]=========================
void inputsInit();
void outputsInit();

void processing();

//=====[Main function, the program entry point after power on or reset]========
int main() {
    processing();
}

//=====[Implementations of public functions]===================================
void inputsInit() {
    Pass_Belt_Button.mode(PullDown);
    Driver_Belt_Button.mode(PullDown);
    Pass_Seat_Button.mode(PullDown);
    Driver_Seat_Button.mode(PullDown);
    ignitionButton.mode(PullDown);
    sirenPin.mode(OpenDrain);
    sirenPin.input();
}

void outputsInit() {
    ignitionLed = OFF;
    engineLed = OFF;
}

// The processing function implements the whole project
void processing() {
    inputsInit();
    outputsInit();
    
    // Welcome message loop when driver seat button is pressed
    while (true) {
        if (Driver_Seat_Button) {
            uartUsb.write("Welcome to enhanced alarm system model 218-W24\r\n", 50);
            break;
            //break out of the loop to continue to the next step
        }
    }
    
    /* Ignition control loop
    *  Check whether all 4 conditions are satisfied so that the ignition Led will be on.
    *  If not, siren alarm, print out ignition is inhibited then list the unsatisfied
    *  condition(s).
    */
    while (true) {
        if (Driver_Seat_Button && Pass_Seat_Button && Driver_Belt_Button && Pass_Belt_Button) {
            ignitionLed = ON;
            if (ignitionButton && ignitionLed) {
                engineLed = ON;
                ignitionLed = OFF;
                uartUsb.write("Engine started.\r\n", 17);
                break;
                //break out of the loop to continue to the next step
            }
        } else {
            if (ignitionButton) {
                sirenPin.output();
                sirenPin = LOW;
                uartUsb.write("Ignition inhibited\r\n", 20);

                if (!Driver_Seat_Button) {
                    uartUsb.write("Driver seat not occupied\r\n", 27);
                }
                if (!Pass_Seat_Button) {
                    uartUsb.write("Passenger seat not occupied\r\n", 30);
                }
                if (!Driver_Belt_Button) {
                    uartUsb.write("Driver seatbelt not fastened\r\n", 31);
                }
                if (!Pass_Belt_Button) {
                    uartUsb.write("Passenger seatbelt not fastened\r\n", 34);
                }
                break;
                //break out of the loop so that the message will not be sent multiple times.
            }
        }
    }
}
