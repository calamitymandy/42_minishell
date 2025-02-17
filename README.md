# 42_minishell

# Work with GIT:
1- Before you start working, it is important to check that you are on your branch.

2- To update the branch you are working on to include the changes from the main branch:

```
git checkout main -> to switch back to the main branch *
git pull -> to fetch the latest changes from main
git checkout your_branch_name -> to switch back to your branch
git merge main -> to bring changes from main to your branch
```

3- To push changes to `main` with PULL REQUEST:

- From GitHub, create a `pull request` to `main` to include the changes from your branch into main.

Comparing changes: `base: main <- compare: your_branch`

Depending on the files you have edited, conflicts may occur during the `merge`. These can be resolved in GitHub or in VSCode.

VSCode will highlight the files with conflicts in red and will show an exclamation mark (!) on the right side of the file name. To resolve the conflicts, you can click on each file and use the "Resolve in Merge Editor" button. It will show a user interface where you can select which version of the code you want to keep or discard for each conflict, and once resolved, you can complete the merge by clicking the `Complete Merge` button.

- Once the conflicts are resolved, you can make a commit to push the changes of your updated branch to GitHub.

*Note: This command will give an error if you have changes in your branch that have not been added to git yet.*

# Schema:


## TESTING ##

# Run a simple command with an absolute path

/bin/ls
/bin/pwd
/bin/echo Hello

# Test an empty command

(press enter)

# Test spaces or tabs

"   "   (spaces)
"		"  (tabs)

# Run commands with arguments (no quotes)

/bin/echo Hello World
/bin/ls -l -a
/bin/cat /etc/passwd
/bin/uname -a
/bin/grep root /etc/passwd
/bin/echo Test1 Test2 Test3

🟢 echo

echo Hello World
echo -n No newline
echo "Hello    World"
echo "Special characters: @#$%^&?*()"
echo ""
echo -n
echo "cat lol.c | cat > lol.c"

🟢 exit

exit
exit 0
exit 42
exit -1
exit hello
exit 999
🔄 Relaunch Minishell after exit tests.

🟢 Return value of a process ($?)

/bin/ls
echo $?
/bin/ls filethatdoesntexist
echo $?
expr $? + $?
/bin/grep "something" /etc/passwd
echo $?

🟢 Signals

# In an empty prompt
(ctrl-C)  # Should display a new prompt
(ctrl-\)  # Should do nothing
(ctrl-D)  # Should exit shell (relaunch it)

# In a prompt after typing something
echo "test" (then ctrl-C)  # New line, clean buffer, should NOT execute "test"
echo "test" (then ctrl-D)  # Should do nothing
echo "test" (then ctrl-\)  # Should do nothing

# In a blocking command
cat (then ctrl-C)  # Should return to prompt
cat (then ctrl-\)  # Should do nothing
cat (then ctrl-D)  # Should do nothing

🟢 Double Quotes

echo "Hello    World"
echo "this is a test with    spaces"
echo "cat lol.c | cat > lol.c"

🟢 Single Quotes

echo '$USER'  # Should print "$USER", not the value
echo 'Hello World'
echo 'cat lol.c | cat > lol.c'
echo ''  # Should print an empty line

🟢 env

env
✅ Should print all current environment variables.

🟢 export

export TEST_VAR=hello
export PATH=/new/path:$PATH
env | grep TEST_VAR
✅ Check if TEST_VAR appears in env.

🟢 unset

unset TEST_VAR
env | grep TEST_VAR  # Should print nothing

🟢 cd

cd /
pwd
cd ..
pwd
cd non_existent_directory  # Should print an error
cd .
pwd

🟢 pwd

pwd
cd /tmp
pwd

🟢 Relative Path

./minishell      # If you're inside the directory containing minishell
create a folder and another folder inside:
../minishell     # If you're in the parent directory
../../minishell  # If you're two levels above

cd /usr/bin
./ls

complex relative paths:

cd /
cd usr/bin
../bin/ls


🟢 Environment Path

ls
create a file.txt with something inside # "wc -l file.txt" Should count the number of lines in file.txt.
wc -l file.txt

unset PATH
ls  # Should fail
export PATH=/bin:/usr/bin
ls  # Should work again

🟢 Redirection (<, >, >>, <<)

echo "Hello, world!" > testfile.txt
cat testfile.txt

echo "Appending this line" >> testfile.txt
cat testfile.txt

cat < testfile.txt

ls > output.txt
cat output.txt

cat << EOF
This is a test
EOF

ls > output.txt > output2.txt  # Should write in output2.txt 
✅ Expected Results:

> should overwrite the file.
>> should append to the file.
< should read input from the file.
<< (here-document) should take input until EOF is found.
Multiple > should fail.

🟢 Pipes (|)

cat file.txt | grep bla | more
ls filethatdoesntexist | grep bla | more  # Should fail gracefully
echo "Hello" | cat | cat | cat
echo "test" | cat > file.txt
ls | wc -l
✅ Expected Results:

Pipes should pass output from one command to another.
Errors in commands should be handled gracefully.
Redirection + pipes should work properly.

🟢 Crazy Tests & History (Up/Down, Long Commands, Ctrl-C, Invalid Commands)

# Type something, then press Ctrl-C → It should clear the buffer
dsbksdgbksdghsd  # Should print an error but not crash
cat | cat | ls  # Should work normally
echo "arg1 arg2 arg3 arg4 .... (long command with 100+ args)"
✅ Expected Results:

Ctrl-C should clear the buffer.
Invalid commands should not crash minishell.
History (Up/Down keys) should work.
Long commands should be handled properly.

🟢 Environment Variables ($variable)

echo $USER
echo "My username is $USER"
echo '$USER'  # Should print "$USER", not its value
export TESTVAR="Hello, World!"
echo $TESTVAR
unset TESTVAR
echo $TESTVAR  # Should print nothing
✅ Expected Results:

$USER should print the actual username.
Double quotes should interpolate variables.
Single quotes should not interpolate variables.
unset should remove the variable.

The export and unset commands are commonly used in shell environments to manage environment variables. Here's how they work and how you can use them effectively:

export
The export command is used to set or mark variables to be available to child processes of the shell.

Create and Export a Variable:

bash
export MY_VAR="hello world"

echo $MY_VAR
Output:
hello world

Export a Variable for a Child Process:
bash
export GREETING="Hi there! "

bash -c 'echo $GREETING'
Output:
Hi there!

List All Exported Variables:
bash
export

Potential issues:


-echo "\$HOME" expande la variable, no la toma como literal (output en bash de este comand: $HOME)

-revisar cd  ~

-código de salida erróneo para exit con argumentos (opcional)
