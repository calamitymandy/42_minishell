# 42_minishell

# Work with GIT:
1- Siempre antes de empezar a trabajar, es importante verificar que estén en su rama o branch.
2- Si quieres actualizar la rama en la que estas trabajando para que incluya los cambios que hay en la rama main, o hacer un pull request para incluir los cambios de su rama en la rama main, sigue los siguientes pasos:

```
`git checkout main` para volver a la rama main *
`git pull` para traer los últimos cambios de main
`git checkout nombredenuestrabranch` para volver a nuestra rama
`git merge main` para traer los cambios de main a nuestra rama
```
- Dependiendo de los archivos que hayamos editado, es posible que haya conflictos al hacer el merge. Se puede solucionar en github o en VScode

VSCode nos va a señalar los archivos con conflictos en color rojo y con un signo de excamación(!) a la derecha del nombre del archivo. Para solucionar los conflictos, podemos clicar en cada archivo y usar el botón de Resolve in Merge Editor. Le mostrará una interfaz de usuario adonde podrá seleccionar qué versión del código se quiere conservar o descartar para cada conflicto, y una vez resueltos pueds completar el merge dándole al botón `Complete Merge`.

- Una vez solucionados los conflictos, podrán hacer un commit para subir los cambios de su rama actualizada a GitHub.

- Desde GitHub, hacer un pull request a main para incluir los cambios de nuestra rama en main. De este modo, no tendremos conflictos que solucionar en GitHub.

*Nota: Este comando dará un error si tienes cambios en tu rama que no hayas agregado a git todavía.