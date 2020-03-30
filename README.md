# Organizer

## Overview

Organizer is a Qt lightweight personal information manager.

It is maintained by the LXQt project but can be used independently from this
desktop environment.


### Compiling source code

Please do not do this on production systems

```
mkdir build  
cd build  
cmake ..
make
su (or sudo)
make install
```

Organizer is installed to /usr/local/bin

Note: If upgrading from a previous version you need to remove the previous version (as root) located at

/usr/local/bin/organizer

and rename (or remove) the 'organizerdb.sqlite' database in your home directory. Make sure to export contacts and then import them into the new version.

### Prerequisites

You need the following dependencies to build and run Organizer on Debian, Ubuntu and its derivatives.

```
build-essential
qt5-default 
qtcreator
automake
checkinstall
cmake
libqt5svg5-dev
libqt5*-dev
qttools5-dev
libkf5plasma-dev
liblxqt0-dev
qtmultimedia5-dev
```

### Binary packages

Binary packages are available as snap packages and in the Arch User Repository (AUR) 

* [Snap by ito32bit](https://snapcraft.io/lxqt-organizer-snap) 

To test the latest successful build use:
```
sudo snap install --edge lxqt-organizer-snap
```
To remove use:
```
sudo snap remove lxqt-organizer-snap
```
* [Arch (AUR) by Librewish](https://aur.archlinux.org/packages/lxqt-organizer-git/) 

It can be installed using yay as shown below.

```
yay -S lxqt-organizer
```

## Versioning

[SemVer](http://semver.org/) is used for versioning. The version number has the form 0.0.0 representing major, minor and bug fix changes. Currently at 0.6.0 and working towards 1.0.0 major release.
 

## Roadmap

```
Calendar themes and localization
System notifications
Time related features
``` 

#### Runtime Dependencies
* ${misc:Depends},
* ${shlibs:Depends}

#### Runtime Recommends
* lxqt-organizer-l10n (not implemented yet)
* lxqt-qtplugin

### Binary packages

Official binary packages are not available in the moment.  
One could find the initial packaging for Debian here:  
https://salsa.debian.org/lxqt-team/lxqt-organizer

Arch AUR will come soon.


### Translation (Weblate)
Not available yet - will come soon, when we implemented the translation system.
