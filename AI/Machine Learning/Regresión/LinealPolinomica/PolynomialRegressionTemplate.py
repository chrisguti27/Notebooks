# -*- coding: utf-8 -*-
"""
Created on Mon Aug 30 12:30:04 2021

@author: Chris
"""
#regresión template
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 

#-----------------IMPORT DATASET-----------------

dataset = pd.read_csv('Position_Salaries.csv')
X = dataset.iloc[:,1:2].values 
Y = dataset.iloc[:,2].values

"""
#--------------------ENTRENAMIENTO Y TEST----------------------

from sklearn.model_selection import train_test_split

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size = 0.2, random_state = 0)
"""

"""
#.................ESCALADO DE VARIABLES..............
from sklearn.preprocessing import StandardScaler
sc_X = StandardScaler()
X_train = sc_X.fit_transform(X_train)
X_test = sc_X.transform(X_test)
"""

#Ajustar con el dataset
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
Poly_reg = PolynomialFeatures(degree = 4) 
X_poly = Poly_reg.fit_transform(X) #(de X^0 a X^3)
lin_reg_2 = LinearRegression()
lin_reg_2.fit(X_poly, Y)

#prediccion
#Lin_reg.predict([[6.5]]) # para regresion lineal simple y multiple
lin_reg_2.predict(Poly_reg.fit_transform([[6.5]]))

#plot
X_grid = np.arange(min(X),max(X),0.1)# para evitar que el gráfico salga a trozos
X_grid = X_grid.reshape(len(X_grid),1)
X_1 = Poly_reg.fit_transform(X_grid) #los X que se quieren predecir en el plot
plt.scatter(X, Y, color = "red")
plt.plot(X_grid, lin_reg_2.predict(X_1), color = "blue")
plt.title("Modelo de Regresión Polinómico n4")
plt.xlabel("Posición del empleado")
plt.ylabel("Sueldo (en $)")
plt.show()
