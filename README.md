# Systemy Operacyjne 2 - Projekt

## Założenia projektowe

Projekt polega na zapoznaniu się i wykorzystaniu wątków w jezyku c++. Wykorzystane zostały biblioteki thread, mutex oraz condition_variable do obsługi oraz synchronizacji wątków. Do wizualizacji (GUI) została wykorzystana biblioteka ncurses.

## Etap 1

Zadanie polega na narysowaniu planszy o konkretnych wymiarach. Na planszę, w losowych odstępach czasu, wpadają kulki poruszajace sie po planszy. Kulki odbijają się od krawędzi, po 5 odbiciach kulka znika. Dodawkowo, po planszy w pionie porusza sie kwadrat/prostokąt.

## Etap 2

Rozszerzenie programu o mutexy. Kulki ktore wpadna do prostokatu zatrzymują sie, dopóki prostokąt z nich nie zejdzie.

## Etap 3 

Wyeliminowanie pętli gorącego czekania. Na środku planszy należy wyswietlic licznik pokazujacy ile kulek jest uśpionych/zatrzymanych.
