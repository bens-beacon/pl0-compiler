# Compiler für PL0

## Bedienung

* mit ``` ./make  ``` Kompiler erstellen
* mit ``` ./compile file.pl0  ``` PL0-Datei kompilieren
* um Comileroutput zu zeigen ``` ./compile file.pl0  -I ```
* um Programm auszuführen ``` ./tests/rlinux file.cl0  ```

## Was ist noch zu tun?

* DO WHILE in DO Until ändern

### Fragen

... keine

### Infos

* Statement Bogen 17 ";" kann immer geschrieben werden. 
* Stringausgabe funktioniert
* IF ELSE funktioniert
* DO While funktioniert, jedoch ist die Bedingung invertiert
* Kommentare wie in C funktionieren auch 

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


Um die die Ausgabedatei anzuschauen, kann man das Programm hexdump verwenden.
Oder man verwendet das Ausgabeprogramm von Beck. 

``` hexdump -C 1.cl0  ```

``` ./outCl0 1.cl0  ```



## Funktionen

### Wichtig

Namen, also Prozedurnamen, Variablenamen werden nur innerhalb der 
aktuellen Prozedur gesucht. Sie gelten ja nur für diesen Bereich.  

### Label - While / IF

Labels merken sich den aktuellen Stand des Schreibzeigers auf den 
Codeausgabebereich. Werden bei IF und WHILE verwendet. Für die Labels 
gibt es eine Liste, Aufbau wie ein Keller. 


Labels beinhalten eine Zahl. Sie werden wie in einen Stack geschrieben.
Verschachtelte Conditions werden von innen nach außen abgearbeitet. Für die 
innere Condition steht das Label ganz oben. Das Label besitzt die Zahl 
die sagt wieviele bytes übersprungen werden müssen um auf die Relativadresse
zuzugreifen 























