build:
	rm -f spellingBee.exe
	gcc main.c -o spellingBee.exe

run:
	./spellingBee.exe

run_simp:
	./spellingBee.exe -d newsamp.txt

valgrind:
	rm -f spellB_debug.exe
	gcc -g main.c -o spellB_debug.exe
	echo "watched w" > sampleIn.txt
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./spellB_debug.exe < sampleIn.txt

clean:
	rm -f spellingBee.exe
	rm -f spellB_debug.exe

run_play:
	./spellingBee.exe -p -r 7

run_sample:
	echo acdeit > sampleIn.txt
	echo e >> sampleIn.txt
	./spellingBee.exe -d newsamp.txt < sampleIn.txt
profile:
	gcc -pg main.c -o spellingBee_profile.exe
