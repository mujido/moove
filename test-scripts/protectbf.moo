dobjstr = (i = index(dobjstr, "()")) ? dobjstr[1..i - 1] | dobjstr;
if (player != this)
  return raise(E_PERM);
elseif (`function_info(dobjstr) ! E_INVARG => 0')
  prot = "protect_" + dobjstr;
  value = index(verb, "@protectbf") ? 1 | 0;
  if (!`property_info($server_options, prot) ! E_PROPNF => 0')
    add_property($server_options, prot, value, {player, "r"});
  else
    $server_options.(prot) = value;
  endif
  player:tell(tostr("Protection of built-in function ", dobjstr, "() has been ", value ? "activated." | "deactivated."));
  if (verb[$] != "!" && !$command_utils:yes_or_no("Do you wish to load server options?"))
    return player:tell("Server options left unloaded.");
  else
    load_server_options();
    player:tell("Server options have been loaded.");
  endif
endif
