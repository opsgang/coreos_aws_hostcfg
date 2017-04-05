# vim: et sr sw=4 ts=4 smartindent syntax=dockerfile:
FROM opsgang/aws_env:stable

MAINTAINER jinal--shah <jnshah@gmail.com>
LABEL \
      name="opsgang/coreos_aws_hostcfg"  \
      vendor="sortuniq"                  \
      description="\
... adds common services;     \
gets instance info;           \
gets secrets from credstash ; \
prepares /home/core;          \
"

COPY assets /assets

RUN    chmod a+x /assets/scripts/*  \
    && /assets/scripts/core_user.sh
