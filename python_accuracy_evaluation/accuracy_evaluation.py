from sklearn import metrics
import seaborn as sm
import pandas as pd
import matplotlib.pyplot as plt

fname_exp = "file_accuracy_evaluation_labels"
fname_pred = "file_accuracy_evaluation_predicted_scaled_rotated"

f1 = open(fname_exp, 'r')
try:
	expected = [int(line.rstrip('\n')) for line in f1]  # etichette della ground truth, e.g., [1, 3, 0, ..., 3]
finally:
	f1.close()

f2 = open(fname_pred, 'r')
try:
	predicted = [int(line.rstrip('\n')) for line in f2]  # etichette predette, e.g., [1, 1, 1, ..., 3]
finally:
	f2.close()

s = "gesto "

class_names =  [s + str(i) for i in range(1, len(set(expected)) + 1)] # sono le etichette, i nomi delle classi, e.g. ["gesto 1", ... "gesto n"]

print(set(expected))

x = metrics.classification_report(expected, predicted)
print(x)

y = metrics.confusion_matrix(expected, predicted)

df_cm = pd.DataFrame(y, index = [i for i in class_names],
                  columns = [i for i in class_names])
plt.figure(figsize = (10,7))
sm.heatmap(df_cm,annot=True,cmap='YlGnBu', fmt='g')

plt.show()


# precision: quantifica la capacita del classificatore di predire correttamente le classi, i.e., cio che predice e corretto
# per il gesto numero 6 il classificatore ha il 96% di precision, significa che il 96% di cio che il classificatore identifica come "gesto numero 6" e correttamente identificato
# recall: quantifica l'abilita di trovare tutti gli esempi positivi dal dataset
# per il gesto numero 6 il classificatore ha il 43% di recall, significa che di tutti i veri "gesto numero 6" nel dataset, il classificatore ne identifica correttamente il 43%


#nell'asse delle x trovo la ground trouth, y gesto predetto

