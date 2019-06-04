#
# setup.sh
#
# setup script for hsc
#
# 2019.06.04  M.Morii
#

# This script must be loaded by "source" command of bash.

##
## setup mitool
##

source /home/morii/work/github/moriiism/mitool/setup/setup.sh

##
## hsctool 
##

export HSCTOOL=/home/morii/work/github/moriiism/hsc

##
## change the terminal title
##

termtitle="hsctool"
PROMPT_COMMAND='echo -ne "\033]0;${termtitle}\007"'
