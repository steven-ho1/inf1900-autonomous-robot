# Polytechnique Montréal

Département de génie informatique et génie logiciel

INF1900: Projet initial de système embarqué

# Grille de correction des programmes:

Identification:
+ Travail    : Mise en commun du code et formation de librairies
+ Section #  : 1
+ Équipe #   : 0314
+ Correcteur : Charles Jiang

# CODE

| Points importants                                        | Poids       |
| -------------------------------------------------------- | ----------- |
| La qualités et le choix de vos portions de code choisies | (4.5/5.0)      |
| La qualités de vos modifications aux Makefiles           | (3.75/5.0)      |
| __Résultat partiel__                                     | __(8.25/10.0)__ |


makefile
-0.25 il faut specifier le compilateur (avr-ar) au lieu de ar
-0.5 CIBLE HEXROMTRG et HEXTRG non necessaire
-0.5 pas besoin de regle install

code:
-0.25 preferablement mettre les valeurs de prescaler dans un enum 
-0.25 debug.cpp vide fichier inutile

# RAPPORT

| Points importants                                                           | Poids      |
| --------------------------------------------------------------------------- | ---------- |
| Explications cohérentes par rapport au code retenu pour former la librairie | (/2.0)     |
| Explications cohérentes par rapport aux Makefiles modifiés                  | (/2.0)     |
| Explications claires avec un bon niveau de détails                          | (/2.0)     |
| Bon français                                                                | (/1.0)     |
| __Résultat partiel__                                                        | __(7/7.0)__ |

# GÉNÉRALITÉS
## Bonne soumission de l'ensemble du code et du rapport selon le format demandé 

| Pénalité par erreur      | -0.5       |
| ------------------------ | ---------- |
| compilation sans erreurs |            |
    mauvais chemin                x             tp/tp7/exec au lieu de tp/tp7/exec_dir, lib aussi
| etc                      |            |
| __Résultat partiel__     | __(2.5/3.0)__ |


# Résultat

__Total des points: 17.75/20__

# Commentaires du correcteur:

À remplir par l'évaluateur


# Basé sur le commit suivant
```
commit d84f15c680f109061ae0906868b1eed0234b3023
Author: steven-ho01 <steven.ho01@outlook.com>
Date:   Mon Oct 31 16:58:28 2022 -0400

    Ajout de rapport
```

# Fichiers indésirables pertinents
Aucun

# Tous les fichiers indésirables
Aucun

# Sorties de `make` dans les sous-répertoires

## Sortie de `make` dans `tp/tp7/lib`
```
make: *** correction_tp7/0314/tp/tp7/lib : Aucun fichier ou dossier de ce type. Arrêt.

```

## Sortie de `make` dans `tp/tp7/exec`
```
make: *** correction_tp7/0314/tp/tp7/exec : Aucun fichier ou dossier de ce type. Arrêt.

```
