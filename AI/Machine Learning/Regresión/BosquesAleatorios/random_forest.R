#Árboles de decisión para Regresión
#Plantilla de Regresión

#---------------------Importar dataset--------------
dataset = read.csv('Position_Salaries.csv')#matriz de caracteristicas
dataset = dataset [,2:3]


#--------------------Ajustar modelo --------------------
#install.packages("randomForest")
library(randomForest)
set.seed(1234)#se establece un número para cada vez que se ejecute el algoritmo se obtenga el mismo resultada
regression = randomForest(x = dataset[1],
                          y = dataset$Salary,
                          ntree = 500)
#dataset$Salary devuelve un vector debe ser un vector
#dataset [1] devuelve un frame
#----------Predicción del modelo de regresión para un valor dado---------
y_pred = predict(regression, newdata = data.frame(Level = 6.5))

#----------------Visualización del Modelo----------------
#install.packages("ggplot2")
library(ggplot2)
x_grid = seq(min(dataset$Level),max(dataset$Level),by = 0.01)#resignar, significa incrementar la resolución del X_grid

ggplot() +
  geom_point(aes(x = dataset$Level,y = dataset$Salary),
             color = "red") +
  geom_line(aes(x = x_grid, y = predict(regression, 
                                        newdata = data.frame(Level = x_grid))),
            color = "blue") +
  ggtitle("Predicción del modelo de regresión") +
  xlab("Nivel del empleado") +
  ylab("Sueldo (en $)")
