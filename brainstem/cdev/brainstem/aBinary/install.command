#!/bin/sh
#
# install
#
# This program puts copies of the current aBinary frameworks
# into your system Frameworks directory.

DEST=~/Library/Frameworks

echo Installing into $DEST

# these three lines move the current directory to the one the 
# script is located in.  This is because the finder uses the
# current working directory / when a script is double clicked
# from the finder and our work here is relative to the script
foo=${0%/*}
echo Changing directory to $foo
cd $foo

# sometimes this script is not in place to start with
echo Ensuring destination directory
mkdir -p $DEST


# now, for each file named a*.framework, copy it up to the 
# destination (library) directory and then remove the local
# copy
for file in `ls -d a*.framework`
do
  if [ -d $DEST/$file ] ; then
      echo removing old $file
      rm -rf $DEST/$file
  fi
  echo installing $file
  tar -cf - $file | tar -C $DEST -xf -
  rm -rf $file
done

echo Framework Installation Finished!
