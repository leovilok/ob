extern char pixels[256*256];
extern char input;

unsigned char x = 128, y = 128;

unsigned iter;

char color(int i){
	return (char) ((i%128)*8/128)<<5 | (((i/256)*16/256)%8)<<2 | (i%256 < 128 ? 0:1) | (i/256 < 128 ? 0:2);
}

void init(){
	for(int i = 0 ; i < 256*256 ; i++){
		pixels[i] = color(i);
	}
}

void loop(){
	if(input & 2)
		x++;
	if(input & 1)
		x--;
	if(input & 4)
		y--;
	if(input & 8)
		y++;

	pixels[x+y*256] = color(256*256-(x+y*256));
}

