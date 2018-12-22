var name = "gjergj";
var name_copy = name;
var x = 1;
var y = 2 + 5 / 2;
var z = y;
// Example of nested conditional statements
if x >= y: // Conditional statements end with the keyword 'end'
	if z <= x: 
		z = x + y;
	end
	elif z >= x:
		z = x - y;
	end
	else:
		x = y;
	end
end