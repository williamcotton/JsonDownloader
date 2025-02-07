#!/bin/bash

docker build -t json-downloader-test . && docker run -v ccache:/ccache json-downloader-test