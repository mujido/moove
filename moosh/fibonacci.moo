#!/home/kurt/moosh/src/moosh

n = input("int");
fib0 = 0;
fib1 = 1;

if(n < 0)
	fib1 = fib0;
elseif(n > 1)
	for i in [2..n]
		tmp = fib0;
		fib0 = fib1;
		if(fib0 + tmp < 0) 		/* if overflow occurs, switch to floating point */
			fib1 += 1.0 * tmp;
		else
			fib1 += tmp;
		endif
	endfor
endif

println("fib[", n, "] = ", fib1);

