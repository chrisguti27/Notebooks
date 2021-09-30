#******TAREA1 FIREHOSE****#
#Especialziación AWS
#Christian Gutierrez

import boto3
import time
import pandas as pd
import json

DeliveryStreamName = 'firehose02'
firehose = boto3.client('firehose')

record = {}
#lectura de archivo .tsv
df = pd.read_csv('clickstream-wikipedia.tsv', sep='\t',header = 0)

for index, row in df.iterrows():
    #creacion de arreglo de datos a enviar
    record = {
        'prev': row['prev'],
        'curr': row['curr'],
        'type': row['type'],
        'n':    row ['n']
    }
    #envio de datos
    response = firehose.put_record(
        DeliveryStreamName = DeliveryStreamName,
        Record = {
            'Data': json.dumps(record)
        }
    )
    print (record)
    time.sleep(0.5)
