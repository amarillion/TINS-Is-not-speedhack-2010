char *string_from_file( char *filename ){
	FILE *f;
	char buaa[256];
	char *str;	
	long l = 0;
	
	f = fopen(filename,"r");
	for ( l = 0; !feof(f); l += fread(buaa, sizeof(char), 256, f) );
	rewind(f);
	
	str = malloc(sizeof(char) * l );
	fread( str, sizeof(char),l,f );
	fclose(f);
	return str;
	
}

double read_number( char *str, char **tok_r ){
	return atof(strtok_r(str,",\n",tok_r));
}
