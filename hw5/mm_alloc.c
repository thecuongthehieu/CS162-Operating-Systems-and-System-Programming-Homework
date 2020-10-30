/*
 * mm_alloc.c
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

s_block_ptr HeadPtr=NULL;

s_block_ptr extend_heap (s_block_ptr last , size_t s){
	void* p=sbrk(s+sizeof(s_block));
		
	if(!(p==(void *)-1)){
		
		s_block_ptr newBlock = (s_block_ptr) p;
		if(last){
			last->next=newBlock;
			
		}else{
			HeadPtr=newBlock;
		}
		newBlock->prev=last;
		newBlock->next=NULL;
		newBlock->size=s;
		newBlock->block=p+sizeof(s_block);
		//last->ptr=p;
		newBlock->free=0;
		
		return newBlock;
	}
	//Return Null on error
	return NULL;
}

s_block_ptr get_block (void *p){
	s_block_ptr head=HeadPtr;
	
	while(head){
		if(head->block==p){
			return head;
		}
		head=head->next;
	}
	//null if we cant find it.
	return NULL;
	
}

s_block_ptr fusion(s_block_ptr b){
	if( (b->next)->free ==1 ){
		b->size=b->size+sizeof(s_block)+(b->next)->size;
		b->next=(b->next)->next;
		(b->next)->prev=b;
	}
	
	if( (b->prev)->free ==1 ){
		
		(b->prev)->next=b->next;
		(b->next)->prev=b->prev;
		(b->prev)->free=b->free;
		(b->prev)->size=(b->prev)->size + sizeof(s_block) + b->size;
		
		return b->prev;
	}
	
	return b;
}

void split_block (s_block_ptr b, size_t s){
	if(b && s >= sizeof(void *)){
		if(  b->size - s  >= sizeof(s_block) + sizeof(void*) ){
			s_block_ptr newP = (s_block_ptr) (b->block + s);
			newP->next = b->next;
			(newP->next)->prev=newP;
			b->next=newP;
			newP->prev=b;
			newP->size=b->size - s - sizeof(s_block);
			b->size = s;
			newP->block= b->block + s + sizeof(s_block);
			
			mm_free(newP->block);
			
		}
	}
}

void* mem_copy(s_block_ptr oldB, s_block_ptr newB){
	if( oldB && newB){
		char * oStart= (char *) oldB->block;
		char * nStart=(char *) newB->block;
		int i;
		for(i=0;i<oldB->size;i++){
			*(nStart + i)=*(oStart + i);
		}
		
		return newB->block;
		
	}
	
	return NULL;
}


void* mm_malloc(size_t size){
	
	s_block_ptr head=HeadPtr;
	s_block_ptr prev=NULL;
	
	while(head){
		if(head->free==1 && head->size >=size){
			mm_realloc(head->block,size);
			head->free=0;
			return head->block;
			
		}else{
			prev=head;
			head=head->next;
		}
	}
	
	//If we exited the loop, we know that we didnt find a suitable space, and so we need to move the break pt.
	head=extend_heap(prev,size);
	if(!head){
		exit(EXIT_FAILURE);
	}
	return head->block;
	
}

/*mm_realloc:
 * If want bigger space then acts the same as malloc but copies over contents of old block to a new, more
 * approproate block.
 * If smaller block is needed but current block isnt big enough to split into two viable blocks, then nothing is
 * done.
 * If current clock is big enough then the block is split accordingly
 */
void* mm_realloc(void* ptr, size_t size){
		s_block_ptr curr=get_block(ptr);
		
		if(curr){
			if(size > curr->size){
				void* p=mm_malloc(size);
				s_block_ptr newP=get_block(p);
				if(newP){
					p=mem_copy(curr,newP);
					mm_free(curr->block);
					return p;
				}
				
				
					
			}else if(size < curr->size){
				split_block(curr,size);
				return curr->block;
			}else{
				return curr->block;
			}
		}
		return NULL;
}

void mm_free(void* ptr){
	s_block_ptr curr=get_block(ptr);
	
	if(curr){
		if(curr->next==NULL){
			
			if(curr->prev){
				
				(curr->prev)->next=NULL;
			}else{
				HeadPtr=NULL;
			}
			
			sbrk(-(curr->size + sizeof(s_block)));
		}else{
			
			curr->free=1;
			fusion(curr);
		}
	}
}

