# PrankScreen

Petit programme de prank pour surprendre tes amis.

## Comment ça marche

Au lancement, le programme force une déconnexion de session.
Au retour sur Windows, une image s'affiche en plein écran noir.
Appuie sur Échap pour fermer — tout se nettoie tout seul.

## Compilation

Besoin de Visual Studio installé.

Lance d'abord le script PowerShell pour embarquer ton image :

    .\generate.ps1

Puis compile :

    cl /EHsc main.cpp /Fe:prank.exe /link gdiplus.lib user32.lib gdi32.lib

## Utilisation

Donne l'exe à ta victime, c'est tout.

## Notes

- Testé sur Windows 10 et 11
- L'exe se supprime automatiquement après le prank
- Usage perso uniquement

## IMPORTANT 
Cest une simple interface si vous voulez modifer faite le 
