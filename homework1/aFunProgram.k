code aFunProgram

  -- This is the "aFunctionProgram" program, for use in Homework 1.

  function main ()
			var
				ch: char
				first: bool = true -- Indicates if this char is the first of the line
			while true
				ch = intToChar(GetCh())
				if first && ch == 'q' -- First char and it was a q
					break
				endIf

				-- first = (ch == '\n') -- For some reason this doesn't work?!
				-- If user just entered a new line char then we did too,
				-- so the next char is first of that line
				if ch == '\n'
					first = true
				else
					first = false
				endIf
				printChar(ch)
			endWhile

			debug -- Exited due to 'q' at beginning
    endFunction

endCode
