# ballboy

Detects a ball within a given HSV range and outputs its coordinates to stdout.

### Installation

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

### Usage

    # See usage:
    ./ballboy

    # Run it for HSV between (78, 41, 88) and (91, 256, 204)
    ./ballboy 78 41 88 91 256 204

[1]: http://docs.opencv.org/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html
