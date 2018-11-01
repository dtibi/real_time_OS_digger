#include <conf.h>
#include <kernel.h>

int n;

xmain()
{
	int first();
	int pr1, pr2; // first and secon priority

	printf("Enter first and second priority\n");
	scanf("%d%d", &pr1, &pr2);
	// run first thread
	resume (create(first, INITSTK, pr1, "proc 1", 1, pr2));
}

first(pr2)
int pr2;
{
	int second();

	n=0;
	// run second thread
	resume (create(second, INITSTK, pr2, "proc 2", 0));

	if (n==0)
	    n=1;
	if (n==1)
	    printf("first win\n");
	else
	    printf("second win\n");
}

second()
{
	if (n==0)
	   n=2;
}