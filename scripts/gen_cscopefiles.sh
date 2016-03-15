#!/bin/bash

PROJECTS="${PROJECT_ROOT:-`pwd`} $SMING_HOME"
BUILD_DIR="out/build"

#truncate cscope.files
cat /dev/null >| cscope.files

for p in ${PROJECTS}; do
  PROJECT_ROOT=$p
  for i in `find "${PROJECT_ROOT}/${BUILD_DIR}" -name *.d`; do
    #first massage dependency file to make it usable bt cscope, then substitute relative by absolute paths
    sed -e 's/^.*://;s/\\//;s/^[ \t]*//;s/[ \t]*$//;s/ \+/\n/g;/^$/d' $i | sed -e "s|^\([^/].*\)|$PROJECT_ROOT/\1|"
  done
done | sort | uniq > cscope.files.tmp

while read p; do
  readlink -f $p
done < cscope.files.tmp | sort | uniq > cscope.files
