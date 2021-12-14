void kmemcpy(void *dest, const void * src, unsigned int n){
	char *char_dest = (char *)dest;
	char *char_src = (char *)src;
   	
   	for (int i=0; i<n; i++)
    	cdest[i] = csrc[i];
}
