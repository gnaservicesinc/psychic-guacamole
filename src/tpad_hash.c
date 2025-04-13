/********************************************************************************* 
 *     COPYRIGHT NOTICE:
 *     Copyright © 2014, 2018 Andrew Smith (GNA SERVICES INC) <Andrew@GNAServicesInc.com>
 *     All Rights Reserved.
 *
 *   This file, tpad_hash.c , is part of tpad.
 *
 *   tpad is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *   tpad is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with tpad.  If not, see <http://www.gnu.org/licenses/>.
 ********************************************************************************/
#include "tpad_headers.h"
extern GtkWidget *window;
#ifndef _Space_ 
#define _Space_ 32
#endif

extern GtkSourceBuffer *mBuff;

char *curBin2sha512(){
	char bbuffer[BUFSIZ];
  	readlink("/proc/self/exe", bbuffer, BUFSIZ);
	return ( (char *)str2sha512( tpad_hash_read_in_file (bbuffer) ) );
}

char *curTxtbuff2sha512(){
	GtkTextIter start,end;
	gtk_text_buffer_get_bounds( GTK_TEXT_BUFFER(mBuff),&start,&end);
	return ( (char *)str2sha512( gtk_text_buffer_get_text(GTK_TEXT_BUFFER(mBuff),&start,&end,TRUE) ) );
	}
	
char *curFile2sha512(){

	if (tpad_fp_get_current() != NULL){ return ( g_strdup((char *)str2sha512( tpad_hash_read_in_file (tpad_fp_get_current()) )) );
	}
	else return ("--\n'\0'");
	}

char *file2sha512(){
	return ( (char *) g_strdup(str2sha512(tpad_hash_choose_file_and_get_contents())));

}
char *file2sha256(){
	return ( (char *) g_strdup(str2sha256(tpad_hash_choose_file_and_get_contents())));
}
char *file2md5(){
	return ( (char *) g_strdup(str2md5(tpad_hash_choose_file_and_get_contents())));
}
char *file2base64(){
	return( (char *) g_strdup( str2base64((char *)tpad_hash_choose_file_and_get_contents())) );
}
char *filefrombase64(){
	return( (char *) g_strdup(strFrombase64((char *) tpad_hash_choose_file_and_get_contents())) );
}
char *str2sha512(const char *str) {
	if(str == NULL) return("--\n'\0'");	

	if(strlen(str) <=1) return("--\n'\0'");	

	unsigned char ichr[1024];
	mbedtls_sha512(str,strlen(str),ichr,0);

    	return (g_strdup(data_to_hex(ichr)));
}
char *str2sha256(const char *str) {
		if(str == NULL) return("--\n'\0'");	
	if(strlen(str) <=1) return("--\n\0");
	
	unsigned char ichr[1024];

	mbedtls_sha256(str,strlen(str),ichr,0);

    	return (g_strdup(data_to_hex(ichr)));
}
char *str2md5(const char *str) {
		if(str == NULL) return("--\n'\0'");	

	if(strlen(str) <=1) return("--\n'\0'");	

	unsigned char ichr[1024];

	if(strlen(str) <=1) return("--\n'\0'");

	mbedtls_md5(str,strlen(str),ichr);

    	return (g_strdup(data_to_hex(ichr)));
}

char* tpad_hash_read_in_file(char* fp){

 if(access(fp, R_OK ) == -1) return("--\n'\0'"); 
 else {
  FILE * pFile;
  unsigned int lSize;

  size_t result;

  pFile = fopen ( fp , "rb" );
  if (pFile==NULL) return("--\n'\0'");

  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  unsigned int* buffer = (unsigned int*) calloc (lSize+1,sizeof(unsigned int));
   if (buffer == NULL) return("--\n'\0'");
  //char  buffer[lSize+1];	
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) return("--\n'\0'");

  fclose (pFile);
  return (buffer);
 }
}
char *strFrombase64(const char *str) {
	if(str == NULL) return("--\n'\0'");	

	if(strlen(str) <=1) return("--\n'\0'");
	
	size_t ssize = strlen(str) + 1;	
	size_t dlen=0,olen=0;
	int ret = 0;
	ret=mbedtls_base64_decode(NULL,0,&dlen,str,ssize-1);

	 unsigned char ichr[dlen *3]; 
	



	switch(mbedtls_base64_decode(ichr,dlen,&olen,str,ssize-1))
	{
	case MBEDTLS_ERR_BASE64_INVALID_CHARACTER :
		return(str2base64(str));
	break;
	
	case MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL:
		return ("Too much!\nSelect less / a smaller file and try again.\n");
	break;

	case 0 :
		return (g_strdup(ichr));
	break;
	default :
		return("--\n");
	}



}
char *str2base64(const char *str) {
	if(str == NULL) return("--\n'\0'");	

	if(strlen(str) <=1) return("--\n'\0'");
	
	size_t ssize = strlen(str) + 1;
		
	int n = 0;
	size_t dlen=0,olen=0;
 	mbedtls_base64_encode(NULL,ssize,&dlen,str,ssize);


	 unsigned char ichr[dlen +1];
	int ret = mbedtls_base64_encode(ichr,dlen,&olen,str,ssize);

	if(ret == 0) return (g_strdup(ichr));

	if (ret == MBEDTLS_ERR_BASE64_BUFFER_TOO_SMALL) {
		return ("Too much!\nSelect less / a smaller file and try again.\n");
	}

	return("--\n");
}
char* tpad_hash_get_file(){
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new("File hash to clipboard",GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "Hash File",
                                         GTK_RESPONSE_ACCEPT,NULL);
    gtk_file_chooser_set_local_only (GTK_FILE_CHOOSER(dialog),TRUE);
    gtk_file_chooser_set_show_hidden (GTK_FILE_CHOOSER(dialog),TRUE);
    gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER(dialog),FALSE);
	if(tpad_fp_get_current()){
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), (gchar*)get_currentfile_basename());
        gtk_file_chooser_set_filename (GTK_FILE_CHOOSER(dialog),(const gchar *)tpad_fp_get_current());
	}
		char *ptrStr=NULL;

	
	if(gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_CANCEL) {
		return(NULL);
	}

	if (gtk_dialog_run(GTK_DIALOG(dialog)) ==  GTK_RESPONSE_ACCEPT)
	{
	ptrStr = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	if(ptrStr == NULL) return("'\0'");

	}



	gtk_widget_destroy(GTK_WIDGET(dialog));
	return(g_strdup(ptrStr));
    
}
char *data_to_hex(void *pdata){

	if(pdata == NULL) return("--\n");

	
	unsigned char* data = (unsigned char*) pdata;



	size_t strLen =strlen(data);
	size_t data_len = 0;
	if( strLen < sizeof(data) / sizeof(data[0]) ){
		data_len=sizeof(data) / sizeof(data[0]) +1;
	}
	else data_len=strLen+1;



	int n = 0;

	char out[data_len * 3];


	for (n = 0; n < data_len; ++n) {
        if (( unsigned int) data[n] > 0 && ( unsigned int) data[n] <= 255 )snprintf(&(out[n*2]), data_len * 2, "%02x", (unsigned int)data[n]);
    }

    return ( g_strdup(out));
}
char* tpad_hash_choose_file_and_get_contents(){
	return(g_strdup(tpad_hash_read_in_file(tpad_hash_get_file())));
}



