from subprocess import Popen,PIPE,STDOUT
from os.path import dirname,abspath
from os import environ

get_output = True

compiler_path = dirname(dirname(abspath(__file__))) + "/compiler"

tests = [
	{
		"source":"source/test_print.mt",
		"cli_output":b"Compiled.\n",
		"output":b'1\n1.500000\nHello, World!\n'
	},
	{
		"source":"source/test_variables.mt",
		"cli_output":b"Compiled.\n",
		"output":b'DSASDA\n31.100000\n52\n52\nhelloo\n'
	}
]

class TestCase:
	def compile(self,source,expected_output):
		cmd = f"{compiler_path} {source}"
		p = Popen(cmd, shell=True, stdin=PIPE, stdout=PIPE,
				stderr=STDOUT, close_fds=True)
		output = p.stdout.read()
		if get_output:
			print(output)
		if output != expected_output:
			print(f"{output} != {expected_output} (compile)");
	def execute(self,exec_file,expected_output):
		p = Popen(exec_file, shell=True, stdin=PIPE, stdout=PIPE,
				stderr=STDOUT, close_fds=True)
		output = p.stdout.read()
		if get_output:
			print(output)
		if output != expected_output:
			print(f"{output} != {expected_output} (execute)");

testCase = TestCase()
for test in tests:
	cli_output = testCase.compile(test["source"],test["cli_output"]);
	c_output = testCase.execute("./a.out",test["output"]);
