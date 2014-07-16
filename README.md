farmbot-arduino-controller
==========================
This software is responsible for receiving G-Codes from the Raspberry Pi, execute them and report back the results.

Technicals
==========================
Created with eclipseArduino V2 - For more details see http://www.baeyens.it/eclipse/

Command line flash tool installation
==========================

(in progress)

```
sudo apt-get install gcc-avr avr-libc avrdude python-configobj python-jinja2 python-serial python-setuptools
mkdir tmp
cd tmp
git clone https://github.com/miracle2k/python-glob2
cd python-glob2
sudo python setup.py install
cd ..
git clone git://github.com/amperka/ino.git
cd ino
sudo make install
```

Command line flash tool use
==========================

```
ino build
ino upload
```
