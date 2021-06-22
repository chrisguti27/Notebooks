# plantilla para el preprocesado de datos

#importar dataset
dataset = read.csv('Data.csv')#matriz de caracteristicas
#Tratamiento de los NaN
dataset$Age = ifelse(is.na(dataset$Age), 
                     ave(dataset$Age,FUN = function(x) mean(x, na.rm = TRUE)),
                     dataset$Age)
dataset$Salary = ifelse(is.na(dataset$Salary), 
                     ave(dataset$Salary,FUN = function(x) mean(x, na.rm = TRUE)),
                     dataset$Salary)
#codificar las variales categoricas
#la funcion factor () designa que los valores a dar no corresponderan a variables ordinales, 
#sino a v. categoricas
dataset$Country = factor(dataset$Country, levels = c("France","Spain","Germany"),
                         labels = c(1, 2, 3))
dataset$Purchased = factor(dataset$Purchased, levels = c('No',"Yes"),
                           labels = c (0,1))

#Dividir los datos en conjunto de entrenamiento y conjunto de test
#install.packages("caTools")
library(caTools)
#sample.split( Y, SplitRatio = 2/3, group = NULL )
  #SplitRatio el % de datos que se van a usar para el entrenamiento
set.seed(123)
split = sample.split(dataset$Purchased,SplitRatio = 0.8)
# los elementos con false con para test, y las TRUE son para entrenamiento
training_set = subset(dataset,split == TRUE)
testing_set = subset(dataset, split == FALSE)

#ESCALADO DE VALORES
#en R no se escalan las variables dummy
training_set[,2:3] = scale(training_set[,2:3])
testing_set [,2:3]= scale(testing_set[,2:3])
