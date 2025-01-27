# 42_minishell

# Work with GIT:
1- Siempre antes de empezar a trabajar, es importante verificar que estés en tu rama o branch.

2- Para actualizar la rama en la que estas trabajando para que incluya los cambios que hay en la rama main:

```
- git checkout main -> para volver a la rama main *
- git pull -> para traer los últimos cambios de main
- git checkout nombredenuestrabranch -> para volver a nuestra rama
- git merge main -> para traer los cambios de main a nuestra rama
```

3- Para subir cambios a `main` con PULL REQUEST:

- Desde GitHub, hacer un `pull request` a `main` para incluir los cambios de nuestra rama en main.

Comparing changes: `base: main <- compare: nuestra_rama`

Dependiendo de los archivos que hayamos editado, es posible que haya conflictos al hacer el `merge`. Se puede solucionar en github o en VScode.

VSCode nos va a señalar los archivos con conflictos en color rojo y con un signo de excamación(!) a la derecha del nombre del archivo. Para solucionar los conflictos, podemos clicar en cada archivo y usar el botón de Resolve in Merge Editor. Le mostrará una interfaz de usuario adonde podrá seleccionar qué versión del código se quiere conservar o descartar para cada conflicto, y una vez resueltos pueds completar el merge dándole al botón `Complete Merge`.

- Una vez solucionados los conflictos, podrás hacer un commit para subir los cambios de tu rama actualizada a GitHub.

*Nota: Este comando dará un error si tienes cambios en tu rama que no hayas agregado a git todavía.





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

Pending issues:



-cd ~ no lleva al directorio home, lo parsea como directorio no encontrado
-cd "" (empty argument) no cambia el directorio actual en la terminal, pero la minishell lleva al directorio home
-cd " " o más de un espacio: mismo problema que cd "".


-echo "Hello" > file.txt  debería escribir Hello en file.txt (sin mostrarlo por terminal)
Sin embargo, la minishell muestra Hello en terminal y no lo escribe en file.txt
Mismo error con ">>" en lugar de ">"

-echo "Hello" | cat > test.txt se parsea como Bad file descriptor. Lo mismo que ls | sort > sorted_files.txt o cat < file.txt | grep "Hello" 

LEAKS
-EN la función cmd_arg_creat_n_fill
	while (aux->type == WORD || aux->type == VAR)
	{
		command->args[i] = ft_strdup(aux->content);
		i++;
		aux = aux->next;
	}
no se está liberando el contenido de command->args. Añadidas lineas de debugging en clean_cmd_nodes (TODO: usar mejor del_all_nodes_cmd)