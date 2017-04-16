# ecr-login
<!---
vim: et sr sw=2 ts=2 smartindent syntax=markdown:
-->

... example systemd for running ecr\_login on a timer

/home/core/bin/ecr\_login expects the file /etc/custom/ecr\_registries
to exist.

It will log in both the `root` and `core` user.

## EXAMPLE: ecr registries

Running ecr\_login against the example below will
run `docker login` against 3 ECR registries
- for acct 012345678901 in eu-west-1 and us-east-2
- for acct 987654321098 in eu-west-1

```
# /etc/custom/ecr_registries
# format:<region>=<aws acct> <aws acct> ...
eu-west-1=012345678901 987654321098
us-east-2=012345678901
```

## EXAMPLE: systemd

```systemd

# /etc/systemd/system/ecr_login.timer
[Unit]
Description=Run ecr_login every 6 hours

[Timer]
OnActiveSec=6h
Persistent=true

[Install]
WantedBy=multi-user.target

# /etc/systemd/system/ecr_login.service
[Unit]
Description=Run awscli container to get credentials to access ECR
After=coreos_aws_hostcfg.service
Requires=coreos_aws_hostcfg.service

[Service]
Type=oneshot
Environment="CONTAINER_NAME=ecr_login"
ExecStartPre=-/usr/bin/docker kill ecr_login
ExecStartPre=-/usr/bin/docker rm -f ecr_login
ExecStart=/home/core/bin/ecr_login
ExecStartPost=-/usr/bin/docker rm -f ecr_login
TimeoutSec=25
User=root

[Install]
WantedBy=multi-user.target

```


