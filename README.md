# Shell-Simulation
Simple simulation of shell using C language and *nix system  calls.

# This simulation can:
- Execute builtin shell(bash) commands, example:
  > ls, ps, top ...
- Execute custom made commands, example:
  > quit, cwd (current working directory), cd ..
- Run background process, example:
  > top&
- Use of redirection (>) & (<) to a file, example:
  > ls > output.txt
- Use of pipe (|), example:
  > ls | sort
- Kill process with terminate signal, example:
  > kill <pid>

# This simulation can not:
- Chain commands, example:
  > ls | grep .c | sort
  
# This project tries to follow kernel coding style, which can be found here:
- https://www.kernel.org/doc/Documentation/CodingStyle

