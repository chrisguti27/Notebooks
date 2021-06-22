# -*- coding: utf-8 -*-
"""
Created on Mon Jun 14 14:46:44 2021
 
@author: Chris
"""
#plantilla de preprocesado

#importar librerias

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd 

#-----------------IMPORT DATASET-----------------

dataset = pd.read_csv('Data.csv')
X = dataset.iloc[:,:-1].values 
Y = dataset.iloc[:,-1].values
"""
#------------------TRATAMIENTO NAs-----------------
from sklearn.impute import SimpleImputer 
imputer = SimpleImputer(missing_values = np.nan, strategy = "mean", verbose = 0)
imputer.fit(X[:,1:3])
X[:,1:3] = imputer.transform(X[:,1:3])

#----------------CODIFICAR DATOS CATEGORICOS--------
from sklearn.preprocessing import LabelEncoder, OneHotEncoder
from sklearn.compose import ColumnTransformer

labelencoder_X = LabelEncoder()
X[:,0] = labelencoder_X.fit_transform(X[:,0])

ct = ColumnTransformer(
    [('one_hot_encoder', OneHotEncoder(categories='auto'), [0])], 
    remainder='passthrough')

X = np.array(ct.fit_transform(X), dtype=np.int32)

labelencoder_Y = LabelEncoder()
Y = labelencoder_Y.fit_transform(Y)
"""
#--------------------ENTRENAMIENTO Y TEST----------------------

from sklearn.model_selection import train_test_split

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size = 0.2, random_state = 0)

#.................SCALADO DE VARIABLES..............
from sklearn.preprocessing import StandardScaler
sc_X = StandardScaler()
X_train = sc_X.fit_transform(X_train)
X_test = sc_X.transform(X_test)