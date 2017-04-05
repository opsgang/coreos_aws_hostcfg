#!/bin/bash
# vim: et sw=4 ts=4 sr smartindent:
#
# - the systemd job should be set up via user-data - see USER-DATA.example.md
# - container should mount /etc/ and /home/core
# - add all fs assets to coreos:
#     - homedir assets for core user (includes /home/core/bin)
#     - reboot strategy - off by default
#     - gets instance info to /etc/custom/instance_info
#     - systemd files for:
#       - instance_info,
#       - credstash_to_fs,
#       - docker network,
#       - papertrail
echo "INFO $0: copying all fs assets to host"
if ! cp -a /assets/fs/* /
then
    echo "ERROR $0: failed to copy essential assets."
    exit 1
fi

if ! . /home/core/bin/common.inc >/dev/null 2>&1
then
    echo "ERROR $0: ... could not source /home/core/bin/common.inc" >&2
    exit 1
fi

i "... setting core user to own files under /home/core"
chown -R core:core /home/core

i "... making all /home/core/bin non .inc files executable"
find /home/core/bin -type f ! -name '*.inc' -exec chmod a+x {} \;
