import numpy as np
import matplotlib.pyplot as plt

class KNN:
    def __init__(self, k=3):
        self.k = k
    
    def fit(self, X, y):
        self.X_train = X
        self.y_train = y
    
    def predict(self, X):
        predictions = []
        for x in X:
            # Calcul des distances euclidiennes
            distances = np.sqrt(np.sum((self.X_train - x) ** 2, axis=1))
            # Trouver les k plus proches voisins
            k_indices = np.argsort(distances)[:self.k]
            k_labels = self.y_train[k_indices]
            # Majoritaire
            most_common = max(set(k_labels), key=list(k_labels).count)
            predictions.append(most_common)
        return np.array(predictions)

# Données d'entraînement
X_train = np.array([[1, 2], [2, 3], [3, 4], [6, 7], [7, 8], [8, 9]])
y_train = np.array(["jaune", "jaune", "jaune", "bleu", "bleu", "bleu"])

# Données de test
X_test = np.array([[2, 8], [9, 6], [1, 3], [1, 2]])

# Initialisation et entraînement du modèle
knn = KNN(k=3)
knn.fit(X_train, y_train)

# Prédictions
predictions = knn.predict(X_test)
print("Prédictions :", predictions)

# Affichage des données
colors = {"jaune": "yellow", "bleu": "blue"}

plt.figure(figsize=(8, 6))
for i, label in enumerate(y_train):
    plt.scatter(X_train[i, 0], X_train[i, 1], color=colors[label], label=label if label not in plt.gca().get_legend_handles_labels()[1] else "")

# Affichage des points testés
for i, pred in enumerate(predictions):
    plt.scatter(X_test[i, 0], X_test[i, 1], color=colors[pred], edgecolors='k', marker='X', s=100, label=f'Test {i+1}')

plt.xlabel("x")
plt.ylabel("y")
plt.title("Visualisation du KNN")
plt.legend()
plt.grid()
plt.show()
