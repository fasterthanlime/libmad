make &> /tmp/log && gcc minimad.c -o minimad .libs/libmad.a &> /tmp/log && ./minimad < one_second_of_silence.mp3
