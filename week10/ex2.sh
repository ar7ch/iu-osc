touch ../week01/file.txt # create this file
link ../week01/file.txt _ex2.txt #create link to that file
ls -i ../week01/file.txt # get the inode number
find ../ -inum 3030077 # find all files in parent directory that point to this inode
find . -inum 3030077 -exec rm {} \; # remove all links in current directory
find ../ -inum 3030077 # only ../week01/file.txt remains
