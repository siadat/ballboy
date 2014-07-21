# ballboy

Detects a ball within a given HSV range and outputs its coordinates to stdout.

![Screenshot](http://blog.sariina.com/wp-content/uploads/2014/07/sina_melodyball.png)

### Requirements

- a webcam
- a coloured ball
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

    # Run it for a green ball with HSV between (78,41,75) and (91, 256, 204):
    ./ballboy 78 41 75 91 256 204

    # Run it with theremin midi controller:
    ./ballboy 66 47 24 90 256 204 | ruby ./examples/theremin.rb

    # Run in verbose mode (extra lines are prefixed with '#')
    ./ballboy -v 66 47 24 90 256 204

The output looks like this:

    x:36    y:55    r:31
    x:38    y:62    r:30
    x:40    y:65    r:32
    x:40    y:60    r:31
    x:35    y:50    r:29

All values are normalized between 0 and 100. If nothing is found, the values will be -1.

[1]: http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html
