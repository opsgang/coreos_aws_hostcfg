# vim: et sr sw=4 ts=4 smartindent syntax=sh:
# ... to be sourced
#
# - sets our key=val info files (e.g. instance_info) in script env
# - provides utility functions to:
#   - check required vars or fail.
#   - get value from kms
#

### FUNCTIONS

# _set_SC(): provides $SC (abbreviated script name for log msgs)
_set_SC() {
    if [[ $0 =~ ^-?bash$ ]]; then
        echo "bash"
    else
        basename $(realpath $0)
    fi
}

_check_var_defined() {
    local var_name="$1"
    local var_val="${!var_name}"
    if [[ -z $var_val ]]; then
        return 1
    fi
}

required_vars_help() {
    cat <<EOM
usage: required_vars "<var_name1> <var_name2> ..."
... return false if any var_name is undeclared, undefined or empty.
EOM

}

required_vars() {
    local rc=0
    local required_vars="$1"
    local this_var
    for this_var in $required_vars; do
        if ! _check_var_defined $this_var
        then
            e "\$$this_var must be set in env"
            rc=1
        fi
    done

    return $rc
}

container_as_cmd_help() {
    cat <<EOM
usage: container_as_cmd <cmd to emulate> <arg1> <arg2> ...
... wrapper to run container as if it were a binary.
You must define CONTAINER_NAME and TAGGED_IMG before use.
Optionally, CONTAINER_ENV="SOME_VAR OTHER_VAR", to pass vars from your env.
e.g.
    export CONTAINER_ENV="AWS_ACCESS_KEY_ID AWS_SECRET_ACCESS_KEY";
    export CONTAINER_NAME=boo; export TAGGED_IMG=opsgang/aws_env:stable;
    container_as_cmd aws ec2 describe-instances
    container_as_cmd credstash -r eu-west-1 -t secrets get MY_SECRET
EOM

}

container_as_cmd() {

    local cmd_args="$@"
    local env_opts=""
    if [[ ! -z "$CONTAINER_ENV" ]]; then
        env_opts=$(_container_env_str)
    fi

    required_vars "CONTAINER_NAME TAGGED_IMG" || return 1
    sudo docker kill $CONTAINER_NAME >/dev/null 2>&1
    sudo docker rm -f $CONTAINER_NAME >/dev/null 2>&1

    sudo -E docker run --name $CONTAINER_NAME --rm $env_opts $TAGGED_IMG $cmd_args
}

# _container_env_str(): constructs --env str from list in $CONTAINER_ENV
_container_env_str() {
    for varname in $CONTAINER_ENV; do
        str="$str --env $varname"
    done

    echo "$str"
}

random_str_help() {
    cat <<EOM
usage: random_str
... produces random-ish str (current datetime & unique num)
Append to container names to avoid race conditions.
EOM

}

random_str() {
    echo $(date +'%Y%m%d%H%m%s')-$(( $(shuf -i 1-999999 -n1) * $(shuf -i 1-999999 -n1) ))
}

# e() / i() ... print log msg
e() {
    echo "ERROR $SC: $*" >&2
}

i() {
    echo "INFO $SC: $*"
}

# MAIN

_do_common() {
	export PATH=$PATH:/home/core/bin
	FUNC_DIR=/home/core/bin/functions.d
	SC=$(_set_SC)
	FILES_TO_SRC="
		/etc/custom/instance_info
		/etc/custom/versions
	"

	# ... src useful stuff in to env
	for file_to_src in $FILES_TO_SRC; do
		if [[ -r $file_to_src ]]; then
			i "... sourcing $file_to_src"
			. $file_to_src
		fi
	done

	for func_file in $(ls -1 $FUNC_DIR/* 2>/dev/null); do
		. $func_file
	done
	unset func_file file_to_src FUNC_DIR FILES_TO_SRC
}

_do_common ; unset _do_common
