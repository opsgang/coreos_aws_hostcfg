# set\_up\_ssh\_keys
<!---
vim: et sr sw=2 ts=2 smartindent syntax=markdown:
-->

... create ssh key files for your users from ssh\_keys stored in credstash ...

Use credstash to store your ssh keys, then use this script to make them available to users
on an ec2 instance.

    Supports multiple keys and multiple users.

## EXAMPLE

1. store an ssh private key in a credstash table under the name SSH\_KEY\_GITHUB.
  Assume we want this on our instance at `~core/.ssh/id\_rsa` and ~root/.ssh/id\_rsa

2. store another private key file in a credstash table under the name SSH\_KEY\_MY\_APP.
  Assume we want this on our instance at `~core/.ssh/id_rsa-app` and ~root/.ssh/id\_rsa-app

3. make sure /etc/custom/.credstash/tables includes the tables that store your keys.
  (See credstash\_to\_fs for more info about that).

4. create an EnvironmentFile or use Environment directives in your systemd unit (example below)
  We want to set these vars:

      SSH_KEY_GITHUB=id_rsa
      SSH_KEY_MY_APP=id_rsa-app

That's it.

As long as your unit runs _After_ and _Requires_ credstash\_to\_fs.service, you need do nothing else.

## EXAMPLE: systemd

The example below will create ssh keys:

* for users _core_ and _bob_ (`$SSH_USERS`)

* from a credstash value stored as SSH_KEY_MY_GITHUB_KEY, written to ~/.ssh/id_rsa

* from a credstash value stored as SSH_KEY_MY_OTHER_KEY, written to ~/.ssh/id_rsa-other

**Note the values must already have been retrieved by credstash\_to\_fs for this to work**
**so ensure that the tables storing the values are included in /etc/custom/.credstash/tables**

```systemd
[Unit]
Description=creates ssh keys for root and core users using credstashed vals prefixed SSH_KEY_
After=credstash_to_fs.service
Requires=credstash_to_fs.service

[Service]
TimeoutSec=20
Type=oneshot
RemainAfterExit=yes
User=root

Environment='SSH_USERS=core bob'
Environment='SSH_VAR_PATTERN=^SSH_KEY_'

Environment='SSH_KEY_MY_GITHUB_KEY=id_rsa'
Environment='SSH_KEY_MY_OTHER_KEY=id_rsa-other'

ExecStart=/bin/bash /home/core/bin/set_up_ssh_keys

[Install]
WantedBy=multi-user.target
```
