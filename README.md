# How to atomically increment
To increment something atomically, the increment has to be attempted in a single
_step_. One cannot just atomically read and atomically write a new value.
Consider the `fixt()` and `not_fixt()` functions. Just because a variable is atomic
doesn't mean that it stays the same between reads and writes in the context of a single
thread if there are multiple threads trying to modify the same variable. The author
naively believes that the code is simple enough not to comment it.

# How to run
```bash
gcc  t1.c -lpthread && ./a.out
```
Or, alternatively:
```bash
clang  t1.c -lpthread && ./a.out
```

