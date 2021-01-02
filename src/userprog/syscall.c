#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "threads/synch.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
static struct lock files_sync_lock;
void validate_void_ptr(const void* pt);


void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&files_sync_lock);
}

static void
syscall_handler (struct intr_frame *f) 
{
  //modified
  int sys_code = *(int*)f->esp;
  switch(sys_code){
       case SYS_HALT:
       
            break;
       case SYS_EXIT:
           exit_wrapper(f->esp);
           break;
       case SYS_EXEC:
           
           break;
       case SYS_WAIT:
          
           break;
       case SYS_CREATE:
       
           break;
       case SYS_REMOVE:
       
           break;
       case SYS_OPEN:
       
           break;
       case SYS_FILESIZE:
       
           break;
       case SYS_READ:
       
           break;
       case SYS_WRITE:
           write_wrapper(f->esp);
           break;
       case SYS_SEEK:

           break;
       case SYS_TELL:

           break;
       case SYS_CLOSE:

           break;
}

 // printf ("system call!\n");
  //thread_exit ();
}






void write_wrapper(void* esp){
    int fd = *((int*)esp+1);
    void* buffer = (void*)(*((int*)esp+2));
    unsigned size = * ((unsigned*)esp+3);
    validate_void_ptr(buffer);
    if(fd==1){
       putbuf (buffer, (size_t) size);
     }

}

void exit_wrapper(void *esp){
    int status = *((int*)esp+1);
    thread_current()->child_status = status;
    thread_exit();
}


void validate_void_ptr(const void* pt){
    if(!((pt!=NULL)&&(is_user_vaddr(pt))&&(pagedir_get_page (thread_current()->pagedir, pt)!=NULL))){
         thread_exit();
     }
}

