#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GPIO_BASE 0xFE200000
#define GPFSEL2   0x08
#define GPSET0    0x1C
#define GPCLR0    0x28

// USE PIN : GPIO 21 (BCM 21/ GPIO Pin Num 40)
        
        
int main(){

  // pi 4 
  int fd = open( "/dev/gpiomem", O_RDWR|O_SYNC );    
  
  if ( fd < 0 ){    
    printf( "can't open /dev/gpiomem \n" );    
    exit(-1);    
  }    
      

  char *gpio_memory_map = (char *)mmap( 
      NULL, 
      4096, 
      PROT_READ|PROT_WRITE,
      MAP_SHARED, 
      fd, 
      GPIO_BASE);

  if ( gpio_memory_map == MAP_FAILED )    
  {    
    printf( " Error : mmap \n" );    
    exit(-1);    
  }    


  volatile unsigned int* gpio = (volatile unsigned int*)gpio_memory_map;

  /* Address Offset space : 4, Address size : 8bit  (total 1 Offset Adress : 32bit) ?
  *  printf("gpio[x] size : %x \n", sizeof(gpio[GPSET0])); // 4byte * 8 = 32bit ?
  */

  /* GPFSEL2 Register
  '[5:3]' FSEL21 FSEL21 - Function Select 21
  FSEL21 - Function Select 21
  000 = GPIO Pin 21 is an input
  001 = GPIO Pin 21 is an output                        // output : 001
  100 = GPIO Pin 21 takes alternate function 0
  101 = GPIO Pin 21 takes alternate function 1
  110 = GPIO Pin 21 takes alternate function 2
  111 = GPIO Pin 21 takes alternate function 3
  011 = GPIO Pin 21 takes alternate function 4
  010 = GPIO Pin 21 takes alternate function 5
 
  // gpio[GPFSEL2 /4] = 0b 0000 0000  0000 0000  0000 000[0  00]'[00 1]'[000];    */
  gpio[GPFSEL2 /4] |= (1<<3);        // GPIO Pin 21 is an output  ([add 5:3] : 001)

  for (int i=0; i<5; i++ )
  {    
    // GPSET0 /GPCLR0 Register : GPIO BCM num 0~31 Control Register

    //gpio[GPSET0 /4] = 0b 0000 0000  0010 0000  0000 0000  0000 0000; // 21
    gpio[GPSET0 /4] |= (1<<21);      // GPIO SET (ON)
    sleep(1);
    
    //gpio[GPCLR0/4] = 0b 0000 0000  0010 0000  0000 0000  0000 0000; // 21
    gpio[GPCLR0 /4] |= (1<<21);      // GPIO CLR (OFF)
    sleep(1);
  }    
  
  munmap( gpio_memory_map, 4096);    
  
  return 0;    
}  
