# 42sh
Complete shell

Shell coded on iOS, should work on any linux system.
This shell supports most of bash features, including redirections, globing, completion, command line edition with termcaps library...

The following builtins were coded completely: cd, echo, env, setenv, unsetenv, history.
Any other command will look for a matching binary in the PATH variable.
It also implements CTRL+R for backward searching in history.
CTRL+U, CTRL+P, CTRL+H, CTRL+L can be used for copying/cutting on the command line and CTRL+Y will paste it.
SHIFT+UP will move one line upward, SHIFT+DOWN will move one line downward, SHIFT+LEFT OR RIGHT can be used to navigate between words.
Home goes to the beginning of line. End to the end of line.
