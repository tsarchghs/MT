# MT
The compiler skips the AST phase and produces the output based on only the tokens given.
Currently it's in alpha stage.

Usage: ```./compiler <file>``` <br>
If the executable code (```./compiler```) doesn't work for your machine compile with ```make```<br>
For the compiler to work you must have C's clang compiler. (<a href="https://clang.llvm.org/get_started.html">Building and Running Clang</a>)<br>
Sample code:
```javascript
var name = "gjergj";
var name_copy = name;
var x = 1;
var y = 2 + 5 / 2;
var z = y;
// Example of nested if's
if x >= y: // If statements ends with the keyword 'end'
	if z <= x: 
		if x < y > z:
			z = 1;
		end
	end
end
```

<ul>
Why is clang required for the compiler to work?
	<li>Because MT's compiler generates C code and then hands it to clang to do the rest of the job.</li>
</ul>
<ul>
Can the compiler use GCC instead?
	<li>Nope, I'll add a command line option in the future but for now you can change <code>system("clang o.c");</code> in <code>compiler.c</code> line 70. Or just manually compile the output <code>o.c</code>.
</li>
</ul>
