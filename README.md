# Description

This tool is here to make your life easier during the TELE's TDs.
Helping you focus on the important notions and lose less time with
boilerplate code that is now provided to you.


# How to make the project

If your are confident you can install all the dependencies yourself,
you can go ahead and use the CMake build files that are provided
and build it as follows :
```shell script
mkdir build; cd build; cmake ..; make -j `nproc`
```

If you are not too confident and have docker installed you can use
the Dockerfiles provided to build and run it inside a container like so :
```shell script
docker build . -t tele
xhost +local:root # gives access to the X11 socket from local docker
docker run -it --rm --device=/dev/dri \
                    --env="DISPLAY=$DISPLAY" \
                    --env="XDG_RUNTIME_DIR=/tmp/runtime-root" \
                    --volume=/tmp/.X11-unix:/tmp/.X11-unix \
                    --volume=$PWD:/app/local \
                    tele
```

Alternatively one can also use the precompiled and pushed image
from the docker hub as so :
```shell script
xhost +local:root # gives access to the X11 socket from local docker
docker run -it --rm --device=/dev/dri \
                    --env="DISPLAY=$DISPLAY" \
                    --env="XDG_RUNTIME_DIR=/tmp/runtime-root" \
                    --volume=/tmp/.X11-unix:/tmp/.X11-unix \
                    --volume=$PWD:/app/local \
                    dotty/tele
```
If you did not already pull the image from the docker hub the previous command
will download it and run it. If you have any questions about the docker
commands I suggest you give a quick look at the documentation.


# Usage

## Selecting the images

First thing to do after launching the program is select the directory
where all the images are located.
There must only be images in this directory and the images must be named
lexicographically with respect to their shooting time.

## The GUI

Once the folder with images is selected you are provided with a view of
the first image and you need to use the mouse to point and with the left click
select the position of the eye of the cyclone.

You can switch to the next image by using the `View` menu,
using the `Alt + Right Arrow` shortcut or also using a more ergonomic shortcut
that is `Right Click`

You can go back in images using the menu, shortcut or also using
the ergonomic shortcut `Shift + Right Click`

All other menus are self explanatory.

## The CSV export

Once you have identified the eye of the cyclone in all the images you can
export by clicking the export button in the toolbar, select the name of the file
and you can go ahead and view the data you got using your preferred csv viewer.
I can recommend this very simple technique :
```shell script
column -s, -t < file.csv
```

If there is a lot of data you can optionally pipe it into a pager like less :
```shell script
column -s, -t < file.csv | less
```

Note tha you can also export the data even if all images are not geotagged but
the exported data might not be of any use.

## The PNG export

This is a neat feature that allows you to save the current view you have
allowing you to save a snapshot of your work to include it in presentations,
report or anything you want.

# Contributing

This project will soon be accessible and you'll be able to submit pull requests
to contribute to the project !

# Todo

* Fix trajectory drawing when skipping a point
* Allow the user to change the thickness of the brush used for the eye pointing
and trajectory drawing
* Allow the user to change the color of the brush used for the eye pointing
and trajectory drawing
* Use `Ctrl + Wheel Up/Down` to control the zoom level
* Export all the tracing steps as different PNG images
* Export the the tracing steps as an animated image (GIF/MP4)

# Issues

If you have difficulties using this software or a feature request please
open an issue on the repository.
Please only use the email to ask permission to use the code.

# Authors
Thomas 'Dotty' Michelot <thomas.michelot@epita.fr>
