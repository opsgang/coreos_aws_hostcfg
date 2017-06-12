#!/bin/bash
# vim: et sw=4 ts=4 sr smartindent:
#
# - the systemd job should be set up via user-data - see USER-DATA.example.md
# - container should mount /etc/ and /home/core
# - adds all fs assets to coreos
echo "INFO $0: copying all fs assets to host"
# N.B - this cp won't work with any dot files directly
# under /assets/fs ... (e.g. /assets/fs/.somefile -> failure)
if ! cp -a --remove-destination /assets/fs/* /
then
    echo "ERROR $0: failed to copy essential assets."
    exit 1
fi

# ... some assets can be user-defined and we don't want to clobber them
cp -a -n /assets/fs-defaults/* /

if ! . /home/core/bin/common.inc >/dev/null 2>&1
then
    echo "ERROR $0: ... could not source /home/core/bin/common.inc" >&2
    exit 1
fi

i "... setting core user to own files under /home/core"
chown -R core:core /home/core

i "... making all /home/core/bin files executable unless readme or .inc"
find /home/core/bin -type f ! -name '*.inc' -a ! -name '*.md' -exec chmod a+x {} \;

