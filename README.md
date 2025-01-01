
# img4dc - Custom audio track size Edition
The original version was hardcoded to 302 sectors of audio for Audio/Data images, I wanted to adjust it to work with custom sector sizes for better load times.
Changes are only for cdi4dc.
Adds a parameter -a, which can be used to set the sector count of the audio track for Audio/Data images (the one that was hardcoded to 302 in the original).
Example: cdi4dc data.iso example.cdi -a 50000
Default is still 302 when not provided.
Keep in mind that the value you use in mkisofs still has 11400 sectors added, so the value you have to use there is the value for cdi4dc + 11400 (e.g. 302 -> 11702).

Cmake Edition readme:
# img4dc - CMake Edition

This is a fork(ish) of the img4dc project which [can be found here](https://sourceforge.net/projects/img4dc/)

If you are using Windows, you probably just want the upstream version as it may have updates.

This project exists because I needed to be able to programmatically download and compile img4dc to build
a Docker image, and the build system in the upstream code wasn't very Linux friendly.

The only changes you'll find here are:

 - CMake is the build system
 - Some stuff may be tweaked to compile nicely on Linux
 - This readme exists :)


