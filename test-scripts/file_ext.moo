/* Search through text files */
for f in (cur_dir():files("*.txt", "*.doc", "*.asc"))
   /* Get the extension */
   ext = f[f:rindex(".") + 1..$];

   /* Perform different operation depending on file type */
   switch(ext)
      case("txt")
         /* Convert text file to Word doc. Calls the program in the
	    TXT2DOC enviroment variable. */
	 exec($TXT2DOC, f);
	 break;

      case("doc")
	/* Move file to /tmp directory */
	move_file(f, "/tmp/" + f);
	break;

      case("asc")
        /* Delete obsolete file */
	delete_file(f);
	break;
   endswitch
endfor