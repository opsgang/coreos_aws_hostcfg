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
        basename $(realpath -- $0)
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

random_str_help() {
    cat <<EOM
usage: random_str
... produces random-ish str (current datetime & unique num)
Append to container names to avoid race conditions.
EOM

}

random_str() {
    date +"%Y%m%d%H%m%s-$(( $(shuf -i 1-999999 -n1) * $(shuf -i 1-999999 -n1) ))"
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
