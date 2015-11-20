#!/bin/bash

cd /home/iwasz/workspace/gtkforms
find src/ -name '*.cc' -o -name '*.h' | grep -v 'Reflection_' | awk -- '{ print "LIST (APPEND LIB_SOURCES \"" $1 "\")"; }'
