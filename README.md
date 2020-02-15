# kfsh
a minimalist bash-like shell

## Overview
Using the availible GNU C library on linux systems, I made a minimal shell that performs some mundane commands that users execute.

## Commands

These are the currently supported commands:

###### Note: directory and file paths can be either relative or absolute

Command | options | option arguments | arguments | format | use
:------:|:------:|:---:|:------:|:-------------:|:------:|
cdir|none|none|directory path|`cdir dir/`|change working directory
ldir|-s|none|directory path|`ldir dir/ [-s]`|list directory
gdir|-s|none|none|`gdir [-s]`|get current working directory
sdir|none|none|directory path and file name|`sdir dir/ /file.ext`|search for file in directory
mkf| -u -g -o|permissions(rwx)|directory name|`mkf -u perm -g perm -o perm dir/`| create directory 
rmf|none|none|directory path|`rmf dir/`|delete empty directory|
finf|none|none|file path|`finf /file.ext`|print file info
mfl|none|none|file path|`mfl /file.ext`|create file
pfl|none|none|file path|`pfl /file.ext`|print file
rfl|none|none|old path and new path|`rfl /old.ext /new.ext`|rename or move file
prt| -n -r -s|print sting|none|`prt -[nrs] string`|print string
genv|--uname --home --shell --editor --lang --path|none|none|`genv [--name] [--home] [--shell] [--editor] [--lang] [--path]`|get environment variable

## Resources
Tutorials are really scarce for developing a shell from scratch, however documentation is not ! 
Here are some resources that streamlined my development process:
+ #### [actual source code](http://www.delorie.com/)
+ #### [commendable documentation](https://pubs.opengroup.org/)
+ #### [source code](https://www.gnu.org/)
+ #### [god tier documentation](https://linux.die.net/)

## More to come
Features I will be implementing soon:
+ ##### Sudo priveledge modifier
+ ##### command history
+ ##### redirecting output
+ ##### pipes
+ ##### process related commands
+ ##### setting related commands
+ ##### git command
