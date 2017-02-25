
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sys/stat.h>

_Atomic int _glbl_id=0;
int fixt(){
	int ret=-1,id;

	//what possible could go wrong?
start: id = atomic_load(&_glbl_id);
	ret = id;
	if (!atomic_compare_exchange_strong(&_glbl_id, &id, id+1))
        {
            goto start;
        }
	return ret;

}

int not_fixt(){
	int ret=-1,id;

	//what possible could go wrong?
	id = atomic_load(&_glbl_id);
	ret = id;
	id += 1;
	atomic_store(&_glbl_id, id);
	return ret;

}

int main()
{
    printf("I compile\n");

}
