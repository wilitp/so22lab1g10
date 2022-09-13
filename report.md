# Grupo 10

<blockquote>

Laboratorio #01

</blockquote>


- De Ipola Guillermo 🐋
- Molina Franco 😶‍🌫️
- Ebner Lautaro 🚙
- Ludueña Zakka Juan Pablo 🖨️

<details>
    <summary> Cómo compilar su código</summary>
    <br>

Para compilar el codigo y crear un ejecutable con nombre mybash

    $ make

Para borrar los archivos objets y el ejecutable

    $ make clean

Para correr el programa se puede ejecutar

    $ make run

ó podemos llamar al ejecutables directamente

    $ ./mybash
</details>

<details>
    <summary> Debugger</summary>
    <br>

Para debuggear el sistema lo haremos con la erramienta valgrind a travez del comando

    $ make debugger

ó directamente corriendo ejecutando el valgrind

    $ make
    $ valgrind ./mybash

</details>

<details>
    <summary> Listar los comandos que ustedes probaron:</summary>
    <br>

```c
$ ls -l | grep -v
$ ls -l | wc -l > prueba.txt
$ errorerror
$ exit
```
</details>

<details>
    <summary> Decisiones de implementación:</summary>
    <br>

</details>

<details>
    <summary> Items relevante:</summary>
    <br>

</details>

<details>
    <summary> Extra: nuestra forma de trabajar</summary>
    <br>


- Manejamos un sistema de  control de verciones: [Bitbucket](https://bitbucket.org/)
- Nos manejamos a travez de ramas (branchs) propias de cada tarea por hacer (normalmente distribuidas por archivos) 
- Comunicación: [Discord](https://discord.com) y [ClickUp](https://app.clickup.com)

</details>







<style>

    blockquote {
        border-left: 3px solid #01ff70 !important;
        padding-left: 1rem !important;
        background-color: #333 !important;

    }

    ul li::before {
        content: "\2022";
        color: #01ff70;
        font-weight: bold;
        display: inline-block; 
        width: 1em;
        margin-left: -1em;
    }

    h1{
        animation: mymove 3s infinite;
    }

    h2{
        color: #01ff70 !important;
    }

    h4{
        color: #01ff70 !important;
    }

    summary{
        animation: mymove 3s infinite;
        font-weight: bold;        
    }

    a{
        text-decoration: underline !important;
        color: #01ff70 !important;
    }
    
    *{
        font-family: "Courier New";
        background-color: #333;
        color: #fafafa;
    }

    @keyframes mymove {
        from {
            color: #01ff70;
        }
        50% {
          color: white; 
        }
        to {
          color: #01ff70;
        }
    }
</style>
