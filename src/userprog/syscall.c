#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "threads/synch.h"
#include "threads/vaddr.h"

static void syscall_handler (struct intr_frame *);
static struct lock files_sync_lock;
static void validate_void_ptr(const void* pt);


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
  validate_void_ptr((const void*)f->esp);
  int sys_code = *(int*)f->esp;
  switch(sys_code){
       case SYS_HALT:
            halt_wrapper();
            break;
       case SYS_EXIT:
           exit_wrapper(f->esp);
           break;
       case SYS_EXEC:
           exec_wrapper(f);
           break;
       case SYS_WAIT:
           wait_wrapper(f);
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
       default:
          exit(-1);
          break;
}

}


void halt_wrapper(void *esp){
  halt();
}

void exit_wrapper(void *esp){
    int* temp = (int*) esp+1;
    //int status = get_int((int**)&temp);
    int status = *(temp);
    validate_void_ptr((const void*)temp);
    exit(status);
}

void exec_wrapper(struct intr_frame *f){
    int* temp = (int*)f->esp+1;
    validate_void_ptr((const void*)temp);
    validate_void_ptr((const void*)*temp);
    char *cmd = (void*)(*(temp));
    f->eax = exec(cmd);
}

void wait_wrapper(struct intr_frame *f){
    int* temp = (int*)f->esp +1;
    //int pid = get_int((int**)&temp);
    validate_void_ptr((const void*)temp);
    int pid = *(temp);
    f->eax = wait(pid);
}

void halt(){
  shutdown_power_off();
}

void exit(int status){
   thread_current()->exit_status = status;
   thread_exit();
}

tid_t exec(const char *cmd_line){
     tid_t tid = process_execute(cmd_line);
     if(thread_current()->child_creation_sucess == false){
     return -1;
     }
     return tid;
}

int wait(tid_t pid){
    int status = process_wait(pid);
    return status;
}

void write_wrapper(void* esp){
    int fd = *((int*)esp+1);
    void* buffer = (void*)(*((int*)esp+2));
    unsigned size = * ((unsigned*)esp+3);
    validate_void_ptr(esp+1);
    validate_void_ptr(esp+2);
    if(fd==1){
       putbuf (buffer, (size_t) size);
     }

}



static void validate_void_ptr(const void* pt){
    if(!((pt!=NULL)&&(is_user_vaddr(pt))&&(pagedir_get_page (thread_current()->pagedir, pt)!=NULL))){
         exit(-1);
     }
   
}

