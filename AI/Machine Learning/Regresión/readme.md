# Regresion de datos

## Tipos de Variables
Antes de hablar de regresión, es importante considerar los tipos de variables que nos podemos encontrar dentro de los diferentes datasets.

![Tipo de variables](https://user-images.githubusercontent.com/57780789/125316421-0498f300-e330-11eb-90fe-387ae4060c17.png)

## Regresión
Se llama análisis de regresión al proceso estadístico que se basa en estimar las relaciones que existen entre variables. Es decir, se centra en estudiar la relación de una variable dependiente respecto a una o más variables independientes, o en otras palabras, predecir una variable dependiente en función de una o más. 

### Regresión lineal
Es el proceso de estimación de una varible dependiente en función de una o más varibles independientes, según el modelo matemático:

![Modelo lineal](https://latex.codecogs.com/gif.latex?%5Cdpi%7B100%7D%20%5Cbg_white%20%5Clarge%20y%3Db_0%20&plus;%20b_1%20%5Ccdot%20x_1%20&plus;%20b_2%20%5Ccdot%20x_2%20&plus;%20%5Ccdots%20&plus;%20b_n%20%5Ccdot%20x_n)

Antes de aplicar el modelo de regresión lineal se debe comprobar que el dataset cumpla con las propiedades de:

* Linealidad: Que la relación entre las variables sea lineal.
* Independencia: Que los errores en la medición de las variables explicativas sean independientes entre sí.
* Homocedasticidad: Que los errores tengan varianza constante.
* Normalidad: Que las variables sigan la Ley Normal.
* No colinealidad (Ausencia de multicolinealidad): Que las variables independientes no estén correlacionadas entre ellas.

*Para más detalle: http://doi.org/10.1344/reire2019.12.222704
