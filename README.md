# ballboy

Detects a ball within a given HSV range and outputs its coordinates to stdout.

### Requirements

- a webcam
- OpenCV

### To build

Install [OpenCV][1]:

    # OSX
    brew install opencv

    # CentOS
    yum install opencv

    # Debian
    apt-get install opencv

Clone and compile ballboy:

    git clone https://github.com/siadat/ballboy
    cd ballboy
    cmake .
    make

### To run

    # See usage:
    ./ballboy

    # Run it for HSV between (78, 41, 75) and (91, 256, 204):
    ./ballboy 78 41 75 91 256 204

    # Run it with theremin midi controller:
    ./ballboy 78 41 75 91 256 204 | ruby ./examples/theremin.rb

The output will be like this:

    x:563   y:507   r:118
    x:593   y:439   r:125
    x:619   y:453   r:130
    x:651   y:449   r:114
    x:647   y:459   r:114


[1]: http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html
