/* Assumptions:
   s is a variable, possibly a string or other data type
   E_INVARG is a variable either defined by the client or set earlier in code
*/
try
   /* may throw E_INVARG if s is not a string */
   return index(s, "abc");
except(E_INVARG)
   return E_INVARG;
endtry