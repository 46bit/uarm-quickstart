# µArm Developer Quickstart
The [µArm](https://kickstarter.com/projects/ufactory/uarm-put-a-miniature-industrial-robot-arm-on-your) is a great bit of hardware, but since shipping the software has let it down. The official software isn't readily usable to kickstart your own programs. This contains several demo programs and the official firmware so you can immediately start your project.

This is an unofficial collection of software, intended for **experienced, cautious hackers**. µArms are easy to break by misuse of the servos and the current firmware doesn't do much to prevent such harm.

## Setting up the µArm
**Make sure your µArm servos at in the correct position shown in the construction guide. They will rapidly burn out if not.**

* Install the [FTDI drivers](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers/all) on your computer. These are needed for the integrated programmer on the Arduino.
* Install the [Arduino software](http://arduino.cc/en/Main/Software).
* Add the `firmware/UF_uArm` libraries to Arduino, and copy the contents of `firmware/arduino.txt` into your Arduino code editor.
* Connect your µArm to power and your computer. If this is your first time connecting it will emit a very loud continuous beep.
* Select the correct port in the Arduino software menus for your µArm.
* Upload your Arduino software to the µArm. It should stop beeping once this is complete.
* Hold the front µArm button until it beeps. Immediately push the rear cylindrical joint all the way down. Then pull the µArm 'hand' all the way forward. Now hold the front button again until it beeps.

## Controlling your µArm
I suggest placing the µArm on a flat, smooth surface. Clear the surrounding area of anything you don't want to get knocked over.

Edit the serial port in `serial.h`. Run `make` to build the provided demo programs into `./bin/test`, `./bin/hanoi` and `./bin/uarmin`.

### ./bin/test
This test program just moves your µArm about a bit.

### ./bin/hanoi
This can be used to solve the [Towers of Hanoi](https://en.wikipedia.org/wiki/Tower_of_Hanoi), if you have a pile of 3 objects to place.

### ./bin/uarmin
Pipe commands to the `stdin` of this program to forward them to the µArm. We've used this to control it with an Xbox controller.

## Command format for ./bin/uarmin
Commands must be formatted like `30 120 180 0 0` and separated by `\n` newlines. Each integer corresponds to:

* Arm Rotation in degrees. Ranges from -90 to 90.
* Arm Stretch. Ranges from 0 to 210.
* Arm Height. Ranges from -180 to 150 **but values below 0 will go through your table**.
* Hand Rotation in degrees. Ranges from -90 to 90.
* Gripper. Set to 1 to pick up an option, 0 to release.

## Future
There's a lot of problems with the official firmware, a quick list [here](https://gist.github.com/46bit/1386cfe0932228fbe8b8). I aim to build a new firmware that resolves many of these issues, and would love contributions to this Quickstart in the meantime.

## Credits
The [µFactory](http://ufactory.cc) team have done a great job designing and shipping the µArm.

Scott Gray wrote up the useful [uArm Serial Protocol Manual](https://dl.dropboxusercontent.com/u/37860507/uArm%20Serial%20Protocol.pdf).

[Tod E. Kurt](http://todbot.com) produced the [arduino-serial](http://todbot.com/blog/2013/04/29/arduino-serial-updated/) library these programs use to reliably talk to the Arduino over Serial.

This unofficial [µArm quickstart](https://github.com/46bit/uarm-quickstart) was made by [Michael Mokrysz](https://46b.it) with the help of others at University of York [HackSoc](http://hacksoc.org). All contents except the `firmware` folder are licensed under MIT.
