FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    ccache \
    libasound2-dev \
    libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev \
    libfreetype-dev \
    libfontconfig1-dev \
    libx11-dev \
    libxcomposite-dev \
    libxcursor-dev \
    libxext-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxrender-dev \
    libwebkit2gtk-4.1-dev \
    libglu1-mesa-dev \
    mesa-common-dev \
    && rm -rf /var/lib/apt/lists/*

# Configure ccache
ENV CCACHE_DIR=/ccache
ENV PATH="/usr/lib/ccache:$PATH"
VOLUME /ccache

# Clone JUCE
RUN git clone --depth 1 --branch 8.0.6 https://github.com/juce-framework/JUCE.git ~/JUCE

RUN rm -f compile_commands.json

# Copy project files
WORKDIR /app
COPY . .

# Build the test application
CMD cd TestApplication/Builds/LinuxMakefile && make CONFIG=Debug && ./build/TestApplication
