setenv OSTYPE `/bin/uname -s`
if ($OSTYPE == "LINUX") Then
setenv PATH ~/bin:/usr/local/turnin:${USRLOCAL}:${JDK}:${SYSTEM}:${FACBIN}:/home/maynard/bin:.
endif
if ($?USER == 0 || $?prompt == 0) exit
set history=100
alias ts 'eval `tset -s \!*`'
alias utar "tar -xvf"
set ignoreeof
set prompt="`hostname`{`whoami`}\!: "
alias cs3423 "cd ~/courses/cs/3423"
    alias sq "ln -a \!*"
set cdpath=( ~/courses/cs ~)
    setenv SUB_cs3423 'section1'
    alias locate "locate -d /var/lib/mlocate/mlocate.db:/usr/local/mlocate/mlocate.db"
