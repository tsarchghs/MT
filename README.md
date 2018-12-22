# MT
The compiler skips the AST phase and produces the output based on only the tokens given.
Currently it's in alpha stage.

```./compiler <file>```
For the compiler to work you must have C's clang compiler. (<a href="https://clang.llvm.org/get_started.html">Building and Running Clang</a>)<br>
Sample code:
```javascript
var dogg = "gjergj";
var dogg_copy = dogg;
var d = 1;
var c = 2;

if d >= c:
	if c <= d:
		if d == c:
			d = 1;
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
