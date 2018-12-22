# MT
The compiler skips the AST phase and produces the output based on only the tokens given.
Currently it's in alpha stage.

Sample code:
```javascript
var dogg = "gjergj";
var dogg_copy = dogg;
var d = 1;
var c = 2;

if d > c:
	if c < d:
		if c < d > c:
			d = 1;
		end
	end
end
```
