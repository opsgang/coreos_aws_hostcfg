# coreos\_aws\_hostcfg

_... container adds build-delivery tools, cfgs and user files to a coreos host_

## building

**master branch built at shippable.com**

[![Run Status](https://api.shippable.com/projects/58e3c53abefe150700ec299b/badge?branch=master)](https://api.shippable.com/projects/58e3c53abefe150700ec299b)

```bash
git clone https://github.com/opsgang/coreos_aws_hostcfg.git
cd coreos_aws_hostcfg
./build.sh # adds custom labels to image
```

## installing

```bash
docker pull opsgang/coreos_aws_hostcfg:stable # or use the tag you prefer
```

## running

