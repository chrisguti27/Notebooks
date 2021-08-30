#Regresión polinómica

#---------------------Importar dataset-----------
dataset = read.csv('Position_Salaries.csv')#matriz de caracteristicas
dataset = dataset [,2:3]
#codificar las variales categoricas
#la funcion factor () designa que los valores a dar no corresponderan a variables ordinales, 
#sino a v. categoricas


#-------------Ajustar el modelo lineal------------

lin_reg = lm (formula = Salary~.,
              data = dataset)
#-----------Ajustar modelo polinómico -----------
#se añade la columna de los cuadrados de la variable X
#y así sucesivamente según las potencias se requieran
dataset$Level2 = dataset$Level^2 #grado2
dataset$Level3 = dataset$Level^3 #grado3
dataset$Level4 = dataset$Level^4 #grado4

poly_reg = lm (formula = Salary ~ .,
               data = dataset)

#visualización del modelo lineal
#install.packages("ggplot2")
library(ggplot2)
ggplot() +
  geom_point(aes(x = dataset$Level,y = dataset$Salary),
             color = "red") +
  geom_line(aes(x = dataset$Level, y = predict(lin_reg, newdata = dataset)),
            color = "blue") +
  ggtitle("Predicción polinómica del sueldo en función del nivel del empleado") +
  xlab("Nivel del empleado") +
  ylab("Sueldo (en $)")
#visualización del modelo polinómico
ggplot() +
  geom_point(aes(x = dataset$Level,y = dataset$Salary),
             color = "red") +
  geom_line(aes(x = dataset$Level, y = predict(poly_reg, newdata = dataset)),
            color = "blue") +
  ggtitle("Predicción lineal del sueldo en función del nivel del empleado") +
  xlab("Nivel del empleado") +
  ylab("Sueldo (en $)")
#Predicción de nuevos resultados con Regresión Lineal
#En R hay que crear un nuevo dataset, con el mismo label de la columna de 
#variable independiente correspondiente que se usó
#para realizar la predicción, pero en el que únicamente conste el valor que
#se quiere predecir
y_pred = predict(lin_reg, newdata = data.frame(Level = 6.5))


#Predicción de nuevos resultados con Regresión Polinómica
y_pred_poly = predict(poly_reg, newdata = data.frame(Level = 6.5, 
                                                Level2 = 6.5^2, 
                                                Level3 = 6.5^3,
                                                Level4 = 6.5^4 ))
