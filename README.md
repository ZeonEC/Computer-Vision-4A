# Vision Stéréo 

## Objectif

L'objectif de ce projet est d'estimer la **réconstruction 3D avec de la caméra** à l'aide d'un **système de vision stéréo** (deux caméras).

En comparant la position d'un même point dans **l'image gauche** et **l'image droite**, il est possible de calculer la **disparité** et d'en déduire la **distance du point par rapport aux caméras**.

Principe :

- plus la **disparité est grande**, plus l'objet est **proche**
- plus la **disparité est petite**, plus l'objet est **loin**

---

# Pipeline de vision stéréo

Le processus de reconstruction 3D à partir de deux images se décompose en plusieurs étapes.

---

# 1. Calibration des caméras

La calibration permet de déterminer comment le **monde 3D est projeté sur le plan image 2D**.

Deux types de paramètres doivent être estimés.

## Paramètres intrinsèques

Ils décrivent les propriétés internes de la caméra :

- la **focale** `f`
- le **centre optique** `(cx, cy)`
- la **distorsion de l'objectif**

Ces paramètres sont regroupés dans la **matrice de caméra K**.

## Paramètres extrinsèques

Ils décrivent la transformation entre les deux caméras :

- **R** : matrice de rotation
- **T** : vecteur de translation

Ces paramètres définissent la position relative entre la **caméra gauche** et la **caméra droite**.

Même si les deux caméras sont identiques, une calibration est nécessaire car **de petites différences mécaniques existent toujours**.

La calibration est généralement réalisée à l'aide d'un **damier (checkerboard)**.

---

# 2. Géométrie épipolaire

La géométrie épipolaire décrit la relation géométrique entre les deux caméras.

Principe :

Un point observé dans **l'image gauche** correspond à un point situé sur une **droite épipolaire** dans **l'image droite**.

Cela permet de **réduire fortement la zone de recherche des correspondances**.

La relation entre les points peut être exprimée avec la **matrice fondamentale F** :

p_Rᵀ F p_L = 0

Si les caméras sont calibrées, on peut utiliser la **matrice essentielle E** :

E = [T]ₓ R

---

# 3. Rectification des images

La rectification consiste à transformer les images afin que les **droites épipolaires deviennent horizontales**.

Après rectification :

- les points correspondants se trouvent sur **la même ligne de l'image**
- la recherche de correspondances devient **un problème 1D**

Cela simplifie fortement l'étape de matching.

---

# 4. Mise en correspondance (Matching)

Cette étape consiste à trouver les **points correspondants entre l'image gauche et l'image droite**.

Deux approches principales existent.

## Matching sparse (points)

On détecte uniquement des **points caractéristiques** dans l'image.

Exemples :

- SIFT
- ORB
- SURF

On obtient alors un **ensemble de correspondances entre points**.

## Matching dense (pixels)

On cherche des correspondances pour **presque tous les pixels de l'image**.

Cela permet de calculer une **carte de disparité dense**.

Techniques courantes :

- Block Matching
- Semi Global Matching (SGM)

---

# 5. Calcul de la disparité

Une fois les correspondances trouvées, on calcule la **disparité** :

d = u_L - u_R

avec :

- `u_L` : position horizontale dans l'image gauche
- `u_R` : position horizontale dans l'image droite

Interprétation :

- **disparité grande → objet proche**
- **disparité faible → objet éloigné**

---

# Formulation linéaire

Certains problèmes d'estimation en vision par ordinateur peuvent être écrits sous la forme :

A p = 0

Ce type d'équation apparaît lors de l'estimation de :

- homographie
- matrice fondamentale
- matrice de projection

Pour éviter la solution triviale `p = 0`, on ajoute la contrainte :

||p||² = 1

La solution est généralement obtenue avec la **décomposition en valeurs singulières (SVD)**.

---

# Résumé

1. Calibration des caméras → estimation des paramètres intrinsèques et extrinsèques (R,T)  
2. Géométrie épipolaire → contrainte sur la recherche de correspondances  
3. Rectification → alignement horizontal des droites épipolaires  
4. Matching → recherche des points correspondants  
5. Calcul de la disparité → décalage entre les pixels  
