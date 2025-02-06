FROM ubuntu:latest

# Prevent interactive prompts during package installation
ARG DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
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

# Create non-root user
RUN useradd -ms /bin/bash vscode \
    && apt-get update \
    && apt-get install -y sudo \
    && echo vscode ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/vscode \
    && chmod 0440 /etc/sudoers.d/vscode

# Clone JUCE as the vscode user
USER vscode
RUN git clone --depth 1 --branch 8.0.6 https://github.com/juce-framework/JUCE.git ~/JUCE

WORKDIR /workspaces/JsonDownloader 