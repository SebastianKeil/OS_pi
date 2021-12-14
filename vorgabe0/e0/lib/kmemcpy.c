void kmemcpy(void *dest, const void * src, unsigned int n){
	char *char_dest = (char *)dest;
	char *char_src = (char *)src;
   	
   	for (unsigned int i=0; i<n; i++){
    	char_dest[i] = char_src[i];
    }
}
