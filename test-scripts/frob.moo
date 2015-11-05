{string} = args;
original_string = string;
" find the ->";
" work backwards looking for the expr_prefix stuff as a delimiter";
" work forwards looking for the expr_suffix stuff as a delimiter";
" if the expr_suffix is ( (ignoring spaces) then look for an argument list, otherwise assume the empty arg list";
iter = 1;
ptr = index(string, "->");
if (!ptr)
  return string;
endif
{su, cu} = {$string_utils, $code_utils};
indicator = this.frob_indicator;
try
  while loop_over_ptrs (ptrs = su:index_all(string, "->"))
    x = 1;
    while (cu:inside_quotes(string[1..ptrs[x]]))
      x = x + 1;
      if (x > length(ptrs))
        break loop_over_ptrs;
      endif
    endwhile
    ptr = ptrs[x];
    "";
    lenstr = length(string);
    if (string[start = ptr - 1] == ")")
      " the datatype is enclosed in parens";
      nested = 1;
      start = start - 1;
      while (start > 0)
        if (string[start] == "(")
          nested = nested - 1;
        elseif (string[start] == ")")
          nested = nested + 1;
        endif
        if (!nested)
          break;
        endif
        start = start - 1;
      endwhile
    else
      " the datatype is subexpression";
      start = 0;
      substring = string[1..ptr - 1];
      for expr_prefix in (this.expr_prefix_chars)
        if (cur = rindex(substring, expr_prefix))
          cur = cur + length(expr_prefix);
          start = max(cur, start);
        endif
      endfor
      " start == 0 if NO prefix was found (meaning the frob op is at the start of the line)";
      start = start || 1;
    endif
    datatype = string[start..ptr - 1];
    frobop_start = start;
    "";
    if (string[end = start = ptr + 2] == "(")
      " the method name is an expression";
      nested = 1;
      end = end + 1;
      while (end < lenstr)
        if (string[end] == ")")
          nested = nested - 1;
        elseif (string[end] == "(")
          nested = nested + 1;
        endif
        if (!nested)
          break;
        endif
        end = end + 1;
      endwhile
    else
      " the method name is a literal";
      substring = string[start..end = $];
      for expr_suffix in (this.expr_suffix_chars)
        if (cur = index(substring, expr_suffix))
          " substract 2 so the last char is BEFORE the first char of the found element";
          cur = cur + start - 2;
          end = min(end, cur);
        endif
      endfor
    endif
    method = string[start..end];
    if (indicator)
      " RIVERMOO-SPECIFIC -- THIS DOES NOT HAPPEN ON E_MOO";
      " prepend the frob indicator to the expression";
      if (method[1] == "(")
        method = tostr("(\"", indicator, "\" + ", method, ")");
        " expression";
      else
        " literal";
        method = indicator + method;
      endif
      " END RIVERMOO-SPECIFIC ALTERATION";
    endif
    frobop_end = end;
    if (frobop_end < lenstr && string[frobop_end + 1] == "(")
      " parameter list exists";
      nested = 1;
      end = start = frobop_end + 2;
      while (end < lenstr)
        if (string[end] == ")")
          nested = nested - 1;
        elseif (string[end] == "(")
          nested = nested + 1;
        endif
        if (!nested)
          break;
        endif
        end = end + 1;
      endwhile
      frobop_end = end;
      end = end - 1;
    else
      start = 1;
      end = 0;
    endif
    dmargs = string[start..end];
    subexpr = `string[frobop_end + 1..frobop_end + 2] == "->" ! ANY';
    variable = tostr("__datatmp", iter);
    static = tostr("@{} && ", variable);
    if (!dmargs)
      dmargs = tostr(variable, ", ", static);
    else
      dmargs = tostr(variable, ", ", dmargs, ", ", static);
    endif
    newsubstr = tostr("$type:handler(__datatmp", iter, " = ", datatype, "):", method, "(", dmargs, ")");
    subexpr && (newsubstr = tostr("(", newsubstr, ")"));
    string[frobop_start..frobop_end] = newsubstr;
    iter = iter + 1;
  endwhile
except (ANY)
  return original_string;
endtry
"Last changed by Kurt-Spiffy, #95@Mithreal, on Sun Jul 28 02:25:15 2002 CDT";
