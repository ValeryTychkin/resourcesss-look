# ================================
# App run
# ================================
FROM swift:5.8

USER root

# Install OS updates and, if needed, sqlite3
RUN export DEBIAN_FRONTEND=noninteractive DEBCONF_NONINTERACTIVE_SEEN=true \
    && apt-get update \
    && apt-get dist-upgrade -y \
    && apt-get -q install -y \
      ca-certificates \
      libseccomp-dev \
      make \
# If your app or its dependencies import FoundationNetworking, also install `libcurl4`.
      # libcurl4 \
# If your app or its dependencies import FoundationXML, also install `libxml2`.
      # libxml2 \
    && rm -r /var/lib/apt/lists/*


WORKDIR /vapor

RUN git clone https://github.com/vapor/toolbox.git

WORKDIR /vapor/toolbox

RUN git checkout 18.7.1
RUN make install

# Set up a build area
WORKDIR /app

# First just resolve dependencies.
# This creates a cached layer that can be reused
# as long as your Package.swift/Package.resolved
# files do not change.
COPY ./Package.* ./
RUN swift package resolve

# RUN mkdir CClientSocket

COPY ./ ./
COPY ./UNIXSocket/client.c ./UNIXSocket/base_socket.c ./Sources/CClientSocket/
COPY ./UNIXSocket/client.h ./UNIXSocket/base_socket.h ./Sources/CClientSocket/include/

RUN vapor build
