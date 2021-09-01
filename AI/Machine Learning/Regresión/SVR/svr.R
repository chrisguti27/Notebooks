#Plantilla de Regresión 

#---------------------Importar dataset--------------
dataset = read.csv('Position_Salaries.csv')#matriz de caracteristicas
dataset = dataset [,2:3]


#--------------------Ajustar modelo SVR --------------------

#*************crear variable de regresión***********
install.packages("e1071")
library(e1071)
regression = svm(formula = Salary ~ ., 
                 data = dataset,
                 type = "eps-regression",
                 kernel = "radial")

#----------Predicción del modelo de regresión para un valor dado---------
y_pred = predict(regression, newdata = data.frame(Level = 6.5))

#----------------Visualización del Modelo----------------
#install.packages("ggplot2")
library(ggplot2)
x_grid = seq(min(dataset$Level),max(dataset$Level,0.1))#grid para incrementar la resolución de la gráfica de la curva

ggplot() +
  geom_point(aes(x = dataset$Level,y = dataset$Salary),
             color = "red") +
  geom_line(aes(x = x_grid, y = predict(regression, 
                                        newdata = data.frame(Level = x_grid))),
            color = "blue") +
  ggtitle("Predicción del modelo de regresión") +
  xlab("Nivel del empleado") +
  ylab("Sueldo (en $)")
