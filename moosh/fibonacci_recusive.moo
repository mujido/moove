if(length(args) == 0)
	println("Enter number of fibonacci values to compute:");
	n = input("int");

	if(n <= 0)
		println("Number must be > 0");
		return -1;
	elseif(n <= 2)
		return {0, 1}[1..n];
	else
		return self({0, 1}, n - 2);
	endif
elseif(args[2] <= 0)
	return args[1];
else
	return self({@args[1], args[1][$ - 1] + args[1][$]}, args[2] - 1);
endif
