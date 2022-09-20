# Grupo 10 

### Laboratorio #01


* De Ipola Guillermo 🐋 
* Molina Franco 😶‍🌫️ 
* Ebner Lautaro 🚙 
* Ludueña Zakka Juan Pablo 🖨️


<br>
<h3 id="comp">Cómo compilar y correr el código</h3> 

Para compilar el código y crear un ejecutable con nombre mybash:

```bash
$ make
```

Para borrar los archivos objects y el ejecutable:

```bash
$ make clean
```

Para correr el programa se puede ejecutar:

```bash
$ make run
```

o podemos llamar al ejecutable directamente, luego de haber [compilado](#comp)

```bash
$ ./mybash
```

<br>

### Debugger

Para debuggear el sistema lo haremos con la herramienta valgrind a través del comando

```bash
$ make debugger
```

o directamente corriendo ejecutando el valgrind, luego de haber [compilado](#comp)

```bash
$ valgrind ./mybash
```

Para correr los test al programa, se ejecuta el siguiente código

```bash
$ make test
```


<br>

###  Lista de comandos ya probados:

```bash
$ ls -l | grep -v
$ ls -l | wc -l > prueba.txt 
$ curl -o google.html google.com | grep moved & 
$ errorerror 
$ clear 
$ exit 
```

<br>

### Modularización
A continuación listamos los módulos y para cada uno explicamos sus principales funciones.

#### Mybash: entrypoint y main loop

- muestra el prompt y usa el módulo parsing múltiples veces para consumir toda una linea.
- cada vez que parsea una pipe la ejecuta usando el módulo execute.

<h4 id="exec">Execute: </h4>

- `execute_pipeline` ejecuta los comandos de una pipe conectando la salida de un comando con el siguiente.

- `execute_command` se encarga de ejecutar un comando simple. También crea un nuevo pipe si este no es el último y devuelve el file descriptor para leerlo.

- `in_out_redirs` hace las redirecciones de input y output, recibiendo como argumento el file descriptor de lectura del comando anterior y el de escritura del comando que se esté corriendo.

<h4 id="pars">Parsing: </h4>

- `parse_scommand`
    - usa el parser para leer el siguiente argumento hasta encontrarse con un pipe(|), un amperson(&) o no tener mas que leer (\n)
    - utiliza el tad del scomand para devolver un comando que refleje lo leído 

<br>

- `parse_pipeline`
    - utiliza parse_scommand para conseguir el siguiente comando.
    - consume un pipeline y deja la lectura del archivo justo después de un "&", un "\n" o antes del EOF.
    

#### Builtin

- implementa tres comandos internos: cd, exit y help.
- expone funciones para determinar si un comando es interno o no.
- se encarga de la ejecución de los determinado comandos internos.

<br>

###  Decisiones de implementación:

#### Tipo de datos

- scommand
    - contiene una lista enlazada para representar a los argumentos.
    - dos strings para los nombres de los archivos de redireccion.

- pipeline
    - contiene una lista de comandos.
    - un booleano que determina si hay que esperar.

para las listas en ambos tipos, usamos [GLists](https://docs.gtk.org/glib/struct.List.html) de la libreria Glib.

### Jerarquía de procesos

- Generamos procesos hijos para cada comando simple de una pipeline.
- Conectamos su salidas y su entradas a través de pipes.
- Execute_pipeline trackea los pids de dichos procesos para esperarlos a todos si el pipeline esta seteado como foreground.
- En cada impresion del prompt, eliminamos todos los procesos zombies hasta ese momento (producto de haber ejecutado pipelines en background).

<br>

###  Items relevantes

- La implementación de la función `parse_pipeline` no pasa un único test, debido a que nuestra shell soporta escribir varios pipelines por linea; cosa que el test no contempla. 

<br>

### Nuestra forma de trabajar

- Manejamos un sistema de control de versiones: [Bitbucket](https://bitbucket.org/)
- Nos manejamos a través de ramas propias a cada tarea por hacer (normalmente distribuidas por archivos).
- Comunicación: Grupo de [WhatsApp](https://web.whatsapp.com/).

<br>

###  Extras - Puntos estrella

- Generalizar el comando pipeline “|” a una cantidad arbitraria de comandos simples.
- Imprimir un prompt con información relevante: nombre del host, nombre de usuario y camino absoluto.
- Ejecutar una cantidad arbitraria de pipelines por linea (separados por "&").

<br>	
<br>

![FAMAF](https://www.famaf.unc.edu.ar/static/assets/logosFooterBottom.svg)

