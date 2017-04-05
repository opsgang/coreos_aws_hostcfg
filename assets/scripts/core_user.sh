#!/bin/sh
# vim: ts=4 sw=4 sr et smartindent:
# create_user_core.sh
# - suitable for alpine
# - uses uid, gid 500 from coreos
addgroup -g 500 core
adduser -D -h /home/core -u 500 -G core -s /bin/bash core
