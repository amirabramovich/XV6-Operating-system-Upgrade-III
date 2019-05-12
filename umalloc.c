#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "mmu.h"
#include "memlayout.h"
#include "proc.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

extern struct proc* myproc(void);

void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}

void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}

void* 
pmalloc(void)
{
  Header *hp, *prevp ,*p;
  struct proc *curproc = myproc();

  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p == freep){
      hp = (Header*)(sbrk(4096));
      hp->s.size = 4096;
      free((void*)(hp + 1));
      prevp = p;
      p = p->s.ptr;
      prevp->s.ptr = p->s.ptr;
      freep = prevp;
      // pde_t *tmp = curproc->pgdir;
      // int dir_offset = PDX(p);
      // tmp += dir_offset;
      // tmp = *tmp | PTE_PAL;
      curproc++;

      return (void*)(PGROUNDUP((uint)(p)+1));//(P2V((pte_t*)(V2P(p) | PTE_PAL)));
    }
  }
}

int 
protect_page(void* ap)
{
  // if ((pte_t*)(V2P(ap + 1) & PTE_PAL)){
  //   ap = (void*)(P2V(pte_t*)(V2P(ap + 1) | PTE_PRO));
  //   return 1;
  // }
  return -1;
}

int 
pfree(void* ap)
{
  // if ((pte_t*)(V2P(ap + 1) & PTE_PAL)){
  //   free(ap);
  //   return 1;
  // }
  return -1;
}
