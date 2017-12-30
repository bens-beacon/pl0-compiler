# Compiler für PL0

## Was ist noch zu tun?

* Liste für Label fehlt noch -> Erklärung File Beck.
* Löschen ist noch unklar! Nur die aktuelle Procedur oder alles löschen?
* Wann wird das Codefile geöffnet und geschlossen? -> Momentan in main und nicht jedesmal
* Codelänge wo bekomme ich diese her? Zeilen Spalten gezählt?
* Fehler bei Funktion Codeout. Die Variable AktProc wird nicht gesetzt
* In der SMR sind pName, AktProc, Val gobal definiert... richtig?

## Dateien

* color.h
* list.h
* list.c
* namelist.h
* smr.h
* smr.c
* code.h
* codegen.h
* codegen.c
* lexer.h
* lexer.c
* parser.h
* parser.c <-- Main()


## Ablauf der Compilers

Als erstes wird initLex() aufgerufen. Dort wird die Datei geöffnet und das
erste Morphem geholt. Als nächstes wird pars() aufgerufen. Diesem wird
der Startbogen übergeben und er holt sich das erste Morphem vom Lexer.
Der Parser geht in den Startbogen ruft sich rekrusiv auf. Dabei wird der
nächste Graf übergeben. Wenn der Token mit dem Wort übereinstimmt 
welches der Graf anfordert z.B. Const im Bogen gleich dem Morphem 
"Const", kann in dem Bogen weitergegangen werden.

### Konstante

Wenn eine Konstante anlegt werden soll. Wird erst geschaut ob es den
Konstantennamen schon gibt wenn nicht, wird geschaut ob der Wert für die 
neue Konstante schon im Konstantenblock vorhanden ist. Wenn nicht wird 
die Konstante in den Konstantenblock geschrieben und ein Bezeichner 
erstellt mit dem Index auf den Konstantenbereich. Falls die Konstante 
doch schon vorhanden ist. Gibt die Suchfunktion SeachConst() den Index
zurück und für die Neue Konstante wird ein Bezeichner mit dem neuen 
Namen und den Index der Suchfunktion angelegt. 

### Variable

Wenn eine Variabel anleget werden soll, muss ein Bezeichner mit dem 
Namen angelegt werden. Wichtig ist, dass man vorher nochmals schaut ob 
der Bezeichner schon vorher angelegt wurde. Sonst gibt es einen Fehler.
Das Displacment der Variable wird jeweils um 4 inkrementiert. Wichtig 
hier ist dass das SppZZVAr von der Prozedur auch erhöht wurde. Also es
ist 4 wenn die erste Varable angelegt wurde und dort ist der Index 0. 
Das geht dann immer so weiter.

### Prozedur

Um eine neue Prozedur anzulegen wird erst geschaut ob der Name der 
Prozdur schon vorhanden ist. Wenn nicht, wird ein Bezeichner angelegt
und die neue Procedur angehängt. Die Neue Procdur besitzt auch wieder 
eine neue Liste. Das Elternteil zeigt auf die Elternprozedur und der 
Index der Prozedur wird inkrementiert. 

### Codegenerieung

Immer bevor Statement aufgerufen wird der Zwischencode genriert. Das 
geschieht in dem in eine extra Liste geschrieben wird. Dazu wird der 
Funktionsbefehl z.B. 1A für entryproc() in die Liste geschrieben und 
alle dazugehörigen Parameter mit angehängt. Die Funktionen findet man 
im code.h

Beispiel für entryproc - Hauptprozedur:

|--------------------
| 1A | ?? | 1 | 4 ...
|-------------------

1A ... entryproc
?? ... Anzahl Zeichen später eingetragen
 1 ... Variablelänge

Beispiel für Variabel speichern
|--------------
| 03 | 0000 | ...
|--------------

03 ... pushvarlocal
0000 ... Index a

### Funktionen


### Wichtig

Namen, also Prozedurnamen, Variablenamen werden nur innerhalb der aktuellen Prozedur gesucht. Sie gelten ja
nur für diesen Bereich.  

### Label - While / IF
Bei einer WHILE-Schleife wird in die die Codeliste ein label gesetzt. 
Ist aber noch etwas unklar. 

























