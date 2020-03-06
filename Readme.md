# Autonomous Controller

**This writeup is still WIP**

## The Idea

I bought a standing desk from Autonomous (Smart Desk 2 Home Office). After setting it up, I was wondering how the controller and the table/motor controller communicate with each other.

## ToDo

- More detailed writeup
- Test findings by implementing my own controller on a uC (probably ESP32)

## First impressions

Opening up the controller reveals nicely labeled PINs.

![IMG_20200304_195541](/home/stefan/Dokumente/Autonomous/IMG_20200304_195541.jpg)

Thus we now only know that UART is used to transfer data, but also which wire carries which signal:

| color  | function |
| ------ | -------- |
| red    | sleep(?) |
| orange | GND      |
| green  | TX       |
| yellow | RX       |
| blue   | 5V       |

After some trying out, I found the baudrate: 9600

Height-Profiles are not saved on this controller - they are stored in the desk.

The controller board uses a STC12C5612AD controller. It features:

- 3,5-5,5V
- 8 Bit
- 23 I/O pins
- 1x UART
- 768 Bytes RAM
- 4K-30 KB program memory size (according to datasheet)

[Datasheet](https://www.datasheet4u.com/datasheet-pdf/STC/STC12C5612AD/pdf.php?id=853205)

## My Setup

I used a DSView Logic Analyzer which I bought here [from Banggood](https://www.banggood.com/DSLogic-Logic-Analyzer-16-Channels-400M-Sampling-USB-based-Debugging-Logic-Analyzer-p-1200454.html).

I did not dismantle the controller that shipped with my desk. Instead, I asked the customer support for a spare one / replacement. They shipped it to me for 27€.

I opened up the casing (standard screws) and found a nicely labeled PCB where I could attach the probes to. 

## Received Data

When no button is pressed, we keep receiving (repeating):

Base Message:

0x98, 0x98, 0x00, 0x00, (2 more Bytes, indicating current height)



Height indication:

Height: 29.05

Last 2 Bytes of Message: 0x4B, 0x4B



Height: 29.09

Last 2 Bytes of Message: 0x4C, 0x4C



Height: 30.03

Last 2 Bytes of Message: 0x4D, 0x4D



Height keeps increasing in steps of 0.4
Message keeps increasing by 1
...



Height: 48.00

Last 2 Bytes of Message: 0x7A, 0x7A



Height: 48.40

Last 2 Bytes of Message: 0x7B, 0x7B

## Transmitted Data

### Pressing up

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x02, 0x02

(keep going with end 0x02 as long as the button is pressed)

### Pressing Down

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x02, 0x01

(keep going with end 0x01 as long as the button is pressed)

### Pressing 1

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x04, 0x04

(keep going with end 0x04 as long as the button is pressed)

### Pressing 2

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x08, 0x08

(keep going with end 0x08 as long as the button is pressed)

### Pressing 3

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x10, 0x10

(repeating 0x10 as long as the button is pressed)

### Pressing 4

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x00, 0x20

(repeating 0x20 as long as the button is pressed)

### Pressing M

0xD8, 0xD8, 0x66, 0x00, 0x00

0xD8, 0xD8, 0x66, 0x40, 0x40

0xD8, 0xD8, 0x66, 0x40, 0x40

(repeating 0x40 as long as the button is pressed)

## Analyzing the protocol

- Controller->Desk: Every message contains 5x2 bytes
- Controller->Desk: Message always starts with 0xD8, 0xD8, 0x66
- Controller->Desk: Last 2 Bytes always repeat
- Desk->Controller: Every message contains ... bytes
- Desk->Controller always starts with ...
- Every button corresponds to 1 bit in the message

  - 7 buttons, 8 bit -> highest bit unused (?)
- The controller sends the current state of the buttons after receiving a message from the desk


