# MT
The compiler skips the AST phase and produces the output based on only the tokens given.
Currently it's in alpha stage.

Usage: ```./compiler <file>``` <br>
For the compiler to work you must have C's <a href="https://gcc.gnu.org/install/">GCC compiler</a>.<br>
Sample code:
```javascript
var child1 = "John";
var child2 = "Johnson"; // creative.. I know :)
var child1_age = 16;
var child2_age = 16;

if child1_age > child2_age:
	print(child1_age);
end
elif child1_age == child2_age:
	print("They are the same age");
end
else:
	print(child2_age);
end
```
Output: ``` They are the same age ```<br>
Dynamic typing:
```javascript
var b = "DSASDA";
print(b);
b = 31.321;
print(b);
b = 52;
print(b);
```
Output:
```
DSASDA
31.321
52
```
Functions: (alpha/buggy)
```javascript
function squareOfTwo():
	print(4);
end
squareOfTwo();
```
<ul>
Why is gcc required for the compiler to work?
	<li>Because MT's compiler (ab)uses GCC's C extension that allows function definitions inside of main function .</li>
</ul>
