#!/bin/sh

FindPath()
{
    fullpath="`echo $1 | grep /`"
    if [ "$fullpath" = "" ]; then
        oIFS="$IFS"
        IFS=:
        for path in $PATH
        do if [ -x "$path/$1" ]; then
            if [ "$path" = "" ]; then
                path="."
            fi
            fullpath="$path/$1"
            break
        fi
    done
    IFS="$oIFS"
    fi
    if [ "$fullpath" = "" ]; then
        fullpath="$1"
    fi

    # Is the sed/ls magic portable?
    if [ -L "$fullpath" ]; then
        #fullpath="`ls -l "$fullpath" | awk '{print $11}'`"
        fullpath=`ls -l "$fullpath" |sed -e 's/.* -> //' |sed -e 's/\*//'`
    fi
    dirname $fullpath
}


here="`FindPath $0`"
export LD_LIBRARY_PATH="$here/base:$LD_LIBRARY_PATH"
$here/base/diec $*
