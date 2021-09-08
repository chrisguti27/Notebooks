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


#----------------CODIFICAR DATOS CATEGORICOS--------
from sklearn.preprocessing import LabelEncoder, OneHotEncoder
from sklearn.compose import make_column_transformer

labelencoder_X = LabelEncoder()
X[:,3] = labelencoder_X.fit_transform(X[:,3])

onehotencoder = make_column_transformer((OneHotEncoder(),
                [3]), remainder = "passthrough")

X = onehotencoder.fit_transform(X)

# en este caso se elimina la primera columna
X = X[:,1:] #hay librerias que por si sola eliminan variables ficticias, o para indicar aquellas que se quiere que formen parte del modelo

#--------------------ENTRENAMIENTO Y TEST----------------------

from sklearn.model_selection import train_test_split

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size = 0.2, random_state = 0)

#ajustar el modelo de regresion lineal multiple con el conjunto de entrenamiento
from sklearn.linear_model import LinearRegression
regression = LinearRegression()
regression.fit(X_train,Y_train) 
# El objeto LinearRegression se para cual quier ajuste linear simple o multiple

#prediccion de los resultados en el conjunto de testing
y_pred = regression.predict(X_test) 

#construir el modelo óptimo de RLM mediante Eliminación hacia atrás
#--------------ELIMINACIÓN HACIA ATRÁS-------------
#Obtener el conjunto de v. independientes estadísticamente significativo
import statsmodels.api as sm
X = np.append(arr = np.ones((50,1)).astype(int),values = X, axis = 1)
#Eliminación hacia atras   
def backwardElimination(SL, X_opt_before):
    regression_OLS = sm.OLS(endog = Y, exog = X_opt_before.tolist()).fit()
    maxP= max(regression_OLS.pvalues).astype(float)
    adjR2_before = regression_OLS.rsquared_adj.astype(float)
    while maxP > SL:
        for j in range(0, len(regression_OLS.pvalues)):                
            if (regression_OLS.pvalues[j].astype(float) == maxP):                    
                X_opt_after = np.delete(X_opt_before, j, 1)
                break
        regression_OLS = sm.OLS(endog = Y, exog = X_opt_after.tolist()).fit()
        adjR2_after = regression_OLS.rsquared_adj.astype(float)
        print(adjR2_after)
        if adjR2_after > adjR2_before:
            X_opt_before = X_opt_after
            adjR2_before = adjR2_after
            maxP = max(regression_OLS.pvalues).astype(float)
        else:
            break
    return X_opt_before

SL=0.05
X_opt= backwardElimination(SL, X)
regression_OLS = sm.OLS(endog = Y, exog = X_opt.tolist()).fit()
print(regression_OLS.summary())