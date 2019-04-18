# marp-project

## Estructura de los directorios
    marp-project
        out -> directorio por defecto de los archivos que se generan
        bin -> directorio por defecto donde se generan los ejecutables
        headers -> headers del programa
        src -> source files
        resources -> casos de prueba
        paper -> memoria con las graficas
        scripts -> bash scripts para acelerar la ejecucion del programa
        graphs -> directorio con las graficas generadas por gnuplot        

## Scripts
    execute.sh directorio outfile
        directorio -> nombre del directorio con los archivos
        outfile -> nombre del archivo de salida -> va automaticamente a el directorio out/

    gnuplot -e "filename='$nombre'" graph.p
        genera una grafica en graphs/ con el nombre $nombre.png del archivo out/$nombre.dat
