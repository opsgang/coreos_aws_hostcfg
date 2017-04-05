# vim: et sr sw=4 ts=4 smartindent syntax=sh:
# ... to be sourced
#
# - sets our key=val info files (e.g. instance_info) in script env
# - provides utility functions to:
#   - check required vars or fail.
#   - get value from kms
#
export PATH=$PATH:/home/core/bin
FUNC_DIR=/home/core/bin/functions.d
SC="$(basename $(realpath $0))"

FILES_TO_SRC="
    /etc/custom/instance_info
    /etc/custom/docker_image_versions
"

function _check_var_defined() {
    local var_name="$1"
    local var_val="${!var_name}"
    if [[ -z $var_val ]]; then
        return 1
    fi
}

function required_vars_help() {
    cat <<EOM
usage: required_vars "<var_name1> <var_name2> ..."
... return false if any var_name is undeclared, undefined or empty.
EOM

}

function required_vars() {
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

function container_as_cmd_help() {
    cat <<EOM
usage: container_as_cmd <cmd to emulate> <arg1> <arg2> ...
... wrapper to run container as if it were a binary.
You must define CONTAINER_NAME and TAGGED_IMG before use.
Optionally, CONTAINER_ENV="SOME_VAR OTHER_VAR", to pass vars from your env.
e.g.
    export CONTAINER_NAME=awscli; export TAGGED_IMG=awscli:0.0.1
    container_as_cmd --region eu-west-1 ec2 describe-instances
EOM

}

# container_as_cmd():
# runs docker container to emulate cmd
# ... requires env vars to be set:
#     CONTAINER_NAME: name to give container
#     TAGGED_IMG: name of img to run
# Optional:
# CONTAINER_ENV="SOME_VAR_FROM_MY_ENV OTHER_VAR_FROM_ENV"
# - e.g. list of vars to pass to docker with vals from calling env.
#
function container_as_cmd() {

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

function _container_env_str() {
    for varname in $CONTAINER_ENV; do
        str="$str --env $varname"
    done

    echo "$str"
}

# ... print msgs to console
function e() {
    echo "ERROR $SC: $*" >&2
}

function i() {
    echo "INFO $SC: $*"
}

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
