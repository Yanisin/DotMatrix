FROM ubuntu:bionic

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential bzip2 python wget make automake autoconf libusb-1.0.0-dev libtool pkg-config texinfo openocd telnet\
    && apt-get install -y --no-install-recommends git ca-certificates \
    && apt-get install -y --no-install-recommends usbutils dfu-util python3 python3-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -m dev
RUN usermod -aG root dev
RUN usermod -aG plugdev dev
WORKDIR /home/dev

# setup gcc-arm-none-eabi toolchain from vendor
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/8-2018q4/gcc-arm-none-eabi-8-2018-q4-major-linux.tar.bz2 -O cortex_m.tar.bz2 && \
    tar -xjf cortex_m.tar.bz2 && \
    rm cortex_m.tar.bz2
ENV PATH "/home/dev/gcc-arm-none-eabi-8-2018-q4-major/bin:$PATH"

# switch the user and provide an "app" directory for development
USER dev
RUN mkdir /home/dev/app
WORKDIR /home/dev/app

CMD ["/bin/bash"]
