# Grupo 10 

#### Laboratorio #01


* De Ipola Guillermo 🐋 
* Molina Franco 😶‍🌫️ 
* Ebner Lautaro 🚙 
* Ludueña Zakka Juan Pablo 🖨️


<br>
<h4 id="comp">Cómo compilar y correr el código</h4> 

Para compilar el codigo y crear un ejecutable con nombre mybash 

```bash
$ make
```

Para borrar los archivos objets y el ejecutable

```bash
$ make clean
```

Para correr el programa se puede ejecutar

```bash
$ make run
```

ó podemos llamar al ejecutables directamente, luego de haber [compilado](#comp)

```bash
$ ./mybash
```

<br>
#### Debugger

Para debuggear el sistema lo haremos con la herramienta valgrind a traves del comando

```bash
$ make debugger
```

ó directamente corriendo ejecutando el valgrind, luego de haber [compilado](#comp)

```bash
$ valgrind ./mybash
```

Para correr los test al programa, se ejecuta el siguiente codigo

```bash
$ make test
```


<br>
####  Lista de comandos ya probados:

```bash
$ ls -l | grep -v
$ ls -l | wc -l > prueba.txt 
$ curl -o google.html google.com | grep moved & 
$ errorerror 
$ clear 
$ exit 
```



<br>
####  Partes del código:

##### Mybash:

- muestra el prompt y usa parcepipeline para consumir toda una linea
- cada vez que parsea una pipe la ejecuta en el execute

<h5 id="exec">Execute: </h5> 

- poseemos dos comandos referidos a ejecurtar pipelines o comandos los cuales

- execute_pipeline ejecuta los comandos de una pipe conectando la salida de un comando con el siguiente

- y execute_command los ejecuta en paralelo a los comandos de una pipe

- ejecuta un comando simple y devuelve su     file descriptor de su output

- Redireccion
    setea el standar_input y standar_output de cada comando segun corresponda

- Funciones con respecto al array
    definimos una funcion que convierte los argumentos de un comando a un array
    y otra funcion la cual los libera

<h5 id="pars">Parsing: </h5> 

- Parsear un comando
    - usa el parser para leer el siguiente argumento hasta encontrarse con un pipe(|), un amperson(&) o no tener mas que leer (\n)
    - utiliza el tad del scomand para devolver un comando que refleje lo leído 

<br>

- Parsear una pipeline
    - utiliza parse_comand para conseguirt el siguiente comando 
    - va consumiendo cada uno iteradamente hasta que no haya otro
    devuelve el pipeline
    

##### Builtin

- implementa tres comandos internos: cd, exit y help
- expone funciones para determinar si un comando es interno o no
- se encarga de la ejecucion de los determinado comandos internos

<br>
####  Decisiones de implementación:

##### Tipo de datos

- scommand_s 
    - contiene una lista enlazada para rempresentar a los argumentos
    - dos strings para los nombres de los archivos de redireccion

- pipeline_s
    - contiene una lista de comandos
    - un booleano que determina si hay que esperar

- listas
    para las listas en ambos tipos, usamos [GLists](https://docs.gtk.org/glib/struct.List.html) de la libreria Glib

##### Funciones modularizadas 

- En execute
    - [in_out_redirs](#exec)
    - [arg_array](#exec)
    - [free_arg_array](#exec)
    - [execute_command](#exec)

- En parsing
    - [parse_scomand](#pars)
    - [clean_garbage](#pars)

#### Jerarquía de procesos

- Generamos porcesos hijos para cada comando simple de una pipeline
- Conectamos sus salidas y sus entradas a travez de pipes
- Execute_pipeline trackea los pids de dichos procesos para esperarlos a todos si el pipeline esta seteado como foreground
- En cada impresion del prompt, eliminamos todos los procesos zombies hasta ese momento (producto de haber ejecutado pipelines en background) 

<br>
####  Items relevante:



<br>
####  Nuestra forma de trabajar

- Manejamos un sistema de control de versiones: [Bitbucket](https://bitbucket.org/)
- Nos manejamos a travez de ramas (branchs) propias de cada tarea por hacer (normalmente distribuidas por archivos)
- Comunicación: Grupo de [WhatsApp](https://web.whatsapp.com/)

<br>
####  Extras - Puntos estrellas:

- Generalizar el comando pipeline “|” a una cantidad arbitraria de comandos simples
- Imprimir un prompt con información relevante: nombre del host, nombre de usuario y camino absoluto.
- Ejecutar una cantidad arbitraria de pipelines por linea (separados por "&")

<br>	
<br>

![FAMAF](https://www.famaf.unc.edu.ar/static/assets/logosFooterBottom.svg)

