#!/usr/bin/env bash
tmpname='/tmp/sce'
scefile='/tmp/.scefile'

if test -f "${scefile}"; then
    echo '
remove Lock = Caps_Lock
add Lock = Escape
keysym Caps_Lock = Escape
keysym Escape = Caps_Lock
' > ${tmpname}
    rm -f ${scefile}
else
    echo '
remove Lock = Caps_Lock
add Lock = Escape
keysym Escape = Caps_Lock 
keysym Caps_Lock = Escape
' > ${tmpname}
    touch ${scefile}
fi

xmodmap ${tmpname}
rm -f ${tmpname}
