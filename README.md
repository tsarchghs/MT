# MT
The compiler skips the AST phase and produces the output based on only the tokens given.
Currently it is in alpha stage and supports only integers.

Sample code:
```javascript
var c = 1 + 2;
var d = c + 1;

if c > d:
	d=d+1;
end
```
