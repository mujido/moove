/* Implements a strcmp()-like program.
   Assumptions:
   str1 and str2 are two strings to compare.
   asc() is a builtin method that returns the ASCII value for a character.
*/
min_size = min(str1:length(), str2:length());
for i in [1..min_size]
   diff = asc(str1[i]) - asc(str2[i]);
   if(diff)
	return diff;
   endif
endfor
return 0;
	