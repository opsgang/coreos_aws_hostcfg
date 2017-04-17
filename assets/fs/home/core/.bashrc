# .bashrc
# original coreos ~/.bashrc only sources this skel file:
. /usr/share/skel/.bashrc

[[ $- != *i* ]] && return # stop if non-interactive

. /home/core/bin/common.inc

_print_helpers() {

    # ... funcs: (non _help ones)
    local f=$(declare -f | grep -Po '^(\w+)(?= \(\))' | grep -v '_help$')

    # ... cmds under /home/core/bin
    local x=$(cd /home/core/bin && find ./ -maxdepth 1 -type f -executable | sed -e 's#\./##')

    # ... combined bin cmds and shell funcs
    f=$(echo "$f$x"| sed -e '/^$/d' | sort | uniq)

    # ... funcs for which a _help func defined in here or under functions.d
    local fh=$(declare -f | grep -Po '^(\w+)(?=_help \(\))')

    local func_list=$(comm -1 <(echo "$f") <(echo "$fh"))

    # ... find longest name, for prettier formatting
    local length_name=$(
        echo "$func_list"                                   \
        | awk '{ print length()+2 | "sort -nr | head -1" }'
    )

    echo "SHELL FUNCTIONS: "
    # ... print helper info
    # ... provide default helper info if not defined.
    local help_txt="usage: ... no help information provided. That sucks."
    local func
    for func in $func_list; do
        if ! set | grep "^${func}_help ()" >/dev/null 2>&1
        then
            eval "function ${func}_help(){ echo \"$help_txt\"; }"
        fi
        _print_helper_msg "$func" "$length_name"
    done
}

_print_helper_msg() {
    local func="$1"
    local length_name="$2"
    local start_line="${func}()"
    local format_str line

    local oIFS=$IFS
    IFS=$'\n'

    for line in $(${func}_help); do
        # ... info lines in cyan 'cos its purrdy Mr. Taggart...
        if [[ $start_line =~ ^[\ ]$ ]]; then
            line="\e[2m\e[36m$line\e[0m\e[22m"
            format_str="%-${length_name}s $line\n"
        else
        # ... function names in green with emboldened usage info
            line="\e[1m$line\e[0m"
            format_str="\e[32m%-${length_name}s\e[0m $line\n"
        fi
        printf "$format_str" $start_line
        start_line=' ' # omit function name at start of subsequent lines
    done

    IFS=$oIFS
    echo ""
}

if [[ $- == *i* ]]; then
    _print_helpers

    # user can provide ~.ps1 file with a set_prompt function
    if [[ -r ~/.ps1 ]]; then
        . ~/.ps1 2>/dev/null
        set_prompt
    fi
fi

