# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

set editor=vim

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth
# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=100000
HISTFILESIZE=200000

# append to the history file, don't overwrite it
if [[ "$SHELL" == "/bin/bash" ]]; then
    shopt -s histappend
    # check the window size after each command and, if necessary,
    # update the values of LINES and COLUMNS.
    shopt -s checkwinsize

    # If set, the pattern "**" used in a pathname expansion context will
    # match all files and zero or more directories and subdirectories.
    shopt -s globstar

    # enable programmable completion features (you don't need to enable
    # this, if it's already enabled in /etc/bash.bashrc and /etc/profile
    # sources /etc/bash.bashrc).
    if ! shopt -oq posix; then
      if [ -f /usr/share/bash-completion/bash_completion ]; then
        . /usr/share/bash-completion/bash_completion
      elif [ -f /etc/bash_completion ]; then
        . /etc/bash_completion
      fi
    fi
fi

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# some more ls aliasesliases
ltrfunction() {
  ls -trFGlAhp  -h $1  && echo Total files: "$[$(ls -1ap | grep -v $'/' | wc -l)]"
}
lfunction() {
  ls -FGlAhp  -h $1  && echo Total files: "$[$(ls -1ap | grep -v $'/' | wc -l)]"
}

PS1='\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]$ '
# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# alias open='xdg-open'
alias ..='cd ..'
alias ltr=ltrfunction
alias l='ls -alhG'
alias ll='ls -alFG'
alias la='ls -AG'
alias ..='cd ..'
alias vi="nvim"
alias vim="nvim"
alias xp='PATH="/usr/lib/xorg:$PATH" xpra start :12'
alias killmosh='pgrep mosh-server | grep -v $(ps -o ppid --no-headers $$) && xargs kill || echo "no active sessions to kill"'
alias cpugetavail='cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_available_governors'
alias cpushowcurrent='cat /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor'
alias cpusethigh='echo performance | sudo tee /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor'
# cat /proc/cpuinfo | grep "^[c]pu MHz" | cut -b 12-18 | paste -sd+ - | bc | awk '{ print $1 / 64 }'"

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# CUDA
export LD_LIBRARY_PATH=/usr/local/cuda/lib64
export CUDA_HOME=/usr/local/cuda
# Iterm
test -e "${HOME}/.iterm2_shell_integration.bash" && source "${HOME}/.iterm2_shell_integration.bash"
set show-mode-in-prompt on

export PATH=/home/parag/dev/google-cloud-sdk/bin:/home/parag/anaconda3/bin:$CUDA_HOME/bin:/usr/local/bin:$PATH
export MPLBACKEND=Agg
export DISPLAY=:0
export MKL_THREADING_LAYER=GNU

# Prompt set by promptline
source ~/.promptline
[ -f ~/.fzf.zsh ] && source ~/.fzf.zsh

# export PATH="$HOME/.rbenv/bin:$PATH"
# eval "$(rbenv init -)"
# Add RVM to PATH for scripting. Make sure this is the last PATH variable change.
# export PATH="$PATH:$HOME/.rvm/bin"

term_title() {
    unset PROMPT_COMMAND
    echo -ne "\033]0;${@}\007"
}

# . /home/pkmital/anaconda3/etc/profile.d/conda.sh
# . /home/parag/anaconda3/etc/profile.d/conda.sh
# . /Users/pkmital/anaconda3/etc/profile.d/conda.sh
export NVM_DIR="$HOME/.nvm"
  [ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm
  [ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"  # This loads nvm bash_completion

# Required for GPU / CUDA / TensorFlow
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CONDA_PREFIX/lib/

# source ~/.winerc
# Required for GPU / CUDA / TensorFlow
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CONDA_PREFIX/lib/
export USE_GKE_GCLOUD_AUTH_PLUGIN=True


# JINA_CLI_BEGIN

## autocomplete
_jina() {
  COMPREPLY=()
  local word="${COMP_WORDS[COMP_CWORD]}"

  if [ "$COMP_CWORD" -eq 1 ]; then
    COMPREPLY=( $(compgen -W "$(jina commands)" -- "$word") )
  else
    local words=("${COMP_WORDS[@]}")
    unset words[0]
    unset words[$COMP_CWORD]
    local completions=$(jina completions "${words[@]}")
    COMPREPLY=( $(compgen -W "$completions" -- "$word") )
  fi
}

complete -F _jina jina

# session-wise fix
ulimit -n 4096
export OBJC_DISABLE_INITIALIZE_FORK_SAFETY=YES
# default workspace for Executors

# JINA_CLI_END







