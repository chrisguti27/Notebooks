# -*- coding: utf-8 -*-
"""
Created on Thu Jul  1 19:25:20 2021

@author: Chris
"""
#regresion lineal multiple

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 

#-----------------IMPORT DATASET-----------------

dataset = pd.read_csv('50_Startups.csv')
X = dataset.iloc[:,:-1].values 
Y = dataset.iloc[:,-1].values

#el dataset presenta datos categoricos, los valores de la variable 'state'
# solo se codifica X, ya que ahi esta la v. categorica

#----------------CODIFICAR DATOS CATEGORICOS--------
from sklearn.preprocessing import LabelEncoder, OneHotEncoder
from sklearn.compose import make_column_transformer

labelencoder_X = LabelEncoder()
X[:,3] = labelencoder_X.fit_transform(X[:,3])

onehotencoder = make_column_transformer((OneHotEncoder(),
                [3]), remainder = "passthrough")

X = onehotencoder.fit_transform(X)
#se cambia el orden de las variables (en el orden de las columnas)
#, las primeras columnas lo ocupan las variable dummy.

#Evitar la trampa de las variables dummy (multicolinealidad), 
#hay que usar solo n-1 variable categoricas

# en este caso se elimina la primera columna
X = X[:,1:] #hay librerias que por si sola eliminan variables ficticias, o para indicar aquellas que se quiere que formen parte del modelo

#--------------------ENTRENAMIENTO Y TEST----------------------

from sklearn.model_selection import train_test_split

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size = 0.2, random_state = 0)
"""
#.................SCALADO DE VARIABLES..............
from sklearn.preprocessing import StandardScaler
sc_X = StandardScaler()
X_train = sc_X.fit_transform(X_train)
X_test = sc_X.transform(X_test)
"""
#ajustar el modelo de regresion lineal multiple con el conjunto de entrenamiento
from sklearn.linear_model import LinearRegression
regression = LinearRegression()
regression.fit(X_train,Y_train) 
# El objeto LinearRegression se para cual quier ajuste linear simple o multiple

#prediccion de los resultados en el conjunto de testing
y_pred = regression.predict(X_test) 

#construir el modelo óptimo de RLM mediante Eliminación hacia atrás
#**********ELIMINACIÓN HACIA ATRÁS****************#
#Obtener el conjunto de v. independientes estadísticamente significativo
import statsmodels.api as sm
X = np.append(arr = np.ones((50,1)).astype(int),values = X, axis = 1)
    #arr: matriz a la que se le añade la otra matriz, se posiciona primera desde la derecha
    #values: matriz que se va a añadir
    #axis: eje en el que se añade 1:column 0:fila

#matriz de caracteristicas optimas X_opt
#1. se considera el modelo con todas las variables independientes
X_opt = X[:,[0,1,2,3,4,5]]
#1.1. se define el nivel de significación
SL = 0.05
#2. se calcula el modelo de RLM para el X_opt
regression_OLS = sm.OLS(endog = Y, exog = X_opt.tolist()).fit()
    #endog = variable que se desea predecir
    #exog = matriz de variables caracteristicas X, incluida la columna de 1s para el término independiente
#3. identificar la variable predictora con el Pvalor mas grande y que cumple con P>SL
regression_OLS.summary()# el pvalor mas grande lo tiene la varible X2
    #4. Si se cumple lo anterior, entonces se elimina la variable predictora(X2)
X_opt = X[:,[0,1,3,4,5]]
    #5. Se ajusta el modelo sin la variable dada y se calcula de nuevo el pvalor (se regresa al paso 3)
regression_OLS = sm.OLS(endog = Y, exog = X_opt.tolist()).fit()
regression_OLS.summary()# el pvalor mas grande lo tiene la varible X1
    #se elimina la variable predictora(X1)
X_opt = X[:,[0,3,4,5]]
    # Se ajusta el modelo sin la variable X1
regression_OLS = sm.OLS(endog = Y, exog = X_opt.tolist()).fit()
regression_OLS.summary()# el pvalor mas grande lo tiene la varible X2
    #se elimina la variable predictora(X2)
X_opt = X[:,[0,3,5]]
    # Se ajusta el modelo sin la variable X2
regression_OLS = sm.OLS(endog = Y, exog = X_opt.tolist()).fit()
regression_OLS.summary()# el pvalor mas grande lo tiene la varible X2 y es > SL (0.05)
    #se elimina la variable predictora(X2)
X_opt = X[:,[0,3]]
    # Se ajusta el modelo sin la variable X2
regression_OLS = sm.OLS(endog = Y, exog = X_opt.tolist()).fit()
regression_OLS.summary()# ya no se cumple la condicion de Pvalor>SL












