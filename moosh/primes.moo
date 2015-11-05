num_primes = input("int");
primes = {2,3};
if (num_primes <= 0)
	print("Number of primes must be > 0");
	return 0;
elseif(num_primes < 3)
	return primes[1..num_primes];
endif
num_primes -= 2;
"Any prime is at least 2 greater than the previous, because prime + 1 would be even (except prime == 2)";
x = primes[$] + 2;
while(num_primes > 0)
	found_prime = 1;
	for y in (primes)
		if (x % y == 0)
			"Not a prime, continue main loop";
			found_prime = 0;
			break;
		endif
	endfor
	if (found_prime)
		primes = {@primes, x};
		--num_primes;
	else
		x += 2;
	endif
endwhile

return primes;
				
