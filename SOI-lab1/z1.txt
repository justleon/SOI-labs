
Æwiczenie 1.
Kompilacja j±dra i wywo³ania systemowe


1. Cel æwiczenia

Celem æwiczenia jest zapoznanie siê z mechanizmem realizacji wywo³añ
systemowych (ang. system calls, t³umaczone równie¿ jako funkcje
systemowe). W trakcie æwiczenia nale¿y dodaæ do systemu nowe wywo³anie
systemowe, zwracaj±ce numer (indeks) procesu w tablicy procesów. Do
przeprowadzenia æwiczenia niezbêdny jest w³asny obraz systemu MInix 2.0
(minix203.img) uruchamialny w maszynie wirtualnej.


2. Wiadomo¶ci ogólne

Wywo³ania systemowe s± to funkcje realizowane przez j±dro systemu
operacyjnego w kontek¶cie danego procesu. Jest to jedyny dopuszczalny
dla procesów u¿ytkowych sposób wej¶cia do j±dra systemu. Przyk³adowymi
wywo³aniami systemowymi s±: READ, WRITE, FORK, EXIT. Nale¿y odró¿niæ
wywo³ania systemowe od realizuj±cych je funkcji bibliotecznych: read(),
write(), fork(), exit().


3. Wywo³ania systemowe w systemie MINIX

W systemie MINIX wywo³ania systemowe s± realizowane wewn±trz jednego z
modu³ów-serwerów: MM lub FS, w zale¿no¶ci od rodzaju samego wywo³ania.
Wszystkie wywo³ania dotycz±ce szeroko rozumianych operacji plikowych
(np. OPEN, READ, WRITE, IOCTL) s± obs³ugiwane przez FS. Pozosta³e
wywo³ania (np. GETPID, FORK, EXIT) s± obs³ugiwane przez MM. W obu
modu³ach, w plikach table.c, znajduj± siê tablice call_vec, które
okre¶laj±, które wywo³ania systemowe s± w danym module obs³ugiwane i
jaka funkcja tym siê zajmuje. Procesy u¿ytkowe maja do swojej
dyspozycji odpowiednie procedury biblioteczne (np. open(), fork() itd.).

4. Zapoznaj siê z plikami ¼ród³owymi potrzebnymi do generacji
j±dra systemu Minix 2.0 (SM). Pliki znajduj± siê w nastêpuj±cych
katalogach:

/usr/src/kernel- ¼ród³a wszystkich sterowników oraz kodu j±dra

/usr/src/mm - ¼ród³a modu³u Memory Menager (MM)

/usr/src/fs - ¼ród³a modu³u File System (FS)

/usr/src/tools - ¼ród³a programów s³u¿±cych do ³±czenia czê¶ci SM w
jeden bootowalny program oraz do inicjacji systemu.


5. Dodanie do systemu nowego wywo³ania systemowego

Dodaj do systemu nowe wywo³anie systemowe GETPROCNR, obs³ugiwane
wewn±trz modu³u MM i zwracaj±ce numer procesu w tablicy procesów dla
procesu, którego identyfikator (PID) bêdzie argumentem tego wywo³ania.

