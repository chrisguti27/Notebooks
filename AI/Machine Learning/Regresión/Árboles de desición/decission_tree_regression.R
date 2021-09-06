#Árboles de decisión para Regresión
#Plantilla de Regresión

#---------------------Importar dataset--------------
dataset = read.csv('Position_Salaries.csv')#matriz de caracteristicas
dataset = dataset [,2:3]


#--------------------Ajustar modelo --------------------
#install.packages("rpart")
library(rpart)
regression = rpart(formula = Salary ~ .,
                   data = dataset,
                   control = rpart.control(minsplit = 1))
#el parámetro rpart.control(minsplit = 1) permite definir el número mínimo de datos por hoja
#----------Predicción del modelo de regresión para un valor dado---------
y_pred = predict(regression, newdata = data.frame(Level = 6.5))

#----------------Visualización del Modelo----------------
#install.packages("ggplot2")
library(ggplot2)
x_grid = seq(min(dataset$Level),max(dataset$Level),by = 0.01)#grid para incrementar la resolución de la gráfica de la curva

ggplot() +
  geom_point(aes(x = dataset$Level,y = dataset$Salary),
             color = "red") +
  geom_line(aes(x = x_grid, y = predict(regression, 
                                        newdata = data.frame(Level = x_grid))),
            color = "blue") +
  ggtitle("Predicción del modelo de regresión") +
  xlab("Nivel del empleado") +
  ylab("Sueldo (en $)")
