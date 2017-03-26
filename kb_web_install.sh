#!/bin/bash
WEB=/home/kevin/docs/kzone5/
SOURCE=$WEB/source
TARGET=$WEB/target
make clean
(cd ..; tar cvfz $TARGET/dirsize.tar.gz dirsize)
cp doc/README_dirsize.html $SOURCE
(cd $WEB; ./make.pl README_dirsize)
