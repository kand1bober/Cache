#ifndef PAGE_HEADER
#define PAGE_HEADER

struct page_t
{
    int id;
    size_t sz;
};


struct empty_page 
{
    int id;
};


page_t slow_get_page(int id)
{
    page_t page = {id, 0};
    return page;
}

#endif 
