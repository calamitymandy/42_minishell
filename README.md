# 42_minishell

# Work with GIT:
1- Siempre antes de empezar a trabajar, es importante verificar que estén en su rama o branch.
2- Para actualizar la rama en la que estas trabajando para que incluya los cambios que hay en la rama main:

```
`git checkout main` para volver a la rama main *
`git pull` para traer los últimos cambios de main
`git checkout nombredenuestrabranch` para volver a nuestra rama
`git merge main` para traer los cambios de main a nuestra rama
```

3- Para subir tus cambios a main con PULL REQUEST:

- Desde GitHub, hacer un pull request a main para incluir los cambios de nuestra rama en main.

Comparing changes: `base: main <- compare: nuestra_rama`

Dependiendo de los archivos que hayamos editado, es posible que haya conflictos al hacer el merge. Se puede solucionar en github o en VScode

VSCode nos va a señalar los archivos con conflictos en color rojo y con un signo de excamación(!) a la derecha del nombre del archivo. Para solucionar los conflictos, podemos clicar en cada archivo y usar el botón de Resolve in Merge Editor. Le mostrará una interfaz de usuario adonde podrá seleccionar qué versión del código se quiere conservar o descartar para cada conflicto, y una vez resueltos pueds completar el merge dándole al botón `Complete Merge`.

- Una vez solucionados los conflictos, podrás hacer un commit para subir los cambios de tu rama actualizada a GitHub.

*Nota: Este comando dará un error si tienes cambios en tu rama que no hayas agregado a git todavía.