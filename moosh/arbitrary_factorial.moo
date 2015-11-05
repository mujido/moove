base = 10;
power = 4;
maxint = base^power;

println("Enter number to find factorial of:");
n = input("int");

if (n > maxint)
	print("Value must be in range [1, ", maxint, "]");
	return -1;
endif

value = {1};

carry = 0;
value_iters = 0;

for i in [1..n]
	print(chr(13), i, "! (", length(value), " words)");
	
	for j in [1..length(value)]
		++value_iters;
		value[j] = value[j] * i + carry; 
		if (value[j] > maxint)
			carry = value[j] / maxint;
			value[j] %= maxint; 
		else
			carry = 0;
		endif
	endfor
	
	if (carry)
		value = {@value, carry};
		carry = 0;
	endif
endfor

println();
println("Value iterations: ", value_iters);
println("Words required: ", len = length(value), " (", len * 15, " bits)");
println(i, "! = ", value);

"Print out the decimal value now";
print("decimal = ");
for i in [1..len]
	x = value[len - i + 1];
        
        if (i > 1)
        	for j in [2..power]
	        	if (x < base^(power - j + 1))
		        	print("0");
		        endif   
        	endfor
        endif

	print(x);
endfor
println();
