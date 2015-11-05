count = input("int");
if (count <= 0)
	print("Count must be > 0");
	return 0;
endif
value = {1., 0., 0., 0.};
"Setup n_factorial = 1.0 for 0!";
n_factorial = {1.0, 0., 0., 0.};
for n in [1..count - 1]
	"Calculate n! (n factorial) for new n using previous value"; 
	for i in [1..length(n_factorial)]

	n_factorial *= n;
	value += 1.0 / n_factorial;
endfor

return value;

