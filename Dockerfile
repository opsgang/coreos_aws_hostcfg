# vim: et sr sw=4 ts=4 smartindent syntax=dockerfile:
FROM opsgang/aws_env:stable

LABEL \
      name="opsgang/coreos_aws_hostcfg"  \
      vendor="sortuniq"                  \
      description="\
... adds common services;\n\
gets instance info;\n\
gets secrets from credstash;\n\
prepares /home/core;"

COPY assets /assets

RUN /bin/bash /assets/scripts/core_user.sh
