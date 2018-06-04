
Debian
====================
This directory contains files used to package vedad/veda-qt
for Debian-based Linux systems. If you compile vedad/veda-qt yourself, there are some useful files here.

## veda: URI support ##


veda-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install veda-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your veda-qt binary to `/usr/bin`
and the `../../share/pixmaps/veda128.png` to `/usr/share/pixmaps`

veda-qt.protocol (KDE)