W tym celu nale¿y:

  1. W pliku /usr/include/minix/callnr.h dodaæ identyfikator nowego
  wywo³ania systemowego GETPROCNR i ewentualnie zwiêkszyæ o jeden
  sta³± N_CALLS.
  
  2. Napisaæ procedurê obs³ugi do_getprocnr i umie¶ciæ ja na przyk³ad
  w pliku /usr/src/mm/main.c
  
  Prototyp tej funkcji umie¶ciæ w pliku /usr/src/mm/proto.h. Funkcja
  ta powinna porównaæ przekazany do niej identyfikator procesu z
  identyfikatorami procesów u¿ytkowych znajduj±cymi siê w tablicy
  mproc, plik /usr/src/mm/mproc.h
  
  Je¿eli identyfikatory te s± takie same, to funkcja musi zwróciæ
  numer (indeks) procesu w tablicy procesów, w przeciwnym razie nale¿y
  zasygnalizowaæ b³±d ENOENT zdefiniowany w pliku /usr/include/errno.h
  
  W zdefiniowanej przez nas procedurze do_getprocnr mamy mo¿liwo¶æ
  odwo³ywania siê do zmiennych typu message:
  
  mm_in - zawiera dane przekazywane do wywo³ania,
  
  mm_out - zawiera dane zwracane do procesu wywo³uj±cego.
  
  W pliku /usr/src/mm/param.h znajduj± siê definicje u³atwiaj±ce
  odwo³ania do elementów wy¿ej wymienionych struktur. Warto¶æ zwracana
  przez nasz± funkcjê do_getprocnr umieszczana jest w strukturze
  mm_out automatycznie (w polu m_type). Zapoznaj siê ze znaczeniem
  pola mp_flags struktury mproc, zwróæ uwagê na flagê IN_USE.
  
  
  3. W pliku /usr/src/mm/table.c w tablicy call_vec w odpowiednim
  miejscu wstawiæ adres (nazwê) funkcji do_getprocnr, za¶ w pliku
  /usr/src/fs/table.c w tym samym miejscu umie¶ciæ adres pusty
  funkcji, no_sys.
  
  
  4. Dokonaæ rekompilacji i prze³adowania systemu Minix z nowym
  j±drem.  Procedura ta ma ró¿ny przebieg w zale¿no¶ci od tego, czy
  æwiczenie wykonywane jest w systemie Minix zainstalowanym na twardym
  dysku, w systemie Minix pracuj±cym pod kontrol± emulatora Bochs, czy
  te¿ wreszcie wykonywane jest w specjalnie przygotowanej dystrybucji
  systemu Minix w ca³o¶ci ³adowanej do RAM-dysku.

  Przyk³adowa rekompilacja i restart systemu Minix:
  
	a. przej¶cie do katalogu /usr/src/tools
    
	b. zapoznanie siê z akceptowalnymi zleceniami dla programu make (czyli
	z zawarto¶ci± konfiguracyjnego pliku tekstowego Makefile) oraz z
	zawarto¶ci± skryptu mkboot,

    
	c. kompilacja nowego j±dra wraz z utworzeniem dyskietki startowej
	z nowym j±drem:
    
	 $ make hdboot
    
	(inne mo¿liwo¶ci, m.in.: "make all", "make fdboot", "make clean" itp)
    
	d. Po zachowaniu zmienionych wersji plików ¼ród³owych nale¿y
	wykonaæ restart systemu z wykorzystaniem utworzonego j±dra. 
	
	 $ cd 
	 $ shutdown  (lub wci¶niêcie <Ctrl-Alt-Del>)
	 $ boot
    
	e. Testowanie w³asno¶ci nowego j±dra. Ewentualne za³adowanie z
	dyskietki uprzednio zachowanych zmian ¼róde³ systemu, a w
	szczególno¶ci zawarto¶ci plików nag³ówkowych, które powinny byæ
	spójne dla j±dra i przygotowywanych pod system z nowym j±drem
	aplikacji.
	
    
6. Program testuj±cy 

Napisz krótki program testuj±cy poprawno¶æ dzia³ania zaimplementowanego
wywo³ania systemowego. Zadaniem programu jest wywo³anie zdefiniowanej
funkcji systemowej dla zadanego jako argument wywo³ania programu
testowego identyfikatora procesu oraz dla 10 kolejnych identyfikatorów
liczbowych procesów.  Przyk³adowo, wywo³anie programu testowego z
argumentem 100 powinno odpytaæ o pozycje w tablicy procesów dla procesów
o identyfikatorach od 90 do 100.  Dla ka¿dego wywo³ania funkcji
systemowej, program testuj±cy powinien wy¶wietlaæ albo zwrócony indeks w
tablicy procesów, albo te¿, w przypadku wyst±pienia b³edu, kod b³êdu
ustawiany przez funkcjê _syscall() w zmiennej errno.

Aby wywo³aæ funkcjê systemow± nale¿y skorzystaæ z funkcji bibliotecznej
_syscall (/usr/include/lib.h). Jej pierwszym argumentem jest numer
serwera (MM lub FS), drugim numer wywo³ania systemowego, trzecim
wska¼nik na strukturê message, w której umieszczamy dane dla wywo³ania.
Deklaracja struktury message znajduje siê w pliku
/usr/include/minix/type.h. Do przekazania numeru PID procesu zaleca siê
u¿ycie pola m1_i1 tej struktury.  Funkcjê _syscall() zdefiniowano w
pliku /usr/src/lib/others/syscall.c 

W programie testuj±cym wskazane jest zaimplementowanie funkcji o
sygnaturze

	 int getprocnr( int )

wewnêtrznie wykorzystuj±cej _syscall() do uzyskania pozycji procesu w
tablicy procesów. Poprawno¶æ rezultatu zwracanego przez wywo³anie
zdefiniowanej funkcji systemowej mo¿na weryfikowaæ analizuj±c aktualn±
zawarto¶æ tablicy procesów oraz wykorzystuj±c komendê ps. Tablicê
procesów mo¿na wy¶wietliæ na pierwszej konsoli poprzez naci¶niêcie
klawisza F1. Analogicznie poprzez wci¶niêcie F2 uzyskuje siê aktualn±
mapê pamiêci.


