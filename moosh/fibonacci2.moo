#!/home/kurt/moosh/src/moosh

fact = {0.,1.};
n = input("int");
for i in [2..n]
        fact = {@fact, fact[$ - 1] + fact[$]};
endfor
println(n >= 2 ? fact | {0.});
	
