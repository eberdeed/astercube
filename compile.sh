#!/bin/bash
cd assimpopengl/build;
cmake ..;
make;
make assimpdocs;
make install;
cd ../../build
cmake ..;
make;
make doc;
make install;
