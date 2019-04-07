#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <asm/page_types.h>

#include <linux/mm.h>

#include <linux/proc_fs.h>
//#include <linux/internal.h>

#include <linux/sched/signal.h>

MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Toshchev Konstantin <konst.toshchev@gmail.com>" );

int    p_id;
char * proc_name = "get_pages_by_pid";

static struct proc_dir_entry * pde;

static struct page * travel_to_page_table(struct vm_area_struct *mmap,
					  unsigned long address)
{
    struct page *page = NULL;
    struct mm_struct *mm = mmap->vm_mm;

    //pdg_t pgd;
    //pgd = mm -> pgd;

    //ищем указатель на Page Global Directory
    pgd_t * pgd;
    pgd = pgd_offset(mm, address);
    //проверяем на exist, present and accessed для pgd
    if (pgd_none(*pgd) || pgd_bad(*pgd)) {
        return page;
    }
    printk(KERN_NOTICE "\tValid pgd");

    //ищем указатель на Page for Directory
    p4d_t * p4d;
    p4d = p4d_offset(pgd, address);
    if (p4d_none(*p4d) || p4d_bad(*p4d)) {
        return page;
    }
    printk(KERN_NOTICE "\tValid p4d");

    //ищем указатель на Page Upper Directory
    pud_t * pud;
    pud = pud_offset(p4d, address);
    if (pud_none(*pud) || pud_bad(*pud)) {
        return page;
    }
    printk(KERN_NOTICE "\tValid pud");

    //ищем указатель на Page Middle Directory
    pmd_t * pmd;
    pmd = pmd_offset(pud, address);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) {
        return page;
    }
    printk(KERN_NOTICE "\tValid pmd");

    //ищем указатель на Page Table Entries
    pte_t * ptep, pte;
    ptep = pte_offset_map(pmd, address);
    if (!ptep) {
        return page;
    }
    pte = *ptep;

    page = pte_page(pte);
    if (page)
        printk(KERN_INFO "\tPage frame struct is %p", page);

    pte_unmap(ptep);

    return page;
}

int read_proc(char *buf, char **start, off_t offset, 
              int count, int *eof, void *data)
{
    int len = 0;
    struct pid * pid_struct = find_get_pid(p_id);
    struct task_struct * task = pid_task(pid_struct, PIDTYPE_PID);

    printk(KERN_INFO "\nname %s\n ",task->comm);
    len = sprintf(buf,"\nname %s\n ",task->comm);
    return len;
}

// int write_proc(struct file *file, const char *buf, 
//                int count,void *data)
// {
//     int ret;
//     char * id;
//     id = (char *)kmalloc(1000*sizeof(char),GFP_KERNEL);
//     printk(KERN_INFO "buf passed %s",buf);
//     if(copy_from_user(id,buf,count))
//      return -EFAULT;
//     printk(KERN_INFO "id passed %s",id);
//     p_id = simple_strtoul(id,NULL,0);
//     printk(KERN_INFO "pid %d ret %d",p_id,ret);
//     return sizeof(buf);
// }

inline int run(void) {

    pde = proc_create_seq(proc_name, 0666, NULL, NULL);
    // pde = proc_create_reg(proc_name, 0666, NULL, NULL);
    if (!pde) {
        printk( KERN_INFO "Error creating proc entry" );
        return -ENOMEM;
    }

    //pde->read_proc  = read_proc;
    //proc_dir_entry->write_proc = write_proc;
    printk(KERN_INFO "proc initialized");
}

    // int count = 0;
    // int p_id;




    // for_each_process(task) {
    //     printk( KERN_INFO "Search page frame for prosess : \n" );
    //     count++;
    // }

    // return count;

int init_module ( void ) {
    run();
    printk( KERN_INFO "Module start!\n" );
    return 0;
}

void cleanup_module ( void ) {
    printk( KERN_INFO "Module stop!\n" );
    return;
}

//module_init(init_module);
//module_exit(cleanup_module);