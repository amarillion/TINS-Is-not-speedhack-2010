#include "elemsun.h"

#define TAB_SIZE 4

void readSource() {
	PACKFILE *f=pack_fopen("levels.cpp","r");
	if (f==NULL) {
		allegro_message("Cannot find levels.cpp");
		exit(EXIT_FAILURE);
	}
	char buffer[256];
	while (pack_fgets(buffer,256,f)) {
		//Start of a function?
		if (strncmp(buffer,"static void ",12)==0) {
			//Get the name
			char *nameStart=buffer+12;
			int nameLen=0;
			while (nameStart[nameLen]!='(' && nameStart[nameLen]!='\0') nameLen++;
			//Look for a matching behaviour
			Behaviour *b=NULL;
			for (int i=0; i<numLevels; i++) {
				for (int j=0; j<MAX_BEHAVIOURS; j++) {
					Behaviour *b2=levels[i].behaviours[j];
					if (b2!=NULL && b2->numSourceLines==0) {
						if (strncmp(b2->name,nameStart,nameLen)==0 && b2->name[nameLen]=='(') b=b2;
					}
				}
			}
			if (b!=NULL) {
				//Read the source into this behaviour
				while (true) {
					if (b->numSourceLines>=MAX_SOURCE_LINES) {
						allegro_message("Function too long: %s",b->name);
						exit(EXIT_FAILURE);
					}
					//Convert initial tabs to spaces
					int tabCount=0;
					while (buffer[tabCount]=='\t') tabCount++;
					//Strip any final \r and \n
					int len=strlen(buffer);
					while (len>0 && (buffer[len-1]=='\n' || buffer[len-1]=='\r')) len--;
					//Copy the line and store it into the behaviour
					char *line=new char[len-tabCount+tabCount*TAB_SIZE+1];
					memset(line,' ',tabCount*TAB_SIZE);
					memcpy(line+tabCount*TAB_SIZE,buffer+tabCount,len-tabCount);
					line[len-tabCount+tabCount*TAB_SIZE]='\0';
					b->sourceLines[b->numSourceLines++]=line;
					//Have we finished?
					if (buffer[0]=='}') break;
					if (!pack_fgets(buffer,256,f)) break;
				}
			}
		}
	}
	pack_fclose(f);
}
